#include "object.h"

#include "dataset.h"

using namespace std;

namespace h5cpp {
Object::Object()
{
}

Object::Object(hid_t id, hid_t parentID, string name)
    : m_id(id)
    , m_parentID(parentID)
    , m_name(name)
{
}

Object::~Object()
{
    if(m_id != 0) {
        H5Oclose(m_id);
        m_id = 0;
    }
}

const std::string& Object::name() const
{
    return m_name;
}

Object::Type Object::type() const
{
    H5I_type_t hType = H5Iget_type(m_id);
    return Object::fromHdf5Type(hType);
}

hid_t Object::id() const
{
    return m_id;
}

bool Object::isValid() const
{
    if(m_id != 0) {
        return true;
    }
    return false;
}

Object::Type Object::fromHdf5Type(H5I_type_t hType) {
    Type returnedType = Type::Invalid;
    switch(hType) {
    case H5I_FILE:
        returnedType = Type::File;
        break;
    case H5I_GROUP:
        returnedType = Type::Group;
        break;
    case H5I_DATATYPE:
        returnedType = Type::Datatype;
        break;
    case H5I_DATASPACE:
        returnedType = Type::Dataspace;
        break;
    case H5I_DATASET:
        returnedType = Type::Dataset;
        break;
    case H5I_ATTR:
        returnedType = Type::Attribute;
        break;
    default:
        break;
    }
    return returnedType;
}

H5I_type_t Object::toHdf5Type(Object::Type hType) {
    H5I_type_t returnedType = H5I_BADID;
    switch(hType) {
    case Type::File:
        returnedType = H5I_FILE;
        break;
    case Type::Group:
        returnedType = H5I_GROUP;
        break;
    case Type::Datatype:
        returnedType = H5I_DATATYPE;
        break;
    case Type::Dataspace:
        returnedType = H5I_DATASPACE;
        break;
    case Type::Dataset:
        returnedType = H5I_DATASET;
        break;
    case Type::Attribute:
        returnedType = H5I_ATTR;
        break;
    default:
        break;
    }
    return returnedType;
}

hid_t Object::parentID() const
{
    return m_parentID;
}

}
