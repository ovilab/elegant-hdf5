#ifndef H5CPP_ARMADILLOCONVERTERS_H
#define H5CPP_ARMADILLOCONVERTERS_H

#include "../typehelper.h"
#include "../dataset.h"

#include <armadillo>

namespace h5cpp {

template<typename eT>
struct TypeHelper<arma::Col<eT>> : public SimpleTypeHelper<arma::Col<eT>> {
    static hid_t hdfType() { return TypeHelper<eT>::hdfType(); }
    static int dimensionCount() { return 1; }
    static arma::Col<eT> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return arma::Col<eT>(extents[0]);
    }
    static std::vector<hsize_t> extentsFromType(const arma::Col<eT> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.n_rows;
        return extents;
    }
    static bool matchingExtents(const arma::Col<eT> &v, const std::vector<hsize_t> &extents) {
        if(v.n_rows == extents[0]) {
            return true;
        }
        return false;
    }
    static void* writeBuffer(arma::Col<eT>& object) {
        return &object[0];
    }
    const void* readBuffer(const arma::Col<eT>& object) {
        return &object[0];
    }
};

template<typename eT>
struct TypeHelper<arma::Row<eT>> : public SimpleTypeHelper<arma::Row<eT>> {
    static hid_t hdfType(){ return TypeHelper<eT>::hdfType(); }
    static int dimensionCount() { return 1; }
    static arma::Row<eT> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return arma::Row<eT>(extents[0]);
    }
    static bool matchingExtents(const arma::Row<eT> &v, const std::vector<hsize_t> &extents) {
        if(v.n_cols == extents[0]) {
            return true;
        }
        return false;
    }
    static std::vector<hsize_t> extentsFromType(const arma::Row<eT> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.n_cols;
        return extents;
    }
    static void* writeBuffer(arma::Row<eT>& object) {
        return &object[0];
    }
    const void* readBuffer(const arma::Row<eT>& object) {
        return &object[0];
    }
};

template<typename eT>
struct TypeHelper<arma::Mat<eT>> : public SimpleTypeHelper<arma::Mat<eT>> {
    static hid_t hdfType(){ return TypeHelper<eT>::hdfType(); }
    static int dimensionCount() { return 2; }
    static arma::Mat<eT> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return arma::Mat<eT>(extents[0], extents[1]);
    }
    static bool matchingExtents(const arma::Mat<eT> &v, const std::vector<hsize_t> &extents) {
        if(v.n_rows == extents[0] && v.n_cols == extents[1]) {
            return true;
        }
        return false;
    }
    static std::vector<hsize_t> extentsFromType(const arma::Mat<eT> &v) {
        std::vector<hsize_t> extents(2);
        extents[0] = v.n_rows;
        extents[1] = v.n_cols;
        return extents;
    }
    static void* writeBuffer(arma::Mat<eT>& object) {
        return &object[0];
    }
    const void* readBuffer(const arma::Mat<eT> &object) {
        return &object[0];
    }
};

template<typename eT>
struct TypeHelper<arma::Cube<eT>> : public SimpleTypeHelper<arma::Cube<eT>> {
    static hid_t hdfType(){ return TypeHelper<eT>::hdfType(); }
    static int dimensionCount() { return 3; }
    static arma::Cube<eT> objectFromExtents(const std::vector<hsize_t> &extents) {
        return arma::Cube<eT>(extents[0], extents[1], extents[2]);
    }
    static bool matchingExtents(const arma::Cube<eT> &v, const std::vector<hsize_t> &extents) {
        if(v.n_rows == extents[0] && v.n_cols == extents[1]  && v.n_slices == extents[2]) {
            return true;
        }
        return false;
    }
    static std::vector<hsize_t> extentsFromType(const arma::Cube<eT> &v) {
        std::vector<hsize_t> extents(3);
        extents[0] = v.n_rows;
        extents[1] = v.n_cols;
        extents[2] = v.n_slices;
        return extents;
    }
    static void* writeBuffer(arma::Cube<eT>& object) {
        return &object[0];
    }
    const void* readBuffer(const arma::Cube<eT>& object) {
        return &object[0];
    }
};

template<typename T, typename op>
struct TypeHelper<arma::Gen<T, op>> : public SimpleTypeHelper<arma::Gen<T, op>> {
    static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
    static int dimensionCount() { return TypeHelper<T>::dimensionCount(); }
    static T objectFromExtents(const std::vector<hsize_t> &extents) {
        return TypeHelper<T>::objectFromExtents(extents);
    }
    static bool matchingExtents(const arma::Gen<T, op> &v, const std::vector<hsize_t> &extents) {
        return TypeHelper<T>::matchingExtents(v, extents);
    }
    static std::vector<hsize_t> extentsFromType(const arma::Gen<T, op> &v) {
        return TypeHelper<T>::extentsFromType(v);
    }
    static void* writeBuffer(arma::Gen<T, op> &object) {
        return TypeHelper<T>::writeBuffer(object);
    }
    const void* readBuffer(const arma::Gen<T, op> &object) {
        return TypeHelper<T>().readBuffer(object);
    }
};

template<typename T, typename U, typename op>
struct TypeHelper<arma::eGlue<T, U, op>> : public SimpleTypeHelper<arma::eGlue<T, U, op>> {
    typedef typename arma::eGlue<T, U, op> object_type;
    typedef typename arma::eGlue<T, U, op>::proxy1_type::stored_type arma_type;
    // TODO what if result has different type?

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
        return TypeHelper<arma_type>::extentsFromType(v);
    }
    const void* readBuffer(const object_type &object) {
        temporaryReadableMemory = object;
        return TypeHelper<arma_type>().readBuffer(temporaryReadableMemory);
    }

    arma_type temporaryReadableMemory;
};

}

#endif

