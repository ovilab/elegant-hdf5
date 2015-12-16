#ifndef H5CPP_ATTRIBUTE_H
#define H5CPP_ATTRIBUTE_H

#include "utils.h"

#include <hdf5.h>
#include <ostream>
#include <string>
#include <iostream>

namespace h5cpp {

class Attribute
{
public:
    Attribute(hid_t parentID, const std::string &name);
    Attribute(hid_t id, hid_t parentID, const std::string &name);
    ~Attribute();

    template<typename T>
    void operator=(const T &other);

    hid_t id() const;
    hid_t parentID() const;
    std::string name() const;

    template<typename T>
    operator T() const;

    operator std::string() const;

private:
    hid_t m_id = 0;
    hid_t m_parentID = 0;
    std::string m_name;
};

template<typename T>
Attribute::operator T() const
{
    T value{};
    if(m_id == 0) {
        std::cerr << "ERROR: Attempted use of undefined attribute '" << m_name << "'." << std::endl;
        return value;
    }
    hid_t datatype = datatypeFromType<T>()();
    H5Aread(m_id, datatype, &value);
    return value;
}


template<typename T>
void Attribute::operator=(const T &other)
{
    hid_t datatype = datatypeFromType<T>()();
    hsize_t dims[1];
    dims[0] = 1;
    if(m_id != 0) {
        H5Aclose(m_id);
        H5Adelete(m_parentID, m_name.c_str());
    }
    hid_t dataspace = H5Screate_simple(1, &dims[0], NULL);
    m_id = H5Acreate(m_parentID, m_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);
    H5Awrite(m_id, datatype, &other);
    H5Sclose(dataspace);
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
    out << "Attribute(type=" << typeName << ", id=" << attribute.id() << ", name=\"" << attribute.name() << "\")";
    return out;
}

#endif // H5CPP_ATTRIBUTE_H
