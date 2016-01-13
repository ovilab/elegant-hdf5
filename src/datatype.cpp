#include "datatype.h"

Datatype::Datatype(hid_t typeID)
    : m_id(typeID)
{

}

Datatype::~Datatype()
{
    if(m_id > 0) {
        H5Tclose(m_id);
    }
}

