#ifndef GROUP_H
#define GROUP_H

#include "logging.h"
#include "attribute.h"
#include "dataset.h"
#include "object.h"

#include <string>
#include <hdf5.h>
#include <vector>
#include <iostream>

namespace h5cpp {

class Group : public Object
{
public:
    Group();
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

    template<typename T>
    Dataset createDataset(std::string name, arma::Mat<T> data);

    Group createGroup(std::string name);

    bool hasKey(std::string name) const;
    std::vector<Attribute> attributes() const;
    std::vector<std::string> attributeKeys() const;
    Attribute operator ()(std::string key) const;
    h5cpp::Attribute attribute(std::string key) const;
    bool hasAttribute(std::string name) const;

private:
    Group(hid_t id, hid_t parentID, std::string name);

//    void constructFromOther(const Object &other);
//    void close();
};

template<typename T>
Dataset Group::createDataset(std::string name, arma::Mat<T> data)
{
    return Dataset::create(m_id, name, data);
}

}

#endif // GROUP_H
