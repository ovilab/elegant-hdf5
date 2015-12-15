#ifndef UTILS_H
#define UTILS_H

#include <hdf5.h>
#include <string>
#include <type_traits>

namespace h5cpp {

class Utils
{
public:
    template<typename T>
    static hid_t datatypeFromType();

};

template<>
inline hid_t Utils::datatypeFromType<char>()
{
    return H5T_NATIVE_SCHAR;
}
template<>
inline hid_t Utils::datatypeFromType<unsigned char>() {
    return H5T_NATIVE_UCHAR;
}
template<>
inline hid_t Utils::datatypeFromType<short>() {
    return H5T_NATIVE_SHORT;
}
template<>
inline hid_t Utils::datatypeFromType<unsigned short>() {
    return H5T_NATIVE_USHORT;
}
template<>
inline hid_t Utils::datatypeFromType<int>() {
    return H5T_NATIVE_INT;
}
template<>
inline hid_t Utils::datatypeFromType<unsigned int>() {
    return H5T_NATIVE_UINT;
}
template<>
inline hid_t Utils::datatypeFromType<long>() {
    return H5T_NATIVE_LONG;
}
template<>
inline hid_t Utils::datatypeFromType<unsigned long>() {
    return H5T_NATIVE_ULONG;
}
template<>
inline hid_t Utils::datatypeFromType<long long>() {
    return H5T_NATIVE_LLONG;
}
template<>
inline hid_t Utils::datatypeFromType<unsigned long long>() {
    return H5T_NATIVE_ULLONG;
}
template<>
inline hid_t Utils::datatypeFromType<float>() {
    return H5T_NATIVE_FLOAT;
}
template<>
inline hid_t Utils::datatypeFromType<double>() {
    return H5T_NATIVE_DOUBLE;
}
template<>
inline hid_t Utils::datatypeFromType<long double>() {
    return H5T_NATIVE_LDOUBLE;
}
template<>
inline hid_t Utils::datatypeFromType<bool>() {
    return H5T_NATIVE_HBOOL;
}
}

#endif // UTILS_H

