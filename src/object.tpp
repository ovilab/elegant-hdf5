#ifndef OBJECT_TPP
#define OBJECT_TPP

#include "object.h"

namespace elegant {
namespace hdf5 {

#ifndef ELEGANT_HDF5_NO_USER_DEFINED_CONVERSION_OPERATORS
template<typename T>
Object::operator T() const
{
    return value<T>();
}
#endif

template<typename T>
inline T& operator<<(T &other, const elegant::hdf5::Object &object)
{
    other = object.value<T>();
    return other;
}

template<typename T>
inline T& operator>>(const elegant::hdf5::Object &object, T &other)
{
    other = object.value<T>();
    return other;
}

inline std::ostream& operator<< (std::ostream &out, const elegant::hdf5::Object &object)
{
    std::string typeName = "Unknown";
    elegant::hdf5::Object::Type type = object.type();
    switch(type) {
    case elegant::hdf5::Object::Type::File:
        typeName = "File";
        break;
    case elegant::hdf5::Object::Type::Group:
        typeName = "Group";
        break;
    case elegant::hdf5::Object::Type::Datatype:
        typeName = "Datatype";
        break;
    case elegant::hdf5::Object::Type::Dataspace:
        typeName = "Dataspace";
        break;
    case elegant::hdf5::Object::Type::Dataset:
        typeName = "Dataset";
        break;
    case elegant::hdf5::Object::Type::Attribute:
        typeName = "Attribute";
        break;
    default:
        break;
    }
    out << "Object(type=" << typeName << ", id=" << object.id() << ", name=\"" << object.name() << "\")";
    return out;
}

}
}

#endif // OBJECT_TPP
