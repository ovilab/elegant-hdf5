#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>
#include <glog/logging.h>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Opening files in different modes", "[openfiles]") {
    GIVEN("a truncated file" ) {
        File file("myfile.h5", File::OpenMode::Truncate);
        THEN("there should be no attributes") {
            REQUIRE(file.attributeKeys().size() == 0);
        }
        THEN("there should be no keys") {
            REQUIRE(file.keys().size() == 0);
        }
    }
    GIVEN("a read-write file") {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        THEN("we should be able to write") {
            file["test"] = ones(10);
        }
        THEN("we should be able to read") {
            vec test = file["test"];
        }
    }
    GIVEN("a read-only file") {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        THEN("we should be able to read") {
            vec test = file["test"];
        }
    }
}
