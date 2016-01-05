#include <catch.hpp>

#include <h5cpp/h5cpp>
#include <armadillo>

using namespace h5cpp;
using namespace arma;

SCENARIO("Continuing work on a closed file", "[!hide]") {
    GIVEN("a truncated file") {
        File file("closed.h5", File::OpenMode::Truncate);
        WHEN("we assign a few objects and close the file") {
            Group group = file.createGroup("my_group");
            group["my_dataset"] = 24;
            group.attribute("my_attribute") = 12.1;
            Dataset dataset = group["my_dataset"];
            Attribute attribute = group.attribute("my_attribute");
            file.close();
            THEN("the objects should still cout fine") {
                REQUIRE_NOTHROW(cout << dataset << endl);
                REQUIRE_NOTHROW(cout << attribute << endl);
            }
            THEN("the objects should throw on access") {
                REQUIRE_THROWS(int i = group["my_dataset"]);
                REQUIRE_THROWS(double d = group.attribute("my_attribute"));
            }
//            THEN("re-opening the file and truncating should work fine") {
//                File file2("closed.h5", File::OpenMode::Truncate);
//                file2["my_dataset"] = 12;
//            }
        }
    }
}

//TEST_CASE("Testing stuff", "[stuff]"){
//    cerr << "=======" << endl;
//    cerr << "=======" << endl;
//    {
//        File file("myfile.h5", File::OpenMode::ReadOnly);
//        for(const string &key : file.attributeKeys()) {
//            cerr << "Found attribute " << key << endl;
//            cerr << file.attribute(key) << endl;
//        }
//    }
//    cerr << "=======" << endl;
//    {
//        cerr << "Starting my stuff" << endl;
//        File file("myfile.h5", File::OpenMode::ReadWrite);
//        Dataset d;
//        cerr << "Assignment" << endl;
//        d = file["my_stuff"];
//        cerr << d << endl;
//        Object e = d;
//        cerr << "Copy to cube" << endl;
//        cerr << e << endl;
//        cerr << d << endl;
//        cube A = e;
//        cerr << A << endl;
//        cerr << "Assignment 2" << endl;
//        Dataset f = e;
//        f = ones(2,3);
//    }
//    cerr << "=======" << endl;
//    {
//        File file("myfile.h5", File::OpenMode::ReadWrite);
//        file["my_mat1"] = ones(2,3);
//        file["my_mat2"] = file["my_mat1"]; // <---- TODO!

//        mat m = file["my_mat1"];
//        mat s = file["my_mat2"];
//        cerr << m << endl;
//        cerr << s << endl;

//        file["my_stuff"] = file["my_mat1"];

//        mat f = file["my_stuff"];
//        cerr << f << endl;
//    }
//    cerr << "=======" << endl;
//    {
//        File file("myfile.h5", File::OpenMode::ReadWrite);
//        Group g = file.createGroup("groupy");
//        g["test"] = ones(3,4);
//        g["blest"] = ones(1,2);
//        g["fest"] = g["test"];
//        file["groupx"] = g;
//        file["groupz"] = file["groupy"];

//        Dataset d = file["my_mat1"];
//        d = file["groupy"];
//    }
//    cerr << "=======" << endl;
//    {
//        File file("myfile.h5", File::OpenMode::ReadOnly);
//        for(const Attribute &attribute : file.attributes()) {
//            cerr << "Found attribute " << attribute.name() << endl;
//            cerr << attribute << endl;
//        }
//    }
//    cerr << "=======" << endl;
//    {
//        File file("myfile.h5", File::OpenMode::ReadWrite);
//        string bla = "blablabla";
//        file.attribute("tomb_raider") = bla;
//        file.attribute("lara_croft") = file.attribute("tomb_raider");

//        string tomb = file.attribute("tomb_raider");
//        string lara = file.attribute("lara_croft");
//        cerr << tomb << endl;
//        cerr << lara << endl;
//    }
//    cerr << "=======" << endl;
//}


