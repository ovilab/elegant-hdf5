#ifndef DATASET_H
#define DATASET_H

#include "object.h"
#include <armadillo>
#include <iostream>

namespace hdf5 {

template<typename T>
hid_t templateToHdf5Type()
{
    hid_t hdf5Datatype = H5T_NATIVE_DOUBLE;
    if (std::is_same<T, char>::value) {
        hdf5Datatype = H5T_NATIVE_SCHAR;
    } else if (std::is_same<T, unsigned char>::value) {
        hdf5Datatype = H5T_NATIVE_UCHAR;
    } else if (std::is_same<T, short>::value) {
        hdf5Datatype = H5T_NATIVE_SHORT;
    } else if (std::is_same<T, unsigned short>::value) {
        hdf5Datatype = H5T_NATIVE_USHORT;
    } else if (std::is_same<T, int>::value) {
        hdf5Datatype = H5T_NATIVE_INT;
    } else if (std::is_same<T, unsigned int>::value) {
        hdf5Datatype = H5T_NATIVE_UINT;
    } else if (std::is_same<T, long>::value) {
        hdf5Datatype = H5T_NATIVE_LONG;
    } else if (std::is_same<T, unsigned long>::value) {
        hdf5Datatype = H5T_NATIVE_ULONG;
    } else if (std::is_same<T, long long>::value) {
        hdf5Datatype = H5T_NATIVE_LLONG;
    } else if (std::is_same<T, unsigned long long>::value) {
        hdf5Datatype = H5T_NATIVE_ULLONG;
    } else if (std::is_same<T, float>::value) {
        hdf5Datatype = H5T_NATIVE_FLOAT;
    } else if (std::is_same<T, double>::value) {
        hdf5Datatype = H5T_NATIVE_DOUBLE;
    } else if (std::is_same<T, long double>::value) {
        hdf5Datatype = H5T_NATIVE_LDOUBLE;
    } else if (std::is_same<T, bool>::value) {
        hdf5Datatype = H5T_NATIVE_HBOOL;
    }
    return hdf5Datatype;
}

class Dataset : public Object
{
public:
    Dataset();
    Dataset(hid_t id, std::string name);
    Dataset(const Object &temp);
    void operator=(const Object &other);
    void operator=(const Dataset &other);
    virtual ~Dataset();

    template<typename T>
    operator arma::Mat<T>() const
    {
        hid_t dataspace = H5Dget_space(m_id);
        int dimensionCount = H5Sget_simple_extent_ndims(dataspace);

        if(dimensionCount != 2) {
            std::cerr << "ERROR: Tried to copy dataspace with "
                      << dimensionCount << " dimensions to arma::mat." << std::endl;
            return arma::Mat<T>(0, 0);
        }

        hsize_t extents[dimensionCount];
        H5Sget_simple_extent_dims(dataspace, extents, NULL);

        arma::Mat<T> matrix(extents[0], extents[1]);

        hid_t hdf5Datatype = templateToHdf5Type<T>();
        H5Dread(m_id, hdf5Datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &matrix[0]);
        return matrix;
    }
private:
    void constructFromOther(const Object &other);
};


}

#endif // DATASET_H
