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

namespace h5cpp {
//class Object;
class Dataset;
class Group;
class File;
class Attribute;
}

//std::ostream& operator<< (std::ostream &out, const h5cpp::Object &object);

namespace h5cpp {
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

    const std::string &name() const;
    Type type() const;
    hid_t id() const;

#ifndef H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
    template<typename T>
    operator T() const;
#endif

    template<typename T>
    T value() const;

    bool isValid() const;
    bool isDataset() const;
    bool isGroup() const;
    bool isNonExistingNamed() const;

    static Type fromHdf5Type(H5I_type_t hType);
    static H5I_type_t toHdf5Type(Object::Type hType);
    hid_t parentID() const;

    void close();

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

}

inline std::ostream& operator<< (std::ostream &out, const h5cpp::Object &object)
{
    std::string typeName = "Unknown";
    h5cpp::Object::Type type = object.type();
    switch(type) {
    case h5cpp::Object::Type::File:
        typeName = "File";
        break;
    case h5cpp::Object::Type::Group:
        typeName = "Group";
        break;
    case h5cpp::Object::Type::Datatype:
        typeName = "Datatype";
        break;
    case h5cpp::Object::Type::Dataspace:
        typeName = "Dataspace";
        break;
    case h5cpp::Object::Type::Dataset:
        typeName = "Dataset";
        break;
    case h5cpp::Object::Type::Attribute:
        typeName = "Attribute";
        break;
    default:
        break;
    }
    out << "Object(type=" << typeName << ", id=" << object.id() << ", name=\"" << object.name() << "\")";
    return out;
}

#endif // TEMP_H
