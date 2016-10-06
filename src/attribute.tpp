#ifndef ATTRIBUTE_TPP
#define ATTRIBUTE_TPP

#include "attribute.h"

namespace elegant {
namespace hdf5 {

#ifndef ELEGANT_HDF5_NO_USER_DEFINED_CONVERSION_OPERATORS
template<typename T>
Attribute::operator T() const
{
    return value<T>();
}

inline Attribute::operator std::string() const
{
    return toString();
}
#endif

template<>
inline std::string Attribute::value<std::string>() const
{
    return toString();
}

template<typename T>
T Attribute::value() const
{
    TypeHelper<T> typeHelper;
    DVLOG(1) << "Reading attribute " << m_id << " " << m_name << " " << m_parentID;
    if(m_id == 0) {
        throw std::runtime_error("Attribute does not exist");
    }
    std::vector<hsize_t> sourceExtents = dataspace().extents();
    int dimensionCount = sourceExtents.size();
    if(dimensionCount != typeHelper.dimensionCount()) {
        std::stringstream errorStream;
        errorStream << "Tried to copy dataspace with " << dimensionCount
                    << " dimensions to type " << demangle(typeid(T).name());
        throw std::runtime_error(errorStream.str());
    }
    hid_t datatype = typeHelper.hdfType();
    AttributeReader reader(m_id, datatype);
    return typeHelper.readFromFile(sourceExtents, reader);
}

template<typename T>
void Attribute::operator=(const T &object)
{
    TypeHelper<T> typeHelper;
    hid_t datatype = typeHelper.hdfType();
    std::vector<hsize_t> extents = typeHelper.extents(object);
    if(m_id != 0) {
        close();
        H5Adelete(m_parentID, m_name.c_str());
    }
    H5S_class_t dataspaceType = typeHelper.dataspaceType();
    Dataspace dataspace(H5Screate(dataspaceType));
    if(dataspaceType == H5S_SIMPLE) {
        H5Sset_extent_simple(dataspace, extents.size(), &extents[0], NULL);
    }
    m_id = H5Acreate(m_parentID, m_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
    if(m_id < 1) {
        throw std::runtime_error("Could not create attribute");
    }
    AttributeWriter writer(m_id, datatype);
    typeHelper.writeToFile(object, writer);
    DVLOG(1) << "Wrote to attribute " << m_id << " " << m_name << " " << m_parentID;
}

template<typename T>
inline T& operator<<(T &other, const elegant::hdf5::Attribute &attribute)
{
    other = attribute.value<T>();
    return other;
}

template<typename T>
inline T& operator>>(const elegant::hdf5::Attribute &attribute, T &other)
{
    other = attribute.value<T>();
    return other;
}

inline std::ostream& operator<< (std::ostream &out, const elegant::hdf5::Attribute &attribute)
{
    std::string typeName = "Unknown";
    out << "Attribute(type=" << typeName
        << ", id=" << attribute.id()
        << ", name=\"" << attribute.name()
        << "\", parent=\"" << attribute.parentID() << "\")";
    return out;
}

} // namespace
} // namespace

#endif // ATTRIBUTE_TPP
