#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>
#include <glog/logging.h>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Reading and writing groups", "[groups]") {
    GIVEN("a truncated file") {
        File file("myfile.h5", File::OpenMode::Truncate);
        WHEN("a group is created") {
            file.createGroup("my_group");
            THEN("a group should exist") {
                Group group = file["my_group"];
                REQUIRE(group.isGroup());
                REQUIRE(group.type() == Object::Type::Group);
                REQUIRE(group.name() == "my_group");
            }
        }
    }
}
