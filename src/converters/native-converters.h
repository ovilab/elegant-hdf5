#ifndef NATIVECONVERTERS
#define NATIVECONVERTERS

#include "../typehelper.h"

namespace elegant {
namespace hdf5 {


template<>
struct TypeHelper<char> : public SimpleTypeHelper<char> {
    hid_t hdfType(){ return H5T_NATIVE_SCHAR; }
};

template<>
struct TypeHelper<unsigned char> : public SimpleTypeHelper<unsigned char> {
    hid_t hdfType(){ return H5T_NATIVE_UCHAR; }
};

template<>
struct TypeHelper<short> : public SimpleTypeHelper<short> {
    hid_t hdfType(){ return H5T_NATIVE_SHORT; }
};

template<>
struct TypeHelper<unsigned short> : public SimpleTypeHelper<unsigned short> {
    hid_t hdfType(){ return H5T_NATIVE_USHORT; }
};

template<>
struct TypeHelper<int> : public SimpleTypeHelper<int> {
    hid_t hdfType(){ return H5T_NATIVE_INT; }
};

template<>
struct TypeHelper<unsigned int> : public SimpleTypeHelper<unsigned int> {
    hid_t hdfType(){ return H5T_NATIVE_UINT; }
};

template<>
struct TypeHelper<long> : public SimpleTypeHelper<long> {
    hid_t hdfType(){ return H5T_NATIVE_LONG; }
};

template<>
struct TypeHelper<unsigned long> : public SimpleTypeHelper<unsigned long> {
    hid_t hdfType(){ return H5T_NATIVE_ULONG; }
};

template<>
struct TypeHelper<long long> : public SimpleTypeHelper<long long> {
    hid_t hdfType(){ return H5T_NATIVE_LLONG; }
};

template<>
struct TypeHelper<unsigned long long> : public SimpleTypeHelper<unsigned long long> {
    hid_t hdfType(){ return H5T_NATIVE_ULLONG; }
};

template<>
struct TypeHelper<float> : public SimpleTypeHelper<float> {
    hid_t hdfType(){ return H5T_NATIVE_FLOAT; }
};

template<>
struct TypeHelper<double> : public SimpleTypeHelper<double> {
    hid_t hdfType(){ return H5T_NATIVE_DOUBLE; }
};

template<>
struct TypeHelper<long double> : public SimpleTypeHelper<long double> {
    hid_t hdfType(){ return H5T_NATIVE_LDOUBLE; }
};

template<>
struct TypeHelper<bool> : public SimpleTypeHelper<bool> {
    hid_t hdfType() {
        return H5T_NATIVE_INT;
    }
    const void* readableBuffer(const bool &object) {
        value = object; // convert to int before storing because HDF5 1.8.x doesn't support bool
        return &value;
    }
    int value;
};

}
}

#endif // NATIVECONVERTERS
