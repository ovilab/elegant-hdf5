#ifndef UTILS_H
#define UTILS_H

#include "logging.h"

#include <hdf5.h>
#include <string>
#include <type_traits>
#include <vector>

namespace h5cpp {

class Dataset;

template<typename T>
struct SimpleTypeHelper
{
    static hid_t hdfType() {
        return 0;
    }
    static int dimensionCount() {
        return 1;
    }
    static T objectFromExtents(const std::vector<hsize_t> &extents) {
        (void)extents;
        return T();
    }
    static void* writeBuffer(T& object) {
        return &object;
    }
    const void* readBuffer(const T& object) {
        return &object;
    }
    static bool matchingExtents(const T &object, const std::vector<hsize_t> &extents) {
        (void)object;
        if(extents[0] != 1) {
            return false;
        }
        return true;
    }
    static std::vector<hsize_t> extentsFromType(const T &object) {
        (void)object;
        std::vector<hsize_t> extents(1);
        extents[0] = 1;
        return extents;
    }
};

template<typename T>
struct TypeHelper : public SimpleTypeHelper<T>
{
    // IMPORTANT: This function is deleted because it must be explicitly implemented for each type used.
    // If not, errors would not appear before compile time because the HDF5 type is not given.
    static hid_t hdfType() = delete;
};

}

#endif // UTILS_H
