#ifndef UTILS_H
#define UTILS_H

#include <hdf5.h>
#include <string>
#include <type_traits>
#include <armadillo>

namespace h5cpp {

template<typename T>
struct TypeHelper
{
    static hid_t hdfType(){
        return 0;
    }
};

template<>
struct TypeHelper<char>
{
    static hid_t hdfType(){ return H5T_NATIVE_SCHAR; }
};

template<>
struct TypeHelper<unsigned char> {
    static hid_t hdfType(){ return H5T_NATIVE_UCHAR; }
};

template<>
struct TypeHelper<short> {
    static hid_t hdfType(){ return H5T_NATIVE_SHORT; }
};

template<>
struct TypeHelper<unsigned short> {
    static hid_t hdfType(){ return H5T_NATIVE_USHORT; }
};

template<>
struct TypeHelper<int> {
    static hid_t hdfType(){ return H5T_NATIVE_INT; }
};

template<>
struct TypeHelper<unsigned int> {
    static hid_t hdfType(){ return H5T_NATIVE_UINT; }
};

template<>
struct TypeHelper<long> {
    static hid_t hdfType(){ return H5T_NATIVE_LONG; }
};

template<>
struct TypeHelper<unsigned long> {
    static hid_t hdfType(){ return H5T_NATIVE_ULONG; }
};

template<>
struct TypeHelper<long long> {
    static hid_t hdfType(){ return H5T_NATIVE_LLONG; }
};

template<>
struct TypeHelper<unsigned long long> {
    static hid_t hdfType(){ return H5T_NATIVE_ULLONG; }
};

template<>
struct TypeHelper<float> {
    static hid_t hdfType(){ return H5T_NATIVE_FLOAT; }
};

template<>
struct TypeHelper<double> {
    static hid_t hdfType(){ return H5T_NATIVE_DOUBLE; }
};

template<>
struct TypeHelper<long double> {
    static hid_t hdfType(){ return H5T_NATIVE_LDOUBLE; }
};

template<>
struct TypeHelper<bool> {
    static hid_t hdfType(){ return H5T_NATIVE_HBOOL; }
};
template<typename T>
struct TypeHelper<arma::Col<T>> {
     static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
};
template<typename T>
struct TypeHelper<arma::Row<T>> {
     static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
};
template<typename T>
struct TypeHelper<arma::Mat<T>> {
     static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
};
template<typename T>
struct TypeHelper<arma::Cube<T>> {
     static hid_t hdfType(){ return TypeHelper<T>::hdfType(); }
};

}

#endif // UTILS_H
