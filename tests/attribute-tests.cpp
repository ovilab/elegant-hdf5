#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>
#include <glog/logging.h>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Reading and writing attributes", "[attributes]") {
    GIVEN("a truncated file, an int and a float") {
        File file("numeric-attributes.h5", File::OpenMode::Truncate);
        double number = 23.4;
        int otherNumber = 19;
        WHEN("a double is written") {
            file.attribute("some_number") = number;
            THEN("the same number should be read") {
                double readNumber = file.attribute("some_number");
                REQUIRE(number == readNumber);
            }
        }
        WHEN("a double is written and overwritten by an int") {
            file.attribute("some_number") = number;
            file.attribute("some_number") = otherNumber;
            THEN("the other number should be read") {
                int readNumber = file.attribute("some_number");
                REQUIRE(readNumber == otherNumber);
            }
            AND_WHEN("file is closed and opened read-only") {
                file.close();
                File readOnlyFile("numeric-attributes.h5", File::OpenMode::ReadOnly);
                THEN("attribute should still be the same") {
                    int readNumber = readOnlyFile.attribute("some_number");
                    REQUIRE(readNumber == otherNumber);
                }
            }
        }
    }
    GIVEN("a truncated file and some strings") {
        File file("string-attributes.h5", File::OpenMode::Truncate);
        string bla = "blablabla";
        WHEN("writing a string attribute") {
            file.attribute("some_string") = bla;
            THEN("the same attribute should be read back") {
                string some = file.attribute("some_string");
                REQUIRE(some == bla);
            }
            AND_WHEN("writing the string to another attribute") {
                file.attribute("other_string") = file.attribute("some_string");
                THEN("the same attribute should be read back") {
                    string other = file.attribute("other_string");
                    REQUIRE(other == bla);
                }
            }
        }
    }
}
