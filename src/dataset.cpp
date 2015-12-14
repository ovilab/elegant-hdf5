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
    : Object(other)
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
        m_id = 0;
    }
}

void Dataset::constructFromOther(const Object &other)
{
    if(other.id() > 0) {
        m_id = H5Dopen(other.id(), ".", H5P_DEFAULT);
    } else {
        cerr << "ERROR: Dataset with name " << other.name() << " doesn't exist." << endl;
        m_id = other.id();
    }
    m_parentID = other.parentID();
    m_name = other.name();
}

}
