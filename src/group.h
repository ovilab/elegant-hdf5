#ifndef GROUP_H
#define GROUP_H

#include <string>
#include <hdf5.h>
#include <vector>
#include <iostream>

#include "dataset.h"
#include "object.h"

namespace hdf5 {

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
        hsize_t dims[2];
        dims[0] = data.n_rows;
        dims[1] = data.n_cols;
        hid_t dataspace = H5Screate_simple(2, &dims[0], NULL);
        hid_t creationParameters = H5Pcreate(H5P_DATASET_CREATE);
        hid_t datatype = templateToHdf5Type<T>();
        hid_t dataset = H5Dcreate(m_id, name.c_str(), datatype, dataspace,
                            H5P_DEFAULT, creationParameters, H5P_DEFAULT);
        if(dataset > 0) {
            herr_t errors = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data[0]);
            if(errors >= 0) {
                H5Sclose(dataspace);
                return Dataset(dataset, name);
            }
        }
        return Dataset();
    }

    Group createGroup(std::string name);

    bool contains(std::string name) const;
protected:
    Group(hid_t id, std::string name);

    void setGroupID(hid_t groupID);

private:
    void constructFromOther(const Object &other);
};

}

#endif // GROUP_H
