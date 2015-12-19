#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <hdf5.h>
#include <vector>
#include <iostream>

#include "attribute.h"
#include "dataset.h"
#include "object.h"

namespace h5cpp {

class Group : public Object
{
public:
    Group();
    Group(const Object &other);
    void operator=(const Object &other);
    void operator=(const Group &other); // TODO: Consider operator chaining

    virtual ~Group();

    std::vector<std::string> keys() const;
    std::vector<Object> items() const;

    Object item(std::string key) const;
    Object operator[](std::string key) const;

    template<typename T>
    Dataset createDataset(std::string name, arma::Mat<T> data);

    Group createGroup(std::string name);

    bool hasKey(std::string name) const;
    std::vector<std::string> attributes() const;
    Attribute operator ()(std::string key) const;
    h5cpp::Attribute attribute(std::string key) const;
    bool hasAttribute(std::string name) const;
protected:
    Group(hid_t id, hid_t parentID, std::string name);

    void setGroupID(hid_t groupID);

private:
    void constructFromOther(const Object &other);
};

template<typename T>
Dataset Group::createDataset(std::string name, arma::Mat<T> data)
{
    return Dataset::create(m_id, name, data);
}

}

#endif // GROUP_H
