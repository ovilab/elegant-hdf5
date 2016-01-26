#ifndef TEMP_H
#define TEMP_H

#include "logging.h"
#include "demangle.h"
//#include "dataset.h"

#include <hdf5.h>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <vector>

namespace elegant {
namespace hdf5 {

//class Object;
class Dataset;
class Group;
class File;
class Attribute;
}
}

//std::ostream& operator<< (std::ostream &out, const elegant::hdf5::Object &object);

namespace elegant {
namespace hdf5 {

class Object
{
public:
    Object();
    Object(hid_t id, hid_t parentID, std::string name);

    enum class CopyMode {
        OpenOnCopy,
        DontOpenOnCopy
    };

    Object(const Object &other, CopyMode mode = CopyMode::OpenOnCopy);
//    Object(Object &&other);
//    Object(Dataset &&other) = delete;
//    Object(Group &&other) = delete;
    Object& operator=(const Object &other);
    Object& operator=(const Dataset &other);
    Object& operator=(const Group &other);
//    Object& operator=(Object &&other);

    template<typename T>
    void operator=(const T& other); // TODO: Consider operator chaining support

    virtual ~Object();

    enum class Type {
        Invalid = -1,
        File,
        Group,
        Datatype,
        Dataspace,
        Dataset,
        Attribute
    };

    enum class Requirement {
        MatchingDimensionCount,
        GreaterThanOrEqualDimensionCount
    };

    const std::string &name() const;
    Type type() const;
    hid_t id() const;

#ifndef H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
    template<typename T>
    operator T() const;
#endif

    template<typename T>
    T value(Requirement mode = Requirement::MatchingDimensionCount) const;

    bool isValid() const;
    bool isDataset() const;
    bool isGroup() const;
    bool isNonExistingNamed() const;

    static Type fromHdf5Type(H5I_type_t hType);
    static H5I_type_t toHdf5Type(Object::Type hType);
    hid_t parentID() const;

    std::vector<Attribute> attributes() const;
    std::vector<std::string> attributeKeys() const;
    Attribute operator ()(std::string key) const;
    Attribute attribute(std::string key) const;
    bool hasAttribute(std::string name) const;

    void close();

    friend std::ostream& operator<<(std::ostream&, const Object&);

protected:
    void constructFromOther(const Object &other);

    hid_t m_id = 0;
    hid_t m_parentID = 0;
    std::string m_name;

private:
};

#ifndef H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
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

} // namespace h5cpp
}

#endif // TEMP_H
