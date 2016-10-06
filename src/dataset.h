#ifndef DATASET_H
#define DATASET_H

#include "object.h"
#include "io/typehelper.h"
#include "utils/logging.h"
#include "utils/demangle.h"
#include "datatype.h"
#include "dataspace.h"
#include "converters/native-converters.h"
#include "converters/std-converters.h"

#ifndef H5CPP_NO_ARMA
#include "converters/armadillo-converters.h"
#endif

#include <iostream>
#include <typeinfo>

namespace elegant {
namespace hdf5 {

class Dataset : public Object
{
public:
    Dataset();
    Dataset(hid_t id, hid_t parentID, std::string name);

    Dataset(const Object &other);
    Dataset(const Dataset &other);

    ~Dataset();

    Dataset& operator=(const Object &other);
    Dataset& operator=(const Dataset &other);

    template<typename T>
    Dataset& operator=(const T &data);

    Datatype::Type datatype() const;

    bool isScalar() const;
    bool isSimple() const;
    int dimensionCount() const;
    std::vector<hsize_t> extents() const;

    template<typename T>
    static Dataset create(hid_t parentID, const std::string &name, const T &data);

private:
    std::vector<hsize_t> extents(hid_t dataspace) const;
    Datatype datatype_impl() const;
    Dataspace dataspace() const;

    template<typename T>
    T valueImpl(Object::ConversionFlags mode) const;

    friend class Object;
    friend class DatasetWriter;
};

}
}

#include "dataset.tpp"

#endif // DATASET_H
