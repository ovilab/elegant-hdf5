#include "../src/elegant/hdf5/hdf5"

#include <catch.hpp>
#include <iostream>

using namespace std;
using namespace elegant::hdf5;
using namespace arma;

SCENARIO("Reading and writing armadillo objects", "[armadillo]") {
    GIVEN("a truncated file") {
        File file("armadillo.h5", File::OpenMode::Truncate);
    }
    GIVEN("a read-write file") {
        File file("armadillo.h5", File::OpenMode::ReadWrite);
        WHEN("writing a column vector") {
            colvec c = ones(5);
            file["my_colvec"] = c;
            THEN("the same should be read back") {
                colvec cr = file["my_colvec"];;
                REQUIRE(0 == Approx(max(abs(c - cr))));
            }
        }
        WHEN("writing a row vector") {
            rowvec r = ones<rowvec>(5);
            file["my_rowvec"] = r;
            THEN("the same should be read back") {
                rowvec rr = file["my_rowvec"];
                REQUIRE(0 == Approx(max(abs(r - rr))));
            }
        }
        WHEN("writing a matrix") {
            mat ma = ones(2, 4);
            file["my_mat"] = ma;
            THEN("the same should be read back") {
                mat mar = file["my_mat"];
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
            }
            THEN("the same should be read back after initialization") {
                mat mar;
                mar = file["my_mat"].value<mat>();
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
                file["my_mat"] >> mar;
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
                file["my_mat"] >> mar;
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
                Dataset ds = file["my_mat"];
                mar = ds.value<mat>();
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
                ds >> mar;
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
                ds >> mar;
                REQUIRE(0 == Approx(max(max(abs(ma - mar)))));
            }
        }
        WHEN("writing matrix to test order") {
            File file2("arma2.h5", File::OpenMode::Truncate);
            mat ma(6, 8);
            for(unsigned int i = 0; i < ma.n_rows; i++) {
                for(unsigned int j = 0; j < ma.n_cols; j++) {
                    ma(i, j) = j + i * ma.n_cols;
                }
            }
            file2["my_ordered_mat"] = ma;
        }
        WHEN("writing a couple of objects") {
            mat ma = ones(2, 4);
            file["my_mat"] = ma;
            THEN("their keys should be found") {
                string matstring = "my_mat";
                auto keys = file.keys();
                REQUIRE(std::find(keys.begin(), keys.end(), matstring) != keys.end());
            }
        }
        WHEN("writing a cube") {
            cube cu = ones(2, 5, 3);
            file["my_cube"] = cu;
            THEN("the same should be read back") {
                cube cur = file["my_cube"];
                cube diff = cu - cur;
                diff = abs(diff);
                double diffs = diff.max();
                REQUIRE(0 == Approx(diffs));
            }
        }
        WHEN("writing a generated object") {
            file["my_ones"] = ones(2, 4);
            THEN("the same should be read back") {
                mat C = ones(2, 4);
                mat D = file["my_ones"];
                REQUIRE(0 == Approx(max(max(abs(C - D)))));
            }
        }
        WHEN("writing an operation result") {
            mat A = ones(2, 4);
            mat B = ones(2, 4);
            file["my_add"] = A + 2*B;
            THEN("the result should be read back") {
                mat C = A + 2*B;
                mat D = file["my_add"];
                REQUIRE(0 == Approx(max(max(abs(C - D)))));
            }
        }
        WHEN("writing a scalar") {
            colvec c = ones(5);
            double scalar = 85.4;
            file["my_scalar"] = scalar;
            THEN("the same should be read back to a cube") {
                cube cr = file["my_scalar"].value<cube>(Object::ConversionFlags::GreaterThanOrEqualDimensionCount);
                REQUIRE(cr(0, 0, 0) == scalar);
            }
        }
    }
}
