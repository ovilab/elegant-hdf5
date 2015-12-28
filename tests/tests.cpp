#include "../src/file.h"
#include "../src/object.h"
#include "../src/dataset.h"
#include "../src/attribute.h"

#include <hdf5.h>
#include <hdf5_hl.h>

#include <iostream>

using namespace std;
using namespace h5cpp;
using namespace arma;

int main()
{
    {
        File file("myfile.h5", File::OpenMode::Truncate);
        cerr << file.attributeKeys().size() << endl;
        cerr << file.items().size() << endl;
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        file.attribute("some_number") = 23.0;
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        file.attribute("some_number") = 217;
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        double num = file.attribute("some_number");
        cerr << num << endl;
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::Truncate);
        cerr << "Writing:" << endl;

        cerr << "Colvec write" << endl;
        colvec c = ones(5);
        file["my_colvec"] = c;

        cerr << "Rowvec" << endl;
        rowvec r = ones<rowvec>(5);
        cerr << "Writing rowvec" << endl;
        file["my_rowvec"] = r;
        cerr << "Done writing" << endl;

        cerr << "Matrix" << endl;
        mat ma = ones(2, 4);
        file["my_mat"] = ma;

        cerr << "Cube" << endl;
        cube cu = ones(2, 5, 3);
        file["my_cube"] = cu;

        file.attribute("real_attr") = 123.94;
        file.attribute("int_attr") = 24;
        //        file.attribute("string_attr") = "lol";
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);

        cerr << "Colvec read" << endl;
        colvec c = file["my_colvec"];
        cerr << c << endl;

        rowvec r = file["my_rowvec"];
        cerr << r << endl;

        mat ma = file["my_mat"];
        cerr << ma << endl;

        cube cu = file["my_cube"];
        cerr << cu << endl;
    }
    cerr << "=======" << endl;
    {
        cerr << "Write my stuff" << endl;
        File file("myfile.h5", File::OpenMode::ReadWrite);
        cerr << "Here comes assignment" << endl;
        file["my_stuff"] = mat(4, 5);
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        Dataset d = file["my_stuff"];
        d = cube(8, 1, 3);
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        file.createGroup("my_group");
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        for(const string &key : file.keys()) {
            cerr << "Found key " << key << endl;
            cerr << file[key] << endl;
        }
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        for(const string &key : file.attributeKeys()) {
            cerr << "Found attribute " << key << endl;
            cerr << file.attribute(key) << endl;
        }
    }
    cerr << "=======" << endl;
    {
        cerr << "Starting my stuff" << endl;
        File file("myfile.h5", File::OpenMode::ReadWrite);
        Dataset d;
        cerr << "Assignment" << endl;
        d = file["my_stuff"];
        cerr << d << endl;
        Object e = d;
        cerr << "Copy to cube" << endl;
        cerr << e << endl;
        cerr << d << endl;
        cube A = e;
        cerr << A << endl;
        cerr << "Assignment 2" << endl;
        Dataset f = e;
        f = ones(2,3);
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        for(const Attribute &attribute : file.attributes()) {
            cerr << "Found attribute " << attribute.name() << endl;
            cerr << attribute << endl;
        }
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        file["my_mat1"] = ones(2,3);
        file["my_mat2"] = file["my_mat1"]; // <---- TODO!

        mat m = file["my_mat1"];
        mat s = file["my_mat2"];
        cerr << m << endl;
        cerr << s << endl;

        file["my_stuff"] = file["my_mat1"];

        mat f = file["my_stuff"];
        cerr << f << endl;
    }
    cerr << "=======" << endl;
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        Group g = file.createGroup("groupy");
        g["test"] = ones(3,4);
        g["blest"] = ones(1,2);
        g["fest"] = g["test"];
        file["groupx"] = g;
        file["groupz"] = file["groupy"];

        Dataset d = file["my_mat1"];
        d = file["groupy"];
    }
    cerr << "=======" << endl;
    return 0;
}

