# Elegant HDF5

**WARNING**: 
Elegant HDF5 is currently in an early stage.
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
#include <algorithm>

#include "../src/attribute.h"
#include "../src/file.h"

using namespace elegant::hdf5;
using namespace std;
using namespace arma;

int main() {
    File results("results.h5");

    // Writing simple types
    results["coffee_consumed"] = 12.4;
    results["meeting_hours"] = 5.2;

    // Reading simple types
    double meetingHours = results["meeting_hours"];
    double coffeeConsumed = results["coffee_consumed"];
    cout << "meeting_hours: " << meetingHours << endl << "coffee_consumed: " << coffeeConsumed << endl;

    // Implicit type conversion (can be disabled)
    int coffeeConsumedAsInt = results["coffee_consumed"];
    cout << "coffee_consumed: " << coffeeConsumedAsInt << endl;

    // std::vector support
    vector<double> sleepHoursPerDay = {5.4, 7.8, 9.2, 6.8, 7.5, 8.6};
    results["sleep_hours_per_day"] = sleepHoursPerDay;

    // Writing Armadillo objects
    mat a = {{2.3, 9.2, 1.0},
             {4.5, 2.4, 8.9}};
    results["travel_distances"] = a;

    // Reading Armadillo objects
    mat b = results["travel_distances"];
    cout << "Travel distances:" << endl << b << endl;

    // Support for attributes
    results.attribute("address") = std::string("Karl Johans gate");
    results.attribute("average_oxygen_pressure") = 45.2;

    // Support for groups
    Group cars;
    if(results.hasKey("cars")) {
        cars = results["cars"];
    } else {
        cars = results.createGroup("cars");
    }
    cars.attribute("batmobile_weight") = 7912.34;

    double batmobileWeight = cars.attribute("batmobile_weight");
    cout << "Batmobile weight: " << batmobileWeight << endl;

    return 0;
}
```

## Preliminary installation instructions

**WARNING**: The package is in a very early, experimental stage. 
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
