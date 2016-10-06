#ifndef GROUP_H
#define GROUP_H

#include "utils/logging.h"
#include "attribute.h"
#include "dataset.h"
#include "object.h"

#include <string>
#include <hdf5.h>
#include <vector>
#include <iostream>

namespace elegant {
namespace hdf5 {

class Group : public Object
{
public:
    Group(ConversionFlags conversionFlags = ConversionFlags::NoFlags);
    Group(const Object &other);
    Group(const Group &other);
//    Group(Group &&other);

    ~Group();

    Group& operator=(const Object &other);
    Group& operator=(const Group &other);
//    Group& operator=(Group &&other);

    std::vector<std::string> keys() const;
    std::vector<Object> items() const;

    Object item(std::string key) const;
    Object operator[](std::string key) const;

    Group createGroup(std::string name);

    bool hasKey(std::string name) const;

private:
    Group(hid_t id, hid_t parentID, std::string name);
};

#ifndef ELEGANT_HDF5_NO_USER_DEFINED_CONVERSION_OPERATORS
template<>
inline Object::operator Group() const {
    return Group(*this);
}
#endif

}
}

#endif // GROUP_H
