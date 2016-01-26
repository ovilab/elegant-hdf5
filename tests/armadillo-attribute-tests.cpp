#include "../src/elegant/hdf5/Attribute"
#include "../src/elegant/hdf5/File"

#include <catch.hpp>
#include <iostream>

using namespace std;
using namespace elegant::hdf5;
using namespace arma;

SCENARIO("Reading and writing Armadillo attributes", "[attributes]") {
    GIVEN("a truncated file") {
        File file("armadillo-attributes.h5", File::OpenMode::Truncate);
    }
    GIVEN("a read-write file and some attributes") {
        File file("armadillo-attributes.h5");
        mat A = ones(4, 5);
        WHEN("a matrix is written") {
            file.attribute("some_mat") = A;
            THEN("the same matrix should be read") {
                mat B = file.attribute("some_mat");
                REQUIRE(0 == Approx(max(max(abs(A - B)))));
            }
        }
    }
}

