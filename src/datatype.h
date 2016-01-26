#ifndef DATATYPE_H
#define DATATYPE_H

#include <hdf5.h>

namespace elegant {
namespace hdf5 {


class Datatype
{
public:
    Datatype();
    Datatype(hid_t typeID);

    Datatype(const Datatype &other);
    Datatype& operator=(const Datatype &other);

    ~Datatype();

    enum class Type {
        Unknown = -1,
        Int,
        Long,
        Float,
        Double,
        String
    };

    bool isValid() const;

    Type type() const;

    void close();
private:
    hid_t id() const;
    void constructFromOther(const Datatype &other);
    hid_t m_id;
    operator hid_t() const;

    friend class Attribute;
    friend class Dataset;
    friend class Object;
};

} // namespace
}

#endif // DATATYPE_H
