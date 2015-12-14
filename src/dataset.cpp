#include "dataset.h"

#include <iostream>

using namespace std;

namespace hdf5 {

Dataset::Dataset()
    : Object()
{

}

Dataset::Dataset(hid_t id, string name)
    : Object(id, name)
{
}

Dataset::Dataset(const hdf5::Object &other)
    : Object(other)
{
    constructFromOther(other);
}

void Dataset::operator=(const Object &other)
{
    constructFromOther(other);
}

void Dataset::operator=(const Dataset &other)
{
    constructFromOther(other);
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
    m_id = H5Dopen(other.id(), ".", H5P_DEFAULT);
}

}
