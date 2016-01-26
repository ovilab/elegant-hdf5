#include <armadillo>
#include <algorithm>

#include "../src/attribute.h"
#include "../src/file.h"

using namespace elegant::hdf5;
using namespace std;
using namespace arma;

int main() {
    File myFile("myfile.h5");

    // Writing simple types
    myFile["my_double"] = 12.0;
    myFile["my_int"] = 5.2;

    // Reading simple types
    int myInt = myFile["my_int"];
    double myDouble = myFile["my_double"];
    cout << "my_int: " << myInt << ", my_double: " << myDouble << endl;

    // Type conversion
    int myDoubleAsInt = myFile["my_double"];
    cout << "my_double: " << myDoubleAsInt << endl;

    // std::vector support
    vector<double> myVector(100);
    std::fill(myVector.begin(), myVector.end(), 123.0);
    myFile["my_vector"] = myVector;

    // Writing Armadillo objects
    mat a = ones(2, 3);
    myFile["my_dataset"] = a;
    mat b = myFile["my_dataset"];
    cout << b << endl;

    // Support for attributes
    myFile.attribute("my_attribute") = std::string("My attribute information");
    myFile.attribute("my_numeric_attribute") = 45.2;

    // Support for groups
    Group myGroup;
    if(myFile.hasKey("my_group")) {
        myGroup = myFile["my_group"];
    } else {
        myGroup = myFile.createGroup("my_group");
    }
    myGroup.attribute("my_group_attribute") = 12.4;
    double myGroupAttribute = myGroup.attribute("my_group_attribute");
    cout << myGroupAttribute << endl;

    return 0;
}
