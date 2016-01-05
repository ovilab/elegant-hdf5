#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>
#include <vector>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Reading and writing std objects", "[std]") {
    GIVEN("a truncated file") {
        File file("std.h5", File::OpenMode::Truncate);
        WHEN("writing an std::vector") {
            vector<double> v{1.2, 4.3, 5.1, 9.3, 2.48};
            file["my_vector"] = v;
            THEN("the same should be read back") {
                vector<double> vr = file["my_vector"];
                vector<double> diff;

                set_difference(v.begin(), v.end(), vr.begin(), vr.end(),
                                    inserter(diff, diff.begin()));

                REQUIRE(diff.size() == 0);
            }
        }
    }
}

