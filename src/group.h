#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <hdf5.h>
#include <vector>
#include <iostream>

#include "dataset.h"
#include "object.h"

namespace h5cpp {

class Group : public Object
{
public:
    Group();
    Group(const Object &other);
    void operator=(const Object &other);
    void operator=(const Group &other);

    virtual ~Group();

    std::vector<std::string> keys() const;
    Object operator[](std::string key) const;

    template<typename T>
    Dataset createDataset(std::string name, arma::Mat<T> data)
    {
        return Dataset::create(m_id, name, data);
    }

    Group createGroup(std::string name);

    bool contains(std::string name) const;
protected:
    Group(hid_t id, hid_t parentID, std::string name);

    void setGroupID(hid_t groupID);

private:
    void constructFromOther(const Object &other);
};

}

#endif // GROUP_H
