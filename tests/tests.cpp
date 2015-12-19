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
        File file("myfile.h5", File::OpenMode::Truncate);
        cout << file.attributes().size() << endl;
        cout << file.items().size() << endl;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        file.attribute("some_number") = 23.0;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        file.attribute("some_number") = 217;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        double num = file.attribute("some_number");
        cout << num << endl;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        cout << "Writing:" << endl;

        mat A = ones(2, 2);
        file["mymatrix"] = A; // 2*ones(2, 2);
    }
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        cout << "Reading:" << endl;

        Dataset myMatrix = file["mymatrix"];
        Object myObject = myMatrix;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadWrite);
        cube A = ones(2, 5, 3);
        cout << "Writing:" << endl;

        cout << A << endl;
        file["mycube"] = A;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        cout << "Reading:" << endl;

        cube A = file["mycube"];
        cout << A << endl;
    }
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        for(const string &key : file.keys()) {
            cout << "Found key " << key << endl;
            cout << file[key] << endl;
        }
    }
    {
        File file("myfile.h5", File::OpenMode::ReadOnly);
        for(const string &key : file.attributes()) {
            cout << "Found attribute " << key << endl;
            cout << file.attribute(key) << endl;
        }
    }
//    {
//        File file("myfile.h5", File::OpenMode::ReadOnly);
//        for(const Attribute &attribute : file.attributes()) {
//            cout << "Found attribute " << attribute.name() << endl;
//            cout << attribute << endl;
//        }
//    }
    return 0;
}

