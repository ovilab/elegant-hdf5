#ifndef DATASET_H
#define DATASET_H

#include "object.h"
#include "typehelper.h"
#include "logging.h"
#include "demangle.h"
#include "datatype.h"
#include "dataspace.h"
#include "converters/native-converters.h"
#include "converters/std-converters.h"

#ifndef H5CPP_NO_ARMA
#include "converters/armadillo-converters.h"
#endif

#include <iostream>
#include <typeinfo>

namespace h5cpp {

class DatasetWriter : public Writer
{
public:
    DatasetWriter(Dataset* dataset, hid_t datatype);
    virtual void write(const void *buffer) override;
private:
    Dataset* m_dataset = nullptr;
    hid_t m_datatype;
};

class DatasetReader : public Reader
{
public:
    DatasetReader(hid_t dataset, hid_t datatype);
    virtual void read(void *buffer) override;
private:
    hid_t m_dataset;
    hid_t m_datatype;
};

class Dataset : public Object
{
public:
    Dataset();
    Dataset(hid_t id, hid_t parentID, std::string name);

    Dataset(const Object &other);
    Dataset(const Dataset &other);
    //    Dataset(Dataset &&other);

    ~Dataset();

    Dataset& operator=(const Object &other);
    Dataset& operator=(const Dataset &other);

    template<typename T>
    Dataset& operator=(const T &data);

    Datatype::Type datatype() const;

    // dataspace properties
    bool isScalar() const;
    bool isSimple() const;
    int dimensionCount() const;
    std::vector<hsize_t> extents() const;

    template<typename T>
    static Dataset create(hid_t parentID, const std::string &name, const T &data);

private:
    std::vector<hsize_t> extents(hid_t dataspace) const;
    Datatype datatype_impl() const;
    Dataspace dataspace() const;

    template<typename T>
    T valueImpl(Object::Requirement mode) const;

    friend class Object;
    friend class DatasetWriter;
};

#ifndef H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
template<>
inline Object::operator Dataset() const {
    return Dataset(*this);
}
#endif

template<typename T>
inline void Object::operator=(const T& matrix)
{
    Dataset dataset = *this;
    dataset = matrix;
}

template<typename T>
Dataset& Dataset::operator=(const T &object)
{
    TypeHelper<T> typeHelper;
    DVLOG(1) << "Dataset assignment operator of T type: " << typeid(T).name();
    DVLOG(1) << "Parent, name, id: " << m_parentID << " " << m_name << " " << m_id;
    bool alreadyExists = (m_id > 0 && m_parentID > 0);
    if(alreadyExists) {
        if(!isValid()) {
            throw(std::runtime_error("Assigning value to invalid dataset object"));
        }
        std::vector<hsize_t> targetExtents = typeHelper.extents(object);
        if(!equal(targetExtents.begin(), targetExtents.end(), extents().begin())) {
            DVLOG(1) << "WARNING: Writing over dataset of different shape. "
                     << "Limitations in HDF5 standard makes it impossible to free space taken "
                     << "up by the old dataset.";
            close();
            herr_t deleteError = H5Ldelete(m_parentID, m_name.c_str(), H5P_DEFAULT);
            if(deleteError < 0) {
                throw std::runtime_error("Could not delete old dataset");
            }
            *this = Dataset::create(m_parentID, m_name, object);
        } else {
            DatasetWriter writer(this, typeHelper.hdfType());
            typeHelper.writeToFile(object, writer);
        }
    } else {
        *this = Dataset::create(m_parentID, m_name, object);
    }
    return *this;
}

template<typename T>
Dataset Dataset::create(hid_t parentID, const std::string &name, const T &data)
{
    TypeHelper<T> typeHelper;
    DVLOG(1) << "Creating dataset on parent " << parentID << " with name " << name;
    std::vector<hsize_t> extents = typeHelper.extents(data);

    H5S_class_t dataspaceType = typeHelper.dataspaceType();
    Dataspace dataspace(H5Screate(dataspaceType));
    if(dataspaceType == H5S_SIMPLE) {
        H5Sset_extent_simple(dataspace, extents.size(), &extents[0], NULL);
    }
    hid_t creationParameters = H5Pcreate(H5P_DATASET_CREATE);
    hid_t datatype = typeHelper.hdfType();
    hid_t datasetID = H5Dcreate(parentID, name.c_str(), datatype, dataspace,
                              H5P_DEFAULT, creationParameters, H5P_DEFAULT);

    if(datasetID < 1) {
        throw std::runtime_error("Could not create dataset");
    }
    Dataset dataset(datasetID, parentID, name);
    DatasetWriter writer(&dataset, datatype);
    typeHelper.writeToFile(data, writer);
    DVLOG(1) << "Returning the created dataset " << dataset;
    return dataset;
}

template<typename T>
T Object::value(Requirement mode) const
{
    if(type() != Type::Dataset) {
        std::stringstream errorStream;
        errorStream << "Tried to convert non-dataset object to " << demangle(typeid(T).name());
        throw std::runtime_error(errorStream.str());
    }
    Dataset dataset = *this;
    return dataset.valueImpl<T>(mode);
}

// TODO Does this lead to a costly copy? If so, the implementation needs to be changed.
template<typename T>
inline T Dataset::valueImpl(Requirement mode) const
{
    TypeHelper<T> typeHelper;
    DVLOG(1) << "Reading dataset " << m_id << " " << m_name << " " << m_parentID << " to type " << demangle(typeid(T).name());
    if(!isValid()) {
        throw(std::runtime_error("Could not fetch value from invalid dataset object"));
    }
    std::vector<hsize_t> extent = extents();
    int datasetDimensionCount = extent.size();
    int targetDimensionCount = typeHelper.dimensionCount();
    if((mode == Object::Requirement::MatchingDimensionCount && datasetDimensionCount != targetDimensionCount)
            || (mode == Object::Requirement::GreaterThanOrEqualDimensionCount && datasetDimensionCount > targetDimensionCount)) {
        std::stringstream errorStream;
        errorStream << "Tried to copy dataspace with " << datasetDimensionCount
                    << " dimensions to type " << demangle(typeid(T).name());
        throw std::runtime_error(errorStream.str());
    }
    if(mode == Object::Requirement::GreaterThanOrEqualDimensionCount && datasetDimensionCount < targetDimensionCount) {
        std::vector<hsize_t> targetExtents(targetDimensionCount);
        int diff = targetDimensionCount - datasetDimensionCount;
        for(int i = 0; i < diff; i++) {
            targetExtents[i] = 1;
        }
        for(int i = diff; i < targetDimensionCount; i++) {
            targetExtents[i] = extent[i - diff];
        }
        extent = targetExtents;
    }
    hid_t datatype = typeHelper.hdfType();
    DatasetReader reader(m_id, datatype);
    return typeHelper.readFromFile(extent, reader);
}

}

#endif // DATASET_H
