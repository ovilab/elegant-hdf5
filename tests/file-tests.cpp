#include "../src/h5cpp/h5cpp"

#include <catch.hpp>
#include <iostream>

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
        THEN("we should get an exception on writing") {
            // TODO throw if file is not writable
//            REQUIRE_THROWS_AS(file["test2"] = 24, std::runtime_error);
        }
    }
}

SCENARIO("Working with invalid files", "[invalidfiles]") {
    GIVEN("a closed file") {
        File file("invalid.h5", File::OpenMode::Truncate);
        file.close();
        WHEN("writing to it") {
            mat A = ones(2, 4);
            THEN("an exception should be thrown") {
                REQUIRE_THROWS_AS(file["my_add"] = A, std::runtime_error);
            }
        }
        WHEN("reading from it") {
            THEN("an exception should be thrown") {
                REQUIRE_THROWS_AS(mat D = file["my_add"], std::runtime_error);
            }
        }
    }
}

SCENARIO("opening file in read-write", "[readwrite]") {
    GIVEN("a read-write file") {
        File file("readwrite.h5");
        WHEN("file is closed and truncated") {
            file.close();
            File file2("readwrite.h5", File::OpenMode::Truncate);
            THEN("it should be empty") {
                REQUIRE(file2.attributeKeys().size() == 0);
                REQUIRE(file2.keys().size() == 0);
            }
        }
        WHEN("file is opened for the 'first' time") {
            THEN("it should be empty") {
                REQUIRE(file.attributeKeys().size() == 0);
                REQUIRE(file.keys().size() == 0);
                REQUIRE(!file.hasKey("my_dataset"));
            }
            THEN("we set a dataset object") {
                file["my_dataset"] = 34;
            }
            THEN("we set some attributes") {
                file.attribute("my_int") = 12;
                file.attribute("my_double") = 41.6;
            }
        }
        WHEN("we open the file again") {
            THEN("the dataset should exist") {
                REQUIRE(file.hasKey("my_dataset"));
            }
            THEN("the attributes should exist") {
                REQUIRE(file.hasAttribute("my_int"));
                REQUIRE(file.hasAttribute("my_double"));
                int i = file.attribute("my_int");
                double d = file.attribute("my_double");
                REQUIRE(i == 12);
                REQUIRE(d == 41.6);
            }
            AND_WHEN("we reset the attributes") {
                file.attribute("my_int") = 45;
                file.attribute("my_double") = 52.8;
                THEN("the attributes should have new values") {
                    int i = file.attribute("my_int");
                    double d = file.attribute("my_double");
                    REQUIRE(i == 45);
                    REQUIRE(d == 52.8);
                }
            }
            AND_WHEN("we change the type of the attributes") {
                file.attribute("my_int") = 91.4;
                file.attribute("my_double") = 31;
                THEN("the attributes should have new values") {
                    double i = file.attribute("my_int");
                    int d = file.attribute("my_double");
                    REQUIRE(i == 91.4);
                    REQUIRE(d == 31);
                }
            }
        }
    }
}

SCENARIO("Continuing work on a closed file", "[closedfile]") {
    GIVEN("a truncated file") {
        File file("closed.h5", File::OpenMode::Truncate);
        WHEN("we assign a few objects and close the file") {
            Group group = file.createGroup("my_group");
            group["my_dataset"] = 24;
            group.attribute("my_attribute") = 12.1;
            Dataset dataset = group["my_dataset"];
            Attribute attribute = group.attribute("my_attribute");
            file.close();
            THEN("the objects should still be accessible") {
                REQUIRE_NOTHROW(int i = group["my_dataset"]);
                REQUIRE_NOTHROW(double d = group.attribute("my_attribute"));
            }
            THEN("re-opening the file and truncating should not work") {
                REQUIRE_THROWS(File file2("closed.h5", File::OpenMode::Truncate));
            }
            AND_WHEN("closing the groups, datasets and attributes") {
                group.close();
                attribute.close();
                dataset.close();
                THEN("opening the file should be ok") {
                    REQUIRE_NOTHROW(File file2("closed.h5", File::OpenMode::Truncate));
                }
            }
        }
    }
}
