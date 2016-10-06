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

    // Type conversion
//    int coffeeConsumedAsInt = results["coffee_consumed"];
//    cout << "coffee_consumed: " << coffeeConsumedAsInt << endl;

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
