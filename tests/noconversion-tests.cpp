#include <catch.hpp>

#define H5CPP_NO_USER_DEFINED_CONVERSION_OPERATORS
#include <elegant/hdf5/hdf5>
#include <armadillo>

using namespace std;
using namespace elegant::hdf5;
using namespace arma;

SCENARIO("Tests with no user-defined conversion operators", "[noconversion]") {
    GIVEN("a truncated file") {
        File file("noconversion.h5", File::OpenMode::Truncate);
        WHEN("a double is written") {
            double number = 23.4;
            file.attribute("some_number") = number;
            THEN("the same number should be read") {
                double readNumber = file.attribute("some_number").value<double>();
                REQUIRE(number == readNumber);
            }
        }
        WHEN("a matrix is written") {
            mat A = ones(2, 2);
            file.attribute("some_mat") = A;
            file["matmat"] = A;
            THEN("the same matrix should be read") {
                mat B = file.attribute("some_mat").value<mat>();
                mat C = file["matmat"].value<mat>();
                C = file["matmat"].value<mat>();
                REQUIRE(0 == max(max(abs(A - B))));
                REQUIRE(0 == max(max(abs(B - C))));
            }
        }
        WHEN("a string is written") {
            string bla = "blablabla";
            file.attribute("some_string") = bla;
            THEN("the same number should be read") {
                string readString = file.attribute("some_string").value<string>();
                REQUIRE(bla == readString);
            }
        }
    }
}
