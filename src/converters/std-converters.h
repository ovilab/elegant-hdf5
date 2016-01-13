#ifndef STDCONVERTERS
#define STDCONVERTERS

#include "../typehelper.h"

#include <vector>

namespace h5cpp {

template<typename eT>
struct TypeHelper<std::vector<eT>> : public SimpleTypeHelper<std::vector<eT>> {
    static hid_t hdfType() {
        return TypeHelper<eT>::hdfType();
    }
    static int dimensionCount() {
        return 1;
    }
    static H5S_class_t dataspaceType() {
        return H5S_SIMPLE;
    }
    static std::vector<eT> objectFromExtents(const std::vector<hsize_t> &extents)
    {
        return std::vector<eT>(extents[0]);
    }
    static std::vector<hsize_t> extentsFromType(const std::vector<eT> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.size();
        return extents;
    }
    static bool matchingExtents(const std::vector<eT> &v, const std::vector<hsize_t> &extents) {
        if(v.size() == extents[0]) {
            return true;
        }
        return false;
    }
    void* writableBuffer(std::vector<eT>& object) {
        return &object[0];
    }
    const void* readableBuffer(const std::vector<eT>& object) {
        return &object[0];
    }
};

}

#endif // STDCONVERTERS

