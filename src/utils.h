#ifndef UTILS_H
#define UTILS_H

#include <hdf5.h>
#include <string>
#include <type_traits>
#include <armadillo>

namespace h5cpp {

template<typename T>
struct datatypeFromType
{
    hid_t operator()(){
        return 0;
    }
};

template<>
struct datatypeFromType<char>
{
    hid_t operator()(){ return H5T_NATIVE_SCHAR; }
};

template<>
struct datatypeFromType<unsigned char> {
    hid_t operator()(){ return H5T_NATIVE_UCHAR; }
};

template<>
struct datatypeFromType<short> {
    hid_t operator()(){ return H5T_NATIVE_SHORT; }
};

template<>
struct datatypeFromType<unsigned short> {
    hid_t operator()(){ return H5T_NATIVE_USHORT; }
};

template<>
struct datatypeFromType<int> {
    hid_t operator()(){ return H5T_NATIVE_INT; }
};

template<>
struct datatypeFromType<unsigned int> {
    hid_t operator()(){ return H5T_NATIVE_UINT; }
};

template<>
struct datatypeFromType<long> {
    hid_t operator()(){ return H5T_NATIVE_LONG; }
};

template<>
struct datatypeFromType<unsigned long> {
    hid_t operator()(){ return H5T_NATIVE_ULONG; }
};

template<>
struct datatypeFromType<long long> {
    hid_t operator()(){ return H5T_NATIVE_LLONG; }
};

template<>
struct datatypeFromType<unsigned long long> {
    hid_t operator()(){ return H5T_NATIVE_ULLONG; }
};

template<>
struct datatypeFromType<float> {
    hid_t operator()(){ return H5T_NATIVE_FLOAT; }
};

template<>
struct datatypeFromType<double> {
    hid_t operator()(){ return H5T_NATIVE_DOUBLE; }
};

template<>
struct datatypeFromType<long double> {
    hid_t operator()(){ return H5T_NATIVE_LDOUBLE; }
};

template<>
struct datatypeFromType<bool> {
    hid_t operator()(){ return H5T_NATIVE_HBOOL; }
};
template<typename T>
struct datatypeFromType<arma::Row<T>> {
     hid_t operator()(){ return datatypeFromType<T>()(); }
};
template<typename T>
struct datatypeFromType<arma::Col<T>> {
     hid_t operator()(){ return datatypeFromType<T>()(); }
};
template<typename T>
struct datatypeFromType<arma::Mat<T>> {
     hid_t operator()(){ return datatypeFromType<T>()(); }
};
template<typename T>
struct datatypeFromType<arma::Cube<T>> {
     hid_t operator()(){ return datatypeFromType<T>()(); }
};

}

#endif // UTILS_H

