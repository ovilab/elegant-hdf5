#include <catch.hpp>

#include <h5cpp/h5cpp>
#include <armadillo>

using namespace h5cpp;
using namespace arma;

SCENARIO("sandbox", "[sandbox]") {
    GIVEN("something") {
        File file("armadillo.h5", File::OpenMode::Truncate);
        WHEN("writing an operation result") {
            mat A = ones(2, 4);
            mat B = ones(2, 4);
            file["my_add"] = A + B;
//            THEN("the result should be read back") {
//                mat C = A + B;
//                mat D = file["my_add"];
//                cout << D << endl;
//                REQUIRE(0 == Approx(max(max(abs(C - D)))));
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


