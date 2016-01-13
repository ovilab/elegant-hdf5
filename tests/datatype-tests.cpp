#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Testing datatypes", "[datatypes]") {
    GIVEN("a truncated file") {
        File file("armadillo.h5", File::OpenMode::Truncate);
        file.attribute("int") = 24;
        file.attribute("float") = 24.1f;
        file.attribute("double") = 24.2;

        REQUIRE(file.attribute("int").datatype() == Datatype::Type::Int);
        REQUIRE(file.attribute("float").datatype() == Datatype::Type::Float);
        REQUIRE(file.attribute("double").datatype() == Datatype::Type::Double);
    }
}

