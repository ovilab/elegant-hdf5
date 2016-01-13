#include "attribute.h"

using namespace std;

namespace h5cpp {

Attribute::Attribute()
{
}

Attribute::Attribute(hid_t parentID, const std::string &name)
    : m_parentID(parentID)
    , m_name(name)
{
    m_id = H5Aopen(parentID, name.c_str(), H5P_DEFAULT);

    DVLOG(1) << "Construct attribute by parent and name " << *this;
}

Attribute::Attribute(hid_t id, hid_t parentID, const std::string &name)
    : m_id(id)
    , m_parentID(parentID)
    , m_name(name)
{
    DVLOG(1) << "Creating attribute " << *this;
}

//Attribute::Attribute(Attribute &&other)
//    : m_id(move(other.m_id))
//    , m_parentID(move(other.m_parentID))
//    , m_name(move(other.m_name))
//{
//    other.m_id = 0;
//}

Attribute::Attribute(const Attribute &other)
{
    DVLOG(1) << "Copy construct attribute";
    constructFromOther(other);
}

Attribute &Attribute::operator=(const Attribute &other)
{
    bool copyFromExistingToExisting = isValid() && other.isValid();
    bool copyFromExistingToNonExisting = isNonExistingNamed() && other.isValid();

    bool isSame = (m_name == other.name() && m_parentID == other.parentID());
    if(isSame) {
        DVLOG(1) << "Is the same attribute";
        return *this;
    } else if(copyFromExistingToExisting || copyFromExistingToNonExisting) {
        DVLOG(1) << "Copying attribute " << other << " to " << *this;
        close();
        if(copyFromExistingToExisting) {
            H5Adelete(m_parentID, m_name.c_str());
        }
        Dataspace otherDataspace = other.dataspace();
        Datatype datatype = other.datatype();
        Dataspace ourDataspace(H5Scopy(otherDataspace));

        m_id = H5Acreate(m_parentID, m_name.c_str(), datatype, ourDataspace, H5P_DEFAULT, H5P_DEFAULT);

        hsize_t elementCount = H5Sget_simple_extent_npoints(otherDataspace);
        hsize_t typeSize = H5Tget_size(datatype);
        hsize_t size = typeSize * elementCount;

        htri_t variableLength = H5Tis_variable_str(datatype);
        if(variableLength) {
            char* stringArray;
            H5Aread(m_id, datatype, &stringArray);
            H5Awrite(m_id, datatype, &stringArray);
            H5free_memory(stringArray);
        } else {
            vector<char> data;
            data.resize(size);
            H5Aread(other.id(), datatype, &data[0]);
            H5Awrite(m_id, datatype, &data[0]);
        }
        return *this;
    }
    DVLOG(1) << "Constructing from other";
    constructFromOther(other);
    return *this;
}

void Attribute::constructFromOther(const Attribute &other) {
    m_name = other.name();
    m_parentID = other.parentID();
    if(other.isValid()) {
        m_id = H5Aopen(other.parentID(), other.name().c_str(), H5P_DEFAULT);
        DVLOG(1) << "Opening " << other << " to become " << *this;
    } else {
        DVLOG(1) << "Copying other attribute " << other;
        m_id = other.id();
    }
}

Dataspace Attribute::dataspace() const
{
    return Dataspace(H5Aget_space(m_id));
}

Attribute::~Attribute()
{
    close();
}

void Attribute::close()
{
    if(m_id != 0) {
        DVLOG(1) << "Close attribute " << m_id;
        H5Aclose(m_id);
        m_id = 0;
    }
}

bool Attribute::isValid() const
{
    DVLOG(1) << "Valid attribute: " << m_id << m_name << m_parentID;
    return (m_id > 0 && !m_name.empty() && m_parentID > 0);
}

bool Attribute::isNonExistingNamed() const
{
    return (!isValid() && !m_name.empty() && m_parentID > 0);
}

hid_t Attribute::id() const
{
    return m_id;
}

hid_t Attribute::parentID() const
{
    return m_parentID;
}

Datatype Attribute::datatype() const
{
    return Datatype(H5Aget_type(m_id));
}

std::string Attribute::name() const
{
    return m_name;
}

vector<hsize_t> Attribute::extents() const
{
    return dataspace().extents();
}

std::string Attribute::toString() const
{
    if(m_id == 0) {
        return std::string();
    }
    Datatype attributeType = datatype();
    hid_t typeClass = H5Tget_class(attributeType);
    if (typeClass != H5T_STRING) {
        DVLOG(1) << "ERROR: Trying to output non-string type to string. This is not yet supported.";
        return std::string();
    }

    H5A_info_t attributeInfo;
    herr_t infoError = H5Aget_info(m_id, &attributeInfo);
    if(infoError < 0) {
        DVLOG(1) << "ERROR: Could not fetch attribute info for " << m_name << ".";
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

Attribute &Attribute::operator=(const string &value)
{
    if(m_id != 0) {
        close();
        H5Adelete(m_parentID, m_name.c_str());
    }

    hid_t dataspace  = H5Screate(H5S_SCALAR);
    hid_t datatype = H5Tcopy(H5T_C_S1);
    H5Tset_size(datatype, value.size());
    H5Tset_strpad(datatype, H5T_STR_NULLTERM);

    m_id = H5Acreate(m_parentID, m_name.c_str(), datatype, dataspace, H5P_DEFAULT, H5P_DEFAULT);

    H5Awrite(m_id, datatype, value.c_str());
    H5Sclose(dataspace);
    return *this;
}

}
