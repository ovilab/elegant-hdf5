#ifndef H5CPP_ATTRIBUTE_H
#define H5CPP_ATTRIBUTE_H

#include "typehelper.h"
#include "logging.h"
#include "demangle.h"
#include "datatype.h"
#include "dataspace.h"

#include <hdf5.h>
#include <ostream>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>

namespace elegant {
namespace hdf5 {


class AttributeReader : public Reader
{
public:
    AttributeReader(hid_t id, hid_t datatype);
    virtual void read(void *buffer);
private:
    hid_t m_id;
    hid_t m_datatype;
};

class AttributeWriter : public Writer
{
public:
    AttributeWriter(hid_t id, hid_t datatype);
    virtual void write(const void *buffer);
private:
    hid_t m_id;
    hid_t m_datatype;
};

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
    Attribute& operator=(const char* object);
//    Attribute& operator=(Attribute &&other) = default;
    ~Attribute();

    template<typename T>
    void operator=(const T &other);


    bool isValid() const;
    bool isNonExistingNamed() const;

    std::string name() const;
    void close();

    Datatype::Type datatype() const;

    // dataspace properties
    bool isScalar() const;
    bool isSimple() const;
    int dimensionCount() const;
    std::vector<hsize_t> extents() const;

    template<typename T>
    T value() const;

    std::string toString() const;

#ifndef H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
    template<typename T>
    operator T() const;

    operator std::string() const;
#endif

    friend std::ostream& operator<<(std::ostream&, const Attribute&);

private:
    hid_t id() const;
    hid_t parentID() const;
    void constructFromOther(const Attribute &other);
    Datatype datatype_impl() const;
    Dataspace dataspace() const;

    hid_t m_id = 0;
    hid_t m_parentID = 0;
    std::string m_name;
};

#ifndef H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
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

inline std::ostream& operator<< (std::ostream &out, const elegant::hdf5::Attribute &attribute)
{
    std::string typeName = "Unknown";
    out << "Attribute(type=" << typeName
        << ", id=" << attribute.id()
        << ", name=\"" << attribute.name()
        << "\", parent=\"" << attribute.parentID() << "\")";
    return out;
}

} // namespace h5cpp

}

#endif // H5CPP_ATTRIBUTE_H
