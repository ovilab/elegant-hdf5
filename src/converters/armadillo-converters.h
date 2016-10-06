#ifndef H5CPP_ARMADILLOCONVERTERS_H
#define H5CPP_ARMADILLOCONVERTERS_H

#include "../io/typehelper.h"
#include "../dataset.h"

#include <armadillo>

namespace elegant {
namespace hdf5 {


template<typename eT>
struct TypeHelper<arma::Col<eT>> : public SimpleTypeHelper<arma::Col<eT>> {
    using ObjectType = arma::Col<eT>;
    hid_t hdfType() const {
        return TypeHelper<eT>().hdfType();
    }
    int dimensionCount() const {
        return 1;
    }
    H5S_class_t dataspaceType() {
        return H5S_SIMPLE;
    }
    std::vector<hsize_t> extents(const arma::Col<eT> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.n_rows;
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

template<typename eT>
struct TypeHelper<arma::Row<eT>> : public SimpleTypeHelper<arma::Row<eT>> {
    using ObjectType = arma::Row<eT>;
    hid_t hdfType() {
        return TypeHelper<eT>().hdfType();
    }
    H5S_class_t dataspaceType() {
        return H5S_SIMPLE;
    }
    int dimensionCount() {
        return 1;
    }
    std::vector<hsize_t> extents(const arma::Row<eT> &v) {
        std::vector<hsize_t> extents(1);
        extents[0] = v.n_cols;
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

template<typename eT>
struct TypeHelper<arma::Mat<eT>> : public SimpleTypeHelper<arma::Mat<eT>> {
    using ObjectType = arma::Mat<eT>;
    hid_t hdfType() const {
        return TypeHelper<eT>().hdfType();
    }
    int dimensionCount() const {
        return 2;
    }
    H5S_class_t dataspaceType() const {
        return H5S_SIMPLE;
    }
    std::vector<hsize_t> extents(const ObjectType &object) {
        std::vector<hsize_t> extents(2);
        extents[0] = object.n_rows;
        extents[1] = object.n_cols;
        return extents;
    }
    ObjectType readFromFile(const std::vector<hsize_t> &extents, Reader &reader) {
        ObjectType object(extents[1], extents[0]); // swap n_rows and n_cols because we will transpose
        reader.read(&object[0]);
        return object.t();
    }
    void writeToFile(const ObjectType &object, Writer& writer) {
        ObjectType rotated = object.t();
        writer.write(&rotated[0]);
    }
};

template<typename eT>
struct TypeHelper<arma::Cube<eT>> : public SimpleTypeHelper<arma::Cube<eT>> {
    using ObjectType = arma::Cube<eT>;
    hid_t hdfType() const {
        return TypeHelper<eT>().hdfType();
    }
    int dimensionCount() const {
        return 3;
    }
    H5S_class_t dataspaceType() const {
        return H5S_SIMPLE;
    }
    std::vector<hsize_t> extents(const arma::Cube<eT> &v) {
        std::vector<hsize_t> extents(3);
        extents[0] = v.n_slices;
        extents[1] = v.n_rows;
        extents[2] = v.n_cols;
        return extents;
    }
    ObjectType readFromFile(const std::vector<hsize_t> &extents, Reader &reader) {
        ObjectType object(extents[2], extents[1], extents[0]); // swap n_rows and n_cols because we will transpose
        reader.read(&object[0]);
        arma::Cube<eT> rotated(object.n_cols, object.n_rows, object.n_slices); // swap n_rows and n_cols
        for(int i = 0; i < int(object.n_slices); i++) {
            rotated.slice(i) = object.slice(i).t();
        }
        return rotated;
    }
    void writeToFile(const ObjectType &object, Writer& writer) {
        arma::Cube<eT> rotated(object.n_cols, object.n_rows, object.n_slices); // swap n_rows and n_cols
        for(int i = 0; i < int(object.n_slices); i++) {
            rotated.slice(i) = object.slice(i).t();
        }
        writer.write(&rotated[0]);
    }
    arma::Cube<eT> rotated;
};

template<typename TargetType, typename op>
struct TypeHelper<arma::Gen<TargetType, op>> : public SimpleTypeHelper<arma::Gen<TargetType, op>> {
    typedef typename arma::Gen<TargetType, op> ObjectType;

    hid_t hdfType(){
        return TypeHelper<TargetType>().hdfType();
    }
    int dimensionCount() {
        return TypeHelper<TargetType>().dimensionCount();
    }
    H5S_class_t dataspaceType() {
        return TypeHelper<TargetType>().dataspaceType();
    }
    std::vector<hsize_t> extents(const ObjectType &v) {
        return TypeHelper<TargetType>().extents(v);
    }
    void writeToFile(const ObjectType &object, Writer& writer) {
        TypeHelper<TargetType>().writeToFile(object, writer);
    }
};

template<typename T, typename U, typename op>
struct TypeHelper<arma::eGlue<T, U, op>> : public SimpleTypeHelper<arma::eGlue<T, U, op>> {
    typedef typename arma::eGlue<T, U, op> ObjectType;
    typedef T TargetType;

    hid_t hdfType(){
        return TypeHelper<TargetType>().hdfType();
    }
    int dimensionCount() {
        return TypeHelper<TargetType>().dimensionCount();
    }
    H5S_class_t dataspaceType() {
        return TypeHelper<TargetType>().dataspaceType();
    }
    std::vector<hsize_t> extents(const ObjectType &v) {
        return TypeHelper<TargetType>().extents(v);
    }
    void writeToFile(const ObjectType &object, Writer& writer) {
        TypeHelper<TargetType>().writeToFile(object, writer);
    }
};

}
}

#endif

