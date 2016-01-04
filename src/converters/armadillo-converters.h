#ifndef H5CPP_ARMADILLOCONVERTERS_H
#define H5CPP_ARMADILLOCONVERTERS_H

#include "../typehelper.h"
#include "../dataset.h"

#include <armadillo>

namespace h5cpp {

template<typename T>
struct TypeHelper<arma::Col<T>> : public SimpleTypeHelper<arma::Col<T>> {
    static hid_t hdfType() { return TypeHelper<T>::hdfType(); }
    static int dimensionCount() { return 1; }
    static arma::Col<T> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return arma::Col<T>(extents[0]);
    }
    static std::vector<hsize_t> extentsFromType(const arma::Col<T> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.n_rows;
        return extents;
    }
    static void* writeBuffer(arma::Col<T>& object) {
        return &object[0];
    }
    static const void* readBuffer(const arma::Col<T>& object) {
        return &object[0];
    }
    static bool matchingExtents(const arma::Col<T> &v, const std::vector<hsize_t> &extents) {
        if(v.n_rows == extents[0]) {
            return true;
        }
        return false;
    }
};

template<typename T>
struct TypeHelper<arma::Row<T>> : public SimpleTypeHelper<arma::Row<T>> {
    static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
    static int dimensionCount() { return 1; }
    static arma::Row<T> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return arma::Row<T>(extents[0]);
    }
    static bool matchingExtents(const arma::Row<T> &v, const std::vector<hsize_t> &extents) {
        if(v.n_cols == extents[0]) {
            return true;
        }
        return false;
    }
    static std::vector<hsize_t> extentsFromType(const arma::Row<T> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.n_cols;
        return extents;
    }
    static void* writeBuffer(arma::Row<T>& object) {
        return &object[0];
    }
    static const void* readBuffer(const arma::Row<T>& object) {
        return &object[0];
    }
};

template<typename T>
struct TypeHelper<arma::Mat<T>> : public SimpleTypeHelper<arma::Mat<T>> {
    static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
    static int dimensionCount() { return 2; }
    static arma::Mat<T> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return arma::Mat<T>(extents[0], extents[1]);
    }
    static bool matchingExtents(const arma::Mat<T> &v, const std::vector<hsize_t> &extents) {
        if(v.n_rows == extents[0] && v.n_cols == extents[1]) {
            return true;
        }
        return false;
    }
    static std::vector<hsize_t> extentsFromType(const arma::Mat<T> &v) {
        std::vector<hsize_t> extents(2);
        extents[0] = v.n_rows;
        extents[1] = v.n_cols;
        return extents;
    }
    static void* writeBuffer(arma::Mat<T>& object) {
        return &object[0];
    }
    const void* readBuffer(const arma::Mat<T> &object) {
        return &object[0];
    }
};

template<typename T>
struct TypeHelper<arma::Cube<T>> : public SimpleTypeHelper<arma::Cube<T>> {
    static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
    static int dimensionCount() { return 3; }
    static arma::Cube<T> objectFromExtents(const std::vector<hsize_t> &extents) {
        return arma::Cube<T>(extents[0], extents[1], extents[2]);
    }
    static bool matchingExtents(const arma::Cube<T> &v, const std::vector<hsize_t> &extents) {
        if(v.n_rows == extents[0] && v.n_cols == extents[1]  && v.n_slices == extents[2]) {
            return true;
        }
        return false;
    }
    static std::vector<hsize_t> extentsFromType(const arma::Cube<T> &v) {
        std::vector<hsize_t> extents(3);
        extents[0] = v.n_rows;
        extents[1] = v.n_cols;
        extents[2] = v.n_slices;
        return extents;
    }
    static void* writeBuffer(arma::Cube<T>& object) {
        return &object[0];
    }
    static const void* readBuffer(const arma::Cube<T>& object) {
        return &object[0];
    }
};

template<typename T, typename U>
struct TypeHelper<arma::Gen<T, U>> : public SimpleTypeHelper<arma::Gen<T, U>> {
    static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
    static int dimensionCount() { return TypeHelper<T>::dimensionCount(); }
    static T objectFromExtents(const std::vector<hsize_t> &extents) {
        return TypeHelper<T>::objectFromExtents(extents);
    }
    static bool matchingExtents(const arma::Gen<T, U> &v, const std::vector<hsize_t> &extents) {
        return TypeHelper<T>::matchingExtents(v, extents);
    }
    static std::vector<hsize_t> extentsFromType(const arma::Gen<T, U> &v) {
        return TypeHelper<T>::extentsFromType(v);
    }
    static void* writeBuffer(arma::Gen<T, U> &object) {
        return TypeHelper<T>::writeBuffer(object);
    }
    static const void* readBuffer(const arma::Gen<T, U> &object) {
        return TypeHelper<T>::readBuffer(object);
    }
};

template<typename T, typename U, typename op>
struct TypeHelper<arma::eGlue<T, U, op>> : public SimpleTypeHelper<arma::eGlue<T, U, op>> {
    typedef typename arma::eGlue<T, U, op> object_type;
    typedef typename arma::eGlue<T, U, op>::proxy1_type::stored_type arma_type;

    static hid_t hdfType(){
        return TypeHelper<arma_type>::hdfType();
    }
    static int dimensionCount() {
        return TypeHelper<arma_type>::dimensionCount();
    }
    static T objectFromExtents(const std::vector<hsize_t> &extents) {
        return TypeHelper<arma_type>::objectFromExtents(extents);
    }
    static bool matchingExtents(const object_type &v, const std::vector<hsize_t> &extents) {
        return TypeHelper<arma_type>::matchingExtents(v, extents);
    }
    static std::vector<hsize_t> extentsFromType(const object_type &v) {
        //         std::cout << TypeHelper<arma_type>::extentsFromType(v) << std::endl;
        return TypeHelper<arma_type>::extentsFromType(v);
    }
    static void* writeBuffer(object_type &object) {
        return TypeHelper<arma_type>::writeBuffer(object);
    }
    const void* readBuffer(const object_type &object) {
        temporaryReadableMemory = object;
        return TypeHelper<arma_type>().readBuffer(temporaryReadableMemory);
    }

    arma_type temporaryReadableMemory;
};

}

#endif

