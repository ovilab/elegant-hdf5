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
     static const void* readBuffer(const arma::Mat<T>& object) {
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

}

#endif

