#ifndef TEMP_H
#define TEMP_H

#include <hdf5.h>
#include <string>
#include <armadillo>
#include <iostream>

namespace hdf5 {
class Object;
class Dataset;
}

std::ostream& operator<< (std::ostream &out, const hdf5::Object &object);

namespace hdf5 {
class Object
{
public:
    Object();
    Object(hid_t id, std::string name);
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

    template<typename T>
    operator arma::Mat<T>() const
    {
        if(type() != Type::Dataset) {
            std::cerr << "ERROR: Tried to convert non-dataset object to arma::mat. "
                 << "Returned matrix will be empty. INFO: " << *this << std::endl;
            return arma::Mat<T>();
        }
        Dataset dataset = *this;
        return dataset;
    }

    virtual bool isValid() const;

    static Type fromHdf5Type(H5I_type_t hType);
    static H5I_type_t toHdf5Type(Object::Type hType);
protected:
    hid_t m_id = 0;
    std::string m_name;
};

}

inline std::ostream& operator<< (std::ostream &out, const hdf5::Object &object)
{
    std::string typeName = "Unknown";
    hdf5::Object::Type type = object.type();
    switch(type) {
    case hdf5::Object::Type::File:
        typeName = "File";
        break;
    case hdf5::Object::Type::Group:
        typeName = "Group";
        break;
    case hdf5::Object::Type::Datatype:
        typeName = "Datatype";
        break;
    case hdf5::Object::Type::Dataspace:
        typeName = "Dataspace";
        break;
    case hdf5::Object::Type::Dataset:
        typeName = "Dataset";
        break;
    case hdf5::Object::Type::Attribute:
        typeName = "Attribute";
        break;
    default:
        break;
    }
    out << "Object(type=" << typeName << ", id=" << object.id() << ", name=\"" << object.name() << "\")";
    return out;
}

#endif // TEMP_H
