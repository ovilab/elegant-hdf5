#include "../src/file.h"
#include "../src/object.h"
#include "../src/dataset.h"
#include "../src/attribute.h"

#include <iostream>

using namespace std;
using namespace h5cpp;
using namespace arma;

int main()
{
    {
        File file("new.h5", File::OpenMode::ReadWrite);
        file("some_number") = 23.0;
    }
    {
        File file("new.h5", File::OpenMode::ReadWrite);
        file("some_number") = 217;
    }
    {
        File file("new.h5", File::OpenMode::ReadOnly);
        for(const string &key : file.keys()) {
            cout << "Found key " << key << endl;
            cout << file[key] << endl;
        }
    }
    {
        File file("new.h5", File::OpenMode::ReadOnly);
        for(const string &key : file.attributes()) {
            cout << "Found attribute " << key << endl;
            cout << file.attribute(key) << endl;
        }
    }
    {
        File file("new.h5", File::OpenMode::ReadOnly);
        double num = file("some_number");
        cout << num << endl;
    }
    {
        File file("new.h5", File::OpenMode::ReadOnly);
        string num = file("woot");
        cout << num << endl;
    }
//    {
//        File file("new.h5", File::OpenMode::ReadWrite);
//        cout << "Writing:" << endl;

//        mat A = ones(2, 2);
//        file["mymatrix"] = A; // 2*ones(2, 2);
//    }
//    {
//        File file("new.h5", File::OpenMode::ReadOnly);
//        cout << "Reading:" << endl;

//        mat A = file["mymatrix"];
//        cout << A << endl;
//    }
//    {
//        File file("new.h5", File::OpenMode::ReadWrite);
//        cube A = ones(2, 5, 3);
//        cout << "Writing:" << endl;

//        cout << A << endl;
//        file["mycube"] = A;
//    }
//    {
//        File file("new.h5", File::OpenMode::ReadOnly);
//        cout << "Reading:" << endl;

//        cube A = file["mycube"];
//        cout << A << endl;
//    }
    return 0;
}

