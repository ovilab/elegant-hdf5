#ifndef DATASET_H
#define DATASET_H

#include "object.h"
#include "utils.h"

#include <armadillo>
#include <iostream>
#include <typeinfo>

namespace h5cpp {

template<class T>
struct is_vec {
    static constexpr bool value = false;
};

template<>
struct is_vec<arma::vec> {
    static constexpr bool value = true;
};

template<class T>
struct is_vec<arma::Col<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_vec<arma::Row<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_mat {
    static constexpr bool value = false;
};

template<class T>
struct is_mat<arma::Mat<T>> {
    static constexpr bool value = true;
};

template<class T>
struct is_cube {
    static constexpr bool value = false;
};

template<class T>
struct is_cube<arma::Cube<T>> {
    static constexpr bool value = true;
};

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
    bool matchingExtents(const arma::Col<T> &v, hsize_t *extents) {
        if(v.n_rows == extents[0]) {
            return true;
        }
        return false;
    }

    template<typename T>
    bool matchingExtents(const arma::Mat<T> &v, hsize_t *extents) {
        if(v.n_rows == extents[0] && v.n_cols == extents[1]) {
            return true;
        }
        return false;
    }

    template<typename T>
    bool matchingExtents(const arma::Cube<T> &v, hsize_t *extents) {
        if(v.n_rows == extents[0] && v.n_cols == extents[1]  && v.n_slices == extents[2]) {
            return true;
        }
        return false;
    }

    template<typename T>
    static void extentsFromType(const arma::Col<T> &v, hsize_t *extents) {
        extents[0] = v.n_rows;
    }

    template<typename T>
    static void extentsFromType(const arma::Mat<T> &v, hsize_t *extents) {
        extents[0] = v.n_rows;
        extents[1] = v.n_cols;
    }

    template<typename T>
    static void extentsFromType(const arma::Cube<T> &v, hsize_t *extents) {
        extents[0] = v.n_rows;
        extents[1] = v.n_cols;
        extents[2] = v.n_slices;
    }

    template<typename T>
    Dataset& operator=(const T &data)
    {
        if(m_id == 0 && m_parentID) {
            *this = Dataset::create(m_parentID, m_name, data);
        } else {
            int targetDimensions = 2;
            if(is_vec<T>::value) {
                targetDimensions = 1;
            } else if(is_mat<T>::value) {
                targetDimensions = 2;
            } else if(is_cube<T>::value) {
                targetDimensions = 3;
            } else {
                std::cerr << "Could not determine dimensions of object." << std::endl;
                return *this;
            }
            hid_t dataspace = H5Dget_space(m_id);
            int currentDimensions = H5Sget_simple_extent_ndims(dataspace);

            hsize_t extents[3];
            H5Sget_simple_extent_dims(dataspace, extents, NULL);

            bool shouldOverwrite = false;
            if(currentDimensions != targetDimensions || !matchingExtents(data, extents)) {
                shouldOverwrite = true;
            }

            if(shouldOverwrite) {
#ifdef H5CPP_VERBOSE
                    std::cerr << "WARNING: Writing over dataset of shape (" << extents[0] << ", " << extents[1] << ") "
                              << "with matrix of shape (" << data.n_rows << ", " << data.n_cols << "). "
                              << "A new dataset will be created and "
                              << "limitations in HDF5 makes it impossible to free the old dataset." << std::endl;
#endif
                H5Sclose(dataspace);
                H5Dclose(m_id);
                H5Ldelete(m_parentID, m_name.c_str(), H5P_DEFAULT);
                *this = Dataset::create(m_parentID, m_name, data);
            } else {
                hid_t datatype = TypeHelper<T>::hdfType();
                herr_t errors = H5Dwrite(m_id, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data[0]);
                H5Sclose(dataspace);
                if(errors < 0) {
                    std::cerr << "Error writing to dataset!" << std::endl;
                }
            }
        }
        return *this;
    }

    template<typename T, typename U>
    Dataset& operator=(const arma::Gen<T, U> &data) {
        T dataReal = data;
        *this = dataReal;
        return *this;
    }

    template<typename T>
    static Dataset create(hid_t parentID, const std::string &name, const T &data)
    {
        int targetDimensions = 0;
        if(arma::is_Col<T>::value || arma::is_Row<T>::value) {
            targetDimensions = 1;
        } else if(arma::is_Mat<T>::value) {
            targetDimensions = 2;
        } else if(arma::is_Cube<T>::value) {
            targetDimensions = 3;
        } else {
            std::cerr << "ERROR: Could not determine dimensions of object." << std::endl;
            return Dataset(0, 0, name);
        }
        hsize_t dims[3];
        extentsFromType(data, dims);
        hid_t dataspace = H5Screate_simple(targetDimensions, &dims[0], NULL);
        hid_t creationParameters = H5Pcreate(H5P_DATASET_CREATE);
        hid_t datatype = TypeHelper<T>::hdfType();
        hid_t dataset = H5Dcreate(parentID, name.c_str(), datatype, dataspace,
                            H5P_DEFAULT, creationParameters, H5P_DEFAULT);
        if(dataset > 0) {
            herr_t errors = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &data[0]);
            if(errors >= 0) {
                H5Sclose(dataspace);
                std::cerr << "Returning the created dataset " << dataset << std::endl;
                return Dataset(dataset, parentID, name);
            }
        };
        return Dataset(0, 0, name);
    }

    // TODO: Support other types of rvalues, such as eOP
    template<typename T, typename U>
    static Dataset create(hid_t parentID, const std::string &name, const arma::Gen<T, U> &data) {
        T dataReal = data;
        return create(parentID, name, dataReal);
    }

    template<typename T>
    operator arma::Row<T>() const
    {
        std::cerr << "Making a row" << std::endl;
        hid_t dataspace = H5Dget_space(m_id);
        int dimensionCount = H5Sget_simple_extent_ndims(dataspace);

        if(dimensionCount != 1) {
            std::cerr << "ERROR: Tried to copy dataspace with "
                      << dimensionCount << " dimensions to arma::mat." << std::endl;
            return arma::Row<T>();
        }

        hsize_t extents[dimensionCount];
        H5Sget_simple_extent_dims(dataspace, extents, NULL);

        arma::Row<T> matrix(extents[0]);

        hid_t hdf5Datatype = datatypeFromType<T>()();
        H5Dread(m_id, hdf5Datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &matrix[0]);
        return matrix;
    }

    template<typename T>
    operator arma::Col<T>() const
    {
        hid_t dataspace = H5Dget_space(m_id);
        int dimensionCount = H5Sget_simple_extent_ndims(dataspace);

        if(dimensionCount != 1) {
            std::cerr << "ERROR: Tried to copy dataspace with "
                      << dimensionCount << " dimensions to arma::mat." << std::endl;
            return arma::Mat<T>();
        }

        hsize_t extents[dimensionCount];
        H5Sget_simple_extent_dims(dataspace, extents, NULL);

        arma::Col<T> matrix(extents[0]);

        hid_t hdf5Datatype = datatypeFromType<T>()();
        H5Dread(m_id, hdf5Datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &matrix[0]);
        return matrix;
    }

    template<typename T>
    operator arma::Mat<T>() const
    {
        hid_t dataspace = H5Dget_space(m_id);
        int dimensionCount = H5Sget_simple_extent_ndims(dataspace);

        if(dimensionCount != 2) {
            std::cerr << "ERROR: Tried to copy dataspace with "
                      << dimensionCount << " dimensions to arma::mat." << std::endl;
            return arma::Mat<T>();
        }

        hsize_t extents[dimensionCount];
        H5Sget_simple_extent_dims(dataspace, extents, NULL);

        arma::Mat<T> matrix(extents[0], extents[1]);

        hid_t hdf5Datatype = TypeHelper<T>::hdfType();
        H5Dread(m_id, hdf5Datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &matrix[0]);
        return matrix;
    }

    template<typename T>
    operator arma::Cube<T>() const
    {
        hid_t dataspace = H5Dget_space(m_id);
        int dimensionCount = H5Sget_simple_extent_ndims(dataspace);

        if(dimensionCount != 3) {
            std::cerr << "ERROR: Tried to copy dataspace with "
                      << dimensionCount << " dimensions to arma::mat." << std::endl;
            return arma::Cube<T>();
        }

        hsize_t extents[dimensionCount];
        H5Sget_simple_extent_dims(dataspace, extents, NULL);

        arma::Cube<T> cube(extents[0], extents[1], extents[2]);

        hid_t hdf5Datatype = TypeHelper<T>::hdfType();
        H5Dread(m_id, hdf5Datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, &cube[0]);

        H5Sclose(dataspace);
        return cube;
    }
private:
    void constructFromOther(const Object &other);
};

template<typename T>
inline void Object::operator=(const T& matrix)
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
