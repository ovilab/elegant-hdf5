#ifndef DATASET_H
#define DATASET_H

#include "object.h"
#include <armadillo>
#include <iostream>

namespace h5cpp {

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
    Dataset(hid_t id, hid_t parentID, std::string name);

    Dataset(const Object &temp);
    Dataset& operator=(const Object &other);
    Dataset& operator=(const Dataset &other);
    virtual ~Dataset();

    template<typename T>
    Dataset& operator=(const arma::Mat<T> matrix)
    {
        if(m_id == 0 && m_parentID) {
            *this = Dataset::create(m_parentID, m_name, matrix);
        }
        return *this;
    }

    template<typename T>
    static Dataset create(hid_t parentID, std::string name, arma::Mat<T> data)
    {
        hsize_t dims[2];
        dims[0] = data.n_rows;
        dims[1] = data.n_cols;
        hid_t dataspace = H5Screate_simple(2, &dims[0], NULL);
        hid_t creationParameters = H5Pcreate(H5P_DATASET_CREATE);
        hid_t datatype = templateToHdf5Type<T>();
        hid_t dataset = H5Dcreate(parentID, name.c_str(), datatype, dataspace,
                            H5P_DEFAULT, creationParameters, H5P_DEFAULT);
        if(dataset > 0) {
            herr_t errors = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data[0]);
            if(errors >= 0) {
                H5Sclose(dataspace);
                return Dataset(dataset, parentID, name);
            }
        };
        return Dataset(0, 0, name);
    }

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

template<typename T>
inline void Object::operator=(const arma::Mat<T>& matrix)
{
    if(isValid()) {
        Dataset dataset = *this;
        dataset = matrix;
    } else if(m_parentID > 0) {
        Dataset::create(m_parentID, m_name, matrix);
    }
}

}

#endif // DATASET_H
