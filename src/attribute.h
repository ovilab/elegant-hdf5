#ifndef H5CPP_ATTRIBUTE_H
#define H5CPP_ATTRIBUTE_H

#include "io/typehelper.h"
#include "utils/logging.h"
#include "utils/demangle.h"
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

#ifndef ELEGANT_HDF5_NO_USER_DEFINED_CONVERSION_OPERATORS
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

} // namespace
} // namespace

#include "attribute.tpp"

#endif // H5CPP_ATTRIBUTE_H
