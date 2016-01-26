# Elegant HDF5

**Note**: Elegant HDF5 is currently in an early stage. 
Some features are missing and the library should be considered very unstable.

Elegant HDF5 is a C++ library for HDF5 files with usability in mind.
One design goal of the library is to be forgiving.
It will assume that you know what you are doing, and go to lengths to make
that possible, rather than throwing exceptions on unorthodox operations.

## Example code

Here's a piece of code that illustrates the use of Elegant HDF5 with Armadillo:

```cpp
#include <armadillo>
#include <elegant/hdf5>

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
```

## Support for matrix libraries

Currently Elegant HDF5 supports reading and writing Armadillo matrices.
Support for other libraries will come in the future.
An extension system for easy implementation of your own types will be
documented in the future.

## Remarks about Armadillo objects

We have standardized matrix I/O on a row-major order and cube I/O on in a
slice-row-column order.
This is similar to what you expect in Python's NumPy.

Because Armadillo stores matrices in a column-major order and slices in the 
order slice-column-row, writing to HDF5 files requires a transpose that
is automatically performed by this library.
Support for direct writing without transposing for increased performance
and interoperability with Matlab and Octave is being considered for future
implementation.
