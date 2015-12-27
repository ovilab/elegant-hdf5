#include "dataset.h"

#include <iostream>

using namespace std;

namespace h5cpp {

Dataset::Dataset()
    : Object()
{

}

Dataset::Dataset(hid_t id, hid_t parentID, string name)
    : Object(id, parentID, name)
{
}

Dataset::Dataset(const h5cpp::Object &other)
    : Object(other, CopyMode::DontOpenOnCopy)
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

Dataset::~Dataset()
{
    if(m_id != 0) {
        H5Dclose(m_id);
#ifdef H5CPP_VERBOSE
        cerr << "Close dataset " << m_id << endl;
#endif
        m_id = 0;
    }
}

void Dataset::constructFromOther(const Object &other)
{
    if(other.id() > 0) {
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

}
