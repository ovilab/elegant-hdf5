#include "datatype.h"

#include <stdexcept>

namespace elegant {
namespace hdf5 {


Datatype::Datatype()
{
}

Datatype::Datatype(hid_t typeID)
    : m_id(typeID)
{
    if(typeID < 1) {
        throw std::runtime_error("Invalid datatype");
    }
}

Datatype::Datatype(const Datatype &other)
{
    constructFromOther(other);
}

Datatype &Datatype::operator=(const Datatype &other)
{
    close();
    constructFromOther(other);
    return *this;
}

Datatype::~Datatype()
{
    close();
}

hid_t Datatype::id() const
{
    return m_id;
}

Datatype::operator hid_t() const
{
    return m_id;
}

bool Datatype::isValid() const
{
    if(m_id > 0) {
        return true;
    } else {
        return false;
    }
}

Datatype::Type Datatype::type() const
{
    if(H5Tequal(m_id, H5T_NATIVE_INT)) {
        return Type::Int;
    } else if(H5Tequal(m_id, H5T_NATIVE_LONG)) {
        return Type::Long;
    } else if(H5Tequal(m_id, H5T_NATIVE_FLOAT)) {
        return Type::Float;
    } else if(H5Tequal(m_id, H5T_NATIVE_DOUBLE)) {
        return Type::Double;
    } else if(H5Tget_class(m_id) == H5T_STRING) {
        return Type::String;
    } else {
        return Type::Unknown;
    }
}

void Datatype::close()
{
    if(m_id > 0) {
        H5Tclose(m_id);
    }
}

void Datatype::constructFromOther(const Datatype &other) {
    if(other.isValid()) {
        m_id = H5Tcopy(other.id());
    } else {
        m_id = other.id();
    }
}

} // namespace
}
