#ifndef TEMP_H
#define TEMP_H

#include <hdf5.h>
#include <string>
#include <armadillo>
#include <iostream>

//#include "dataset.h"

namespace h5cpp {
//class Object;
class Dataset;
}

//std::ostream& operator<< (std::ostream &out, const h5cpp::Object &object);

namespace h5cpp {
class Object
{
public:
    Object();
    Object(hid_t id, hid_t parentID, std::string name);

    template<typename T>
    void operator=(const arma::Mat<T>& matrix); // TODO: Consider operator chaining support

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
            std::cerr << "ERROR: Tried to convert non-dataset object to arma::mat. " << std::endl;
//                 << "Returned matrix will be empty. INFO: " << *this << std::endl;
            return arma::Mat<T>();
        }
        Dataset dataset = *this;
        return dataset;
    }

    virtual bool isValid() const;

    static Type fromHdf5Type(H5I_type_t hType);
    static H5I_type_t toHdf5Type(Object::Type hType);
    hid_t parentID() const;

protected:
    hid_t m_id = 0;
    hid_t m_parentID = 0;
    std::string m_name;
};

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
