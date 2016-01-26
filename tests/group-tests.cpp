#include "../src/elegant/hdf5/hdf5"

#include <catch.hpp>
#include <iostream>

using namespace std;
using namespace elegant::hdf5;
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
                AND_THEN("creating another group with the same name should not throw") {
                    REQUIRE_NOTHROW(file.createGroup("my_group"));
                }
            }
        }
        WHEN("a dataset object is created") {
            file["my_dataset"] = 24;
            THEN("creating a group with the same name should throw") {
                REQUIRE_THROWS_AS(file.createGroup("my_dataset"), std::runtime_error);
            }
        }
    }
}
