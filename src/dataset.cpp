#include "dataset.h"

#include <iostream>

using namespace std;

namespace h5cpp {

Dataset::Dataset()
    : Object()
{
}

//Dataset::Dataset(Dataset &&other)
//    : Object(move(other))
//{
//}

Dataset::~Dataset()
{
    closeObject();
}

Dataset::Dataset(hid_t id, hid_t parentID, string name)
    : Object(id, parentID, name)
{
}

Dataset::Dataset(const Object &other)
    : Object()
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
    if(!other.isDataset()) {
        DLOG(INFO) << "ERROR: Cannot assign to Dataset with " << other;
        return *this;
    }
    Object::operator=(other);
    return *this;
}

Dataset& Dataset::operator=(const Dataset &other)
{
    Object::operator=(other);
    return *this;
}

//Dataset& Dataset::operator=(Object &&other)
//{
//    Object &otherObject = other;
//    Object::operator=(move(otherObject));
//    return *this;
//}

//Dataset& Dataset::operator=(Dataset &&other)
//{
//    Object &otherObject = other;
//    Object::operator=(move(otherObject));
//    return *this;
//}

//void Dataset::constructFromOther(const Object &other)
//{
//    if(other.isValid() && other.isDataset()) {
//        close();
//        m_id = H5Dopen(other.id(), ".", H5P_DEFAULT);
//#ifdef H5CPP_VERBOSE
//        DLOG(INFO) << "Opened dataset " << m_id << " from other " << other.id();
//#endif
//        m_name = other.name();
//        m_parentID = other.parentID();
//    } else if(other.id() == 0) {
//        close();
//        m_id = other.id();
//        m_name = other.name();
//        m_parentID = other.parentID();
//    } else {
//        DLOG(INFO) << "ERROR: Cannot construct Dataset from " << other;
//    }
//}

//void Dataset::close()
//{
//    if(m_id > 0) {
//#ifdef H5CPP_VERBOSE
//    DLOG(INFO) << "Closing dataset " << m_id;
//#endif
//        H5Dclose(m_id);
//        m_id = 0;
//    }
//}

}
