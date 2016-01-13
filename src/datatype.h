#ifndef DATATYPE_H
#define DATATYPE_H

#include <hdf5.h>

class Datatype
{
public:
    Datatype(hid_t typeID);
    ~Datatype();

private:
    hid_t m_id = 0;
};

#endif // DATATYPE_H
