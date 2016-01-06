#ifndef H5CPP_ATTRIBUTE_H
#define H5CPP_ATTRIBUTE_H

#include "typehelper.h"
#include "logging.h"
#include "demangle.h"

#include <hdf5.h>
#include <ostream>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>

namespace h5cpp {

class Attribute
{
public:
    Attribute();
    Attribute(hid_t parentID, const std::string &name);
    Attribute(hid_t id, hid_t parentID, const std::string &name);
    Attribute(const Attribute &other);
//    Attribute(Attribute &&other);
//    Attribute(Attribute &&other) = default;
    Attribute& operator=(const Attribute &other);
    Attribute& operator=(const std::string &value);
//    Attribute& operator=(Attribute &&other) = default;
    ~Attribute();

    template<typename T>
    void operator=(const T &other);

    bool isValid() const;
    bool isNonExistingNamed() const;
    hid_t id() const;
    hid_t parentID() const;
    std::string name() const;
    void close();

    std::vector<hsize_t> extents() const;
    std::vector<hsize_t> extents(hid_t dataspace) const;

    template<typename T>
    operator T() const;

    operator std::string() const;

private:
    hid_t m_id = 0;
    hid_t m_parentID = 0;
    std::string m_name;
    void constructFromOther(const Attribute &other);
};

template<typename T>
Attribute::operator T() const
{
    DVLOG(1) << "Reading attribute " << m_id << " " << m_name << " " << m_parentID;
    if(m_id == 0) {
        throw std::runtime_error("Attribute does not exist");
    }
    std::vector<hsize_t> extent = extents();
    int dimensionCount = extent.size();
    if(dimensionCount != TypeHelper<T>::dimensionCount()) {
        std::stringstream errorStream;
        errorStream << "Tried to copy dataspace with " << dimensionCount
                    << " dimensions to type " << demangle(typeid(T).name());
        throw std::runtime_error(errorStream.str());
    }
    T value = TypeHelper<T>::objectFromExtents(extent);
    hid_t datatype = TypeHelper<T>::hdfType();
    herr_t readError = H5Aread(m_id, datatype, TypeHelper<T>::writableBuffer(value));
    if(readError < 0) {
        throw std::runtime_error("Could not read attribute");
    }
    return value;
}

template<typename T>
void Attribute::operator=(const T &other)
{
    hid_t datatype = TypeHelper<T>::hdfType();
    std::vector<hsize_t> extents = TypeHelper<T>::extentsFromType(other);
    if(m_id != 0) {
        close();
        H5Adelete(m_parentID, m_name.c_str());
    }
    H5S_class_t dataspaceType = TypeHelper<T>::dataspaceType();
    hid_t dataspace = H5Screate(dataspaceType);
    if(dataspaceType == H5S_SIMPLE) {
        H5Sset_extent_simple(dataspace, extents.size(), &extents[0], NULL);
    }
    m_id = H5Acreate(m_parentID, m_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
    if(m_id < 1) {
        throw std::runtime_error("Could not create attribute");
    }
    TypeHelper<T> temporary;
    herr_t writeError = H5Awrite(m_id, datatype, temporary.readableBuffer(other));
    if(writeError < 0) {
        throw std::runtime_error("Could not write attribute");
    }
    H5Sclose(dataspace);
    DVLOG(1) << "Wrote to attribute " << m_id << " " << m_name << " " << m_parentID;
}

}

inline std::ostream& operator<< (std::ostream &out, const h5cpp::Attribute &attribute)
{
    std::string typeName = "Unknown";
//    h5cpp::Object::Type type = object.type();
//    switch(type) {
//    case h5cpp::Object::Type::File:
//        typeName = "File";
//        break;
//    case h5cpp::Object::Type::Group:
//        typeName = "Group";
//        break;
//    case h5cpp::Object::Type::Datatype:
//        typeName = "Datatype";
//        break;
//    case h5cpp::Object::Type::Dataspace:
//        typeName = "Dataspace";
//        break;
//    case h5cpp::Object::Type::Dataset:
//        typeName = "Dataset";
//        break;
//    case h5cpp::Object::Type::Attribute:
//        typeName = "Attribute";
//        break;
//    default:
//        break;
//    }
    out << "Attribute(type=" << typeName
        << ", id=" << attribute.id()
        << ", name=\"" << attribute.name()
        << "\", parent=\"" << attribute.parentID() << "\")";
    return out;
}

#endif // H5CPP_ATTRIBUTE_H
