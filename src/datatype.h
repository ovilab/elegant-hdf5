#ifndef DATATYPE_H
#define DATATYPE_H

#include <hdf5.h>

class Datatype
{
public:
    Datatype();
    Datatype(hid_t typeID);

    Datatype(const Datatype &other);
    Datatype& operator=(const Datatype &other);

    ~Datatype();

    bool isValid() const;
    bool isInt() const;
    bool isFloat() const;
    bool isDouble() const;

    void close();

    hid_t id() const;

    operator hid_t() const;
private:
    hid_t m_id;
    void constructFromOther(const Datatype &other);
};

#endif // DATATYPE_H
