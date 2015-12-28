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
    close();
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
        cerr << "ERROR: Cannot assign to Dataset with " << other << endl;
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
//        cerr << "Opened dataset " << m_id << " from other " << other.id() << endl;
//#endif
//        m_name = other.name();
//        m_parentID = other.parentID();
//    } else if(other.id() == 0) {
//        close();
//        m_id = other.id();
//        m_name = other.name();
//        m_parentID = other.parentID();
//    } else {
//        cerr << "ERROR: Cannot construct Dataset from " << other << endl;
//    }
//}

//void Dataset::close()
//{
//    if(m_id > 0) {
//#ifdef H5CPP_VERBOSE
//    cerr << "Closing dataset " << m_id << endl;
//#endif
//        H5Dclose(m_id);
//        m_id = 0;
//    }
//}

}
