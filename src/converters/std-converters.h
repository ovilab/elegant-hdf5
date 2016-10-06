#ifndef STDCONVERTERS
#define STDCONVERTERS

#include "../io/typehelper.h"

#include <vector>

namespace elegant {
namespace hdf5 {


template<typename eT>
struct TypeHelper<std::vector<eT>> : public SimpleTypeHelper<std::vector<eT>> {
    using ObjectType = std::vector<eT>;
    hid_t hdfType() const {
        return TypeHelper<eT>().hdfType();
    }
    int dimensionCount() const {
        return 1;
    }
    H5S_class_t dataspaceType() {
        return H5S_SIMPLE;
    }
    std::vector<hsize_t> extents(const ObjectType &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.size();
        return extents;
    }
    ObjectType readFromFile(const std::vector<hsize_t> &extents, Reader &reader) {
        ObjectType object(extents[0]);
        reader.read(&object[0]);
        return object;
    }
    void writeToFile(const ObjectType &object, Writer& writer) {
        writer.write(&object[0]);
    }
};

}
}

#endif // STDCONVERTERS

