#include "attribute.h"

using namespace std;

namespace h5cpp {

Attribute::Attribute(hid_t parentID, const std::string &name)
    : m_parentID(parentID)
    , m_name(name)
{
    m_id = H5Aopen(parentID, name.c_str(), H5P_DEFAULT);
#ifdef H5CPP_VERBOSE
    cerr << "Close attribute " << m_id << endl;
#endif
}

Attribute::Attribute(hid_t id, hid_t parentID, const std::string &name)
    : m_id(id)
    , m_parentID(parentID)
    , m_name(name)
{
}

Attribute::~Attribute()
{
    if(m_id != 0) {
#ifdef H5CPP_VERBOSE
        cerr << "Close attribute " << m_id << endl;
#endif
        H5Aclose(m_id);
        m_id = 0;
    }
}

hid_t Attribute::id() const
{
    return m_id;
}

hid_t Attribute::parentID() const
{
    return m_parentID;
}

std::string Attribute::name() const
{
    return m_name;
}

h5cpp::Attribute::operator std::string() const
{
    if(m_id == 0) {
        return std::string();
    }
    hid_t attributeType = H5Aget_type(m_id);
    hid_t typeClass = H5Tget_class(attributeType);
    if (typeClass != H5T_STRING) {
        std::cerr << "ERROR: Trying to output non-string type to string. This is not yet supported." << std::endl;
        return std::string();
    }

    H5A_info_t attributeInfo;
    herr_t infoError = H5Aget_info(m_id, &attributeInfo);
    if(infoError < 0) {
        std::cerr << "ERROR: Could not fetch attribute info for " << m_name << "." << std::endl;
        return std::string();
    }

    htri_t variableLength = H5Tis_variable_str(attributeType);

    hid_t attributeTypeNative = H5Tget_native_type(attributeType, H5T_DIR_ASCEND);
    char *stringArray;
    if(variableLength) {
        H5Aread(m_id, attributeTypeNative, &stringArray);
    } else {
        stringArray = new char[attributeInfo.data_size + 1];
        H5Aread(m_id, attributeTypeNative, stringArray);
        stringArray[attributeInfo.data_size] = '\0';
    }
    std::string value = stringArray;
    if(!variableLength) {
        delete[] stringArray;
    } else {
        H5free_memory(stringArray);
    }
    return value;
}

}
