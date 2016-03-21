# Elegant HDF5

**WARNING**: Elegant HDF5 is currently in an early stage. 
Some features are missing and the library should be considered very unstable.
**Expect big and sudden API changes in the upcoming releases.**

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
    double myDouble = myFile["my_int"];
    cout << "my_int: " << myInt << " my_double: " << myDouble << endl;
    
    // Type conversion
    int myDoubleAsInt = myFile["my_double"];
    cout << "my_double: " << myDoubleAsInt << endl;
    
    // std::vector support
    vector<double> myVector(100);
    myVector.fill(123);
    myFile["my_vector"] = myVector;
    
    // Writing Armadillo objects
    mat a = ones(10, 15);
    myFile["my_dataset"] = a;
    mat b = myFile["my_dataset"];
    
    // Support for attributes
    myFile.attribute("my_attribute") = "My attribute information";
    myFile.attribute("my_numeric_attribute") = 45.2;
    
    // Support for groups
    Group myGroup = myFile.createGroup("my_group");
    myGroup.attribute("my_group_attribute") = 12.4;
    
    return 0;
}
```

## Preliminary installation instructions

*WARNING*: The package is in a very early, experimental stage. 
In the future, simpler installation and conan packages will be provided. 
Expect big and sudden API changes in the upcoming releases.

To test the example on Ubuntu, do the following:

```
sudo apt-get install catch libhdf5-dev qmake
git clone https://github.com/ovilab/elegant-hdf5.git
cd elegant-hdf5
qmake
make
LD_LIBRARY_PATH=src ./example/example
```

This should run the example.

### Conan package installation

You may also do a local conan package installation.
See the [Conan documentation](http://docs.conan.io/en/latest/) for more details.

In short, what you need to do is the following:

```
sudo apt-get install catch libhdf5-dev qmake
git clone https://github.com/ovilab/elegant-hdf5.git
cd elegant-hdf5
conan export dragly/master
```

In your project's folder add a file named conanfile.txt:

```
[requires]
h5cpp/0.1@dragly/master

[generators]
qmake
cmake
qbs
```

Then run the following command to generate the conanbuildinfo files:

```
conan install --build
```

Then you may add the conanbuildinfo file of your choice depending on your build platform.

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
