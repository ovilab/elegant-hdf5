#ifndef NATIVECONVERTERS
#define NATIVECONVERTERS

#include "../typehelper.h"

namespace h5cpp {

template<>
struct TypeHelper<char> : public SimpleTypeHelper<char> {
    static hid_t hdfType(){ return H5T_NATIVE_SCHAR; }
};

template<>
struct TypeHelper<unsigned char> : public SimpleTypeHelper<unsigned char> {
    static hid_t hdfType(){ return H5T_NATIVE_UCHAR; }
};

template<>
struct TypeHelper<short> : public SimpleTypeHelper<short> {
    static hid_t hdfType(){ return H5T_NATIVE_SHORT; }
};

template<>
struct TypeHelper<unsigned short> : public SimpleTypeHelper<unsigned short> {
    static hid_t hdfType(){ return H5T_NATIVE_USHORT; }
};

template<>
struct TypeHelper<int> : public SimpleTypeHelper<int> {
    static hid_t hdfType(){ return H5T_NATIVE_INT; }
};

template<>
struct TypeHelper<unsigned int> : public SimpleTypeHelper<unsigned int> {
    static hid_t hdfType(){ return H5T_NATIVE_UINT; }
};

template<>
struct TypeHelper<long> : public SimpleTypeHelper<long> {
    static hid_t hdfType(){ return H5T_NATIVE_LONG; }
};

template<>
struct TypeHelper<unsigned long> : public SimpleTypeHelper<unsigned long> {
    static hid_t hdfType(){ return H5T_NATIVE_ULONG; }
};

template<>
struct TypeHelper<long long> : public SimpleTypeHelper<long long> {
    static hid_t hdfType(){ return H5T_NATIVE_LLONG; }
};

template<>
struct TypeHelper<unsigned long long> : public SimpleTypeHelper<unsigned long long> {
    static hid_t hdfType(){ return H5T_NATIVE_ULLONG; }
};

template<>
struct TypeHelper<float> : public SimpleTypeHelper<float> {
    static hid_t hdfType(){ return H5T_NATIVE_FLOAT; }
};

template<>
struct TypeHelper<double> : public SimpleTypeHelper<double> {
    static hid_t hdfType(){ return H5T_NATIVE_DOUBLE; }
};

template<>
struct TypeHelper<long double> : public SimpleTypeHelper<long double> {
    static hid_t hdfType(){ return H5T_NATIVE_LDOUBLE; }
};

template<>
struct TypeHelper<bool> : public SimpleTypeHelper<bool> {
    static hid_t hdfType(){ return H5T_NATIVE_HBOOL; }
};

}

#endif // NATIVECONVERTERS
