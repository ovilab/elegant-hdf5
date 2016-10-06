#ifndef DATASET_TPP
#define DATASET_TPP

#include "dataset.h"
#include "dataset_p.h"

namespace elegant {
namespace hdf5 {

#ifndef ELEGANT_HDF5_NO_USER_DEFINED_CONVERSION_OPERATORS
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
    DVLOG(1) << "Parent, name, id: " << m_parentId << " " << m_name << " " << m_id;
    bool alreadyExists = (m_id > 0 && m_parentId > 0);
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
            herr_t deleteError = H5Ldelete(m_parentId, m_name.c_str(), H5P_DEFAULT);
            if(deleteError < 0) {
                throw std::runtime_error("Could not delete old dataset");
            }
            *this = Dataset::create(m_parentId, m_name, object);
        } else {
            DatasetWriter writer(this, typeHelper.hdfType());
            typeHelper.writeToFile(object, writer);
        }
    } else {
        *this = Dataset::create(m_parentId, m_name, object);
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
T Object::value(ConversionFlags mode) const
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
inline T Dataset::valueImpl(ConversionFlags mode) const
{
    std::cout << "Is mode " << mode << std::endl;
    if(mode == ConversionFlags::InheritedFlags) {
        mode = m_inheritedConversionFlags;
    }
    std::cout << "Is inhe " << mode << std::endl;

    TypeHelper<T> typeHelper;

    hid_t targetDatatype = typeHelper.hdfType();
    bool requireET = (mode & Object::ConversionFlags::EqualTypes);

    if(requireET && !H5Tequal(typeHelper.hdfType(), H5Dget_type(m_id))) {
        std::stringstream errorStream;
        errorStream << "Could not get " << m_name << " because it does not match the requested type";
        throw std::runtime_error(errorStream.str());
    }
    DVLOG(1) << "Reading dataset " << m_id << " " << m_name << " " << m_parentId << " to type " << demangle(typeid(T).name());
    if(!isValid()) {
        throw(std::runtime_error("Could not fetch value from invalid dataset object"));
    }
    std::vector<hsize_t> extent = extents();
    int datasetDimensionCount = extent.size();
    int targetDimensionCount = typeHelper.dimensionCount();
    bool requireEDC = (mode & Object::ConversionFlags::EqualDimensionCount);
    bool requireGTEDC = (mode & Object::ConversionFlags::GreaterThanOrEqualDimensionCount);
    if((requireEDC && datasetDimensionCount != targetDimensionCount)
            || (requireGTEDC && datasetDimensionCount > targetDimensionCount)) {
        std::stringstream errorStream;
        errorStream << "Tried to copy dataspace with " << datasetDimensionCount
                    << " dimensions to type " << demangle(typeid(T).name());
        throw std::runtime_error(errorStream.str());
    }
    if(requireGTEDC && datasetDimensionCount < targetDimensionCount) {
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
    DatasetReader reader(m_id, targetDatatype);
    return typeHelper.readFromFile(extent, reader);
}

} // namespace
} // namespace

#endif // DATASET_TPP
