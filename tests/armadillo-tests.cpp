#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Reading and writing armadillo objects", "[armadillo]") {
    GIVEN("a truncated file") {
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
    }
}
