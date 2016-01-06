#include <catch.hpp>

#include <h5cpp/h5cpp>
#include <armadillo>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Sandbox tests", "[sandbox]") {
//    GIVEN("tests") {
////        File file("sandbox.h5", File::OpenMode::ReadWrite);
//        File file("sandbox.h5", File::OpenMode::Truncate);
////        file["lol"] = true;
//        file["lol"] = 34.2;
////        bool a = file["lol"];
//        if(file["lol"]) {
//            cout << "Yes" << endl;
//        } else {
//            cout << "No" << endl;
//        }
//    }
    GIVEN("a truncated file") {
        File file("sandbox.h5", File::OpenMode::Truncate);
        WHEN("a double is written") {
            double number = 23.4;
            file.attribute("some_number") = number;
            THEN("the same number should be read") {
                double readNumber = file.attribute("some_number");
                REQUIRE(number == readNumber);
            }
        }
        WHEN("a matrix is written") {
            mat A = ones(2, 2);
            file.attribute("some_mat") = A;
            file["matmat"] = A;
            THEN("the same matrix should be read") {
                mat B = file.attribute("some_mat");
                mat C = file["matmat"];
                C = file["matmat"].value<mat>();
                cout << A << endl;
                cout << B << endl;
                cout << C << endl;
            }
        }
        WHEN("a string is written") {
            string bla = "blablabla";
            file.attribute("some_string") = bla;
            THEN("the same number should be read") {
                string readString = file.attribute("some_string");
                REQUIRE(bla == readString);
            }
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


