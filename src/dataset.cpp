#include "dataset.h"

#include <iostream>

using namespace std;

namespace h5cpp {

Dataset::Dataset()
    : Object()
{
}

Dataset::Dataset(Dataset &&other)
    : Object(move(other))
{
}

Dataset::Dataset(hid_t id, hid_t parentID, string name)
    : Object(id, parentID, name)
{
}

Dataset::Dataset(const Object &other)
{
    constructFromOther(other);
}

Dataset::Dataset(const Dataset &other)
    : Object()
{
    constructFromOther(other);
}

Dataset& Dataset::operator=(const Object &other)
{
    constructFromOther(other);
    return *this;
}

Dataset& Dataset::operator=(const Dataset &other)
{
    constructFromOther(other);
    return *this;
}

Dataset& Dataset::operator=(Dataset &&other)
{
    Object &otherObject = other;
    Object::operator=(move(otherObject));
    return *this;
}

Dataset::~Dataset()
{
    close();
}

void Dataset::constructFromOther(const Object &other)
{
    close();
    if(other.id() > 0) {
        H5O_info_t info;
        herr_t err = H5Oget_info(other.id(), &info);
        if(err < 0) {
            cerr << "ERROR: Could not convert object to dataset. Could not get object info." << endl;
            return;
        }
        if(info.type != H5O_TYPE_DATASET) {
            cerr << "ERROR: Could not convert object to dataset. Object is not dataset. Type info: " << info.type << endl;
            return;
        }
        m_id = H5Dopen(other.id(), ".", H5P_DEFAULT);
#ifdef H5CPP_VERBOSE
        cerr << "Open dataset " << m_id << " from other " << other.id() << endl;
#endif
    } else {
        m_id = other.id();
#ifdef H5CPP_VERBOSE
        cerr << "Copy dataset from other " << m_id << endl;
#endif
    }
    m_parentID = other.parentID();
    m_name = other.name();
}

void Dataset::close()
{
    if(m_id > 0) {
        H5Dclose(m_id);
#ifdef H5CPP_VERBOSE
        cerr << "Close dataset " << m_id << endl;
#endif
        m_id = 0;
    }
}

}
