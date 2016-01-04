#ifndef DATASET_H
#define DATASET_H

#include "object.h"
#include "typehelper.h"
#include "logging.h"
#include "demangle.h"

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

    Dataset(const Object &other);
    Dataset(const Dataset &other);
    //    Dataset(Dataset &&other);

    ~Dataset();

    Dataset& operator=(const Object &other);
    Dataset& operator=(const Dataset &other);

    template<typename T>
    Dataset& operator=(const T &data);

    // TODO Support other arma temporaries, like eOp
    template<typename T, typename U>
    Dataset& operator=(const arma::Gen<T, U> &data);

    template<typename T>
    static Dataset create(hid_t parentID, const std::string &name, const T &data);

    // TODO: Support other types of rvalues, such as eOP
    template<typename T, typename U>
    static Dataset create(hid_t parentID, const std::string &name, const arma::Gen<T, U> &data);

    template<typename T>
    operator T();
};

template<>
inline Object::operator Dataset() {
    return Dataset(*this);
}

template<typename T>
inline void Object::operator=(const T& matrix)
{
    DLOG(INFO) << "Assignment operator of T";
    DLOG(INFO) << "Is valid: " << isValid();
    if(isValid()) {
        Dataset dataset = *this;
        dataset = matrix;
    } else if(m_parentID > 0) {
        Dataset::create(m_parentID, m_name, matrix);
    }
}

template<typename T>
Dataset& Dataset::operator=(const T &data)
{
    DLOG(INFO) << "Dataset assignment operator of T type: " << typeid(T).name();
    DLOG(INFO) << "Parent, name, id: " << m_parentID << " " << m_name << " " << m_id;
    if(m_id == 0 && m_parentID > 0) {
        *this = Dataset::create(m_parentID, m_name, data);
    } else {
        int targetDimensions = TypeHelper<T>::dimensionCount();
        hid_t dataspace = H5Dget_space(m_id);
        int currentDimensions = H5Sget_simple_extent_ndims(dataspace);

        std::vector<hsize_t> extents(currentDimensions);
        H5Sget_simple_extent_dims(dataspace, &extents[0], NULL);

        bool shouldOverwrite = false;
        if(currentDimensions != targetDimensions || !TypeHelper<T>::matchingExtents(data, extents)) {
            shouldOverwrite = true;
        }

        if(shouldOverwrite) {
            //#ifdef H5CPP_VERBOSE
            FLAGS_stderrthreshold = 1;
            DLOG(WARNING) << "Writing over dataset of different shape. "
                          << "Limitations in HDF5 standard makes it impossible to free space taken "
                          << "up by the old dataset." << std::endl;
            //#endif
            H5Sclose(dataspace);
            close();
            H5Ldelete(m_parentID, m_name.c_str(), H5P_DEFAULT);
            *this = Dataset::create(m_parentID, m_name, data);
        } else {
            DLOG(INFO) << "Writing to old dataset";
            hid_t datatype = TypeHelper<T>::hdfType();
            herr_t errors = H5Dwrite(m_id, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, TypeHelper<T>::readBuffer(data));
            H5Sclose(dataspace);
            if(errors < 0) {
                DLOG(INFO) << "Error writing to dataset!";
            }
        }
    }
    return *this;
}

template<typename T, typename U>
Dataset& Dataset::operator=(const arma::Gen<T, U> &data) {
    T dataReal = data;
    *this = dataReal;
    return *this;
}

template<typename T>
Dataset Dataset::create(hid_t parentID, const std::string &name, const T &data)
{
    DLOG(INFO) << "Creating dataset on parent " << parentID << " with name " << name;
    int targetDimensions = TypeHelper<T>::dimensionCount();
    std::vector<hsize_t> extents = TypeHelper<T>::extentsFromType(data);
    DLOG(INFO) << "Extents: " << extents[0] << " "
               << (targetDimensions > 1 ? extents[1] : 0) << " "
               << (targetDimensions > 2 ? extents[2] : 0);

    hid_t dataspace = H5Screate_simple(targetDimensions, &extents[0], NULL);
    hid_t creationParameters = H5Pcreate(H5P_DATASET_CREATE);
    hid_t datatype = TypeHelper<T>::hdfType();
    hid_t dataset = H5Dcreate(parentID, name.c_str(), datatype, dataspace,
                              H5P_DEFAULT, creationParameters, H5P_DEFAULT);
    if(dataset > 0) {
        herr_t errors = H5Dwrite(dataset, datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, TypeHelper<T>::readBuffer(data));
        if(errors >= 0) {
            H5Sclose(dataspace);
            DLOG(INFO) << "Returning the created dataset " << dataset;
            return Dataset(dataset, parentID, name);
        }
    };
    return Dataset(0, 0, name);
}

template<typename T, typename U>
Dataset Dataset::create(hid_t parentID, const std::string &name, const arma::Gen<T, U> &data) {
    T dataReal = data;
    return create(parentID, name, dataReal);
}

template<typename T>
inline Dataset::operator T()
{
    DLOG(INFO) << "Getting dataspace for " << m_id;
    hid_t dataspace = H5Dget_space(m_id);
    int dimensionCount = H5Sget_simple_extent_ndims(dataspace);
    DLOG(INFO) << "Dimensions are " << dimensionCount;

    if(dimensionCount != TypeHelper<T>::dimensionCount()) {
        std::cerr << "ERROR: Tried to copy dataspace with "
                  << dimensionCount << " dimensions to type T" << std::endl; // TODO: Use demangle and friends
        return T();
    }

    std::vector<hsize_t> extents(dimensionCount);
    H5Sget_simple_extent_dims(dataspace, &extents[0], NULL);
    DLOG(INFO) << "Got extents:";
    for(int i = 0; i < dimensionCount; i++) {
        DLOG(INFO) << extents[i];
    }

    T object = TypeHelper<T>::objectFromExtents(extents);

    hid_t hdf5Datatype = TypeHelper<T>::hdfType();
    H5Dread(m_id, hdf5Datatype, H5S_ALL, H5S_ALL, H5P_DEFAULT, TypeHelper<T>::writeBuffer(object));
    return object;
}

}

#endif // DATASET_H
