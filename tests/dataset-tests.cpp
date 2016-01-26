#include "../src/elegant/hdf5/hdf5"

#include <catch.hpp>
#include <iostream>

using namespace std;
using namespace elegant::hdf5;
using namespace arma;

SCENARIO("Writing different datasets", "[datasets]") {
    GIVEN("a truncated file") {
        File file("dataset.h5", File::OpenMode::Truncate);

        int scalar_int = 82;
        float scalar_float = 83.4f;
        double scalar_double = 85.1;

        file["scalar_int"] = scalar_int;
        file["scalar_float"] = scalar_float;
        file["scalar_double"] = scalar_double;

        int read_int = file["scalar_int"];
        float read_float = file["scalar_float"];
        double read_double = file["scalar_double"];
        REQUIRE(read_int == scalar_int);
        REQUIRE(read_float == scalar_float);
        REQUIRE(read_double == scalar_double);
    }
}

