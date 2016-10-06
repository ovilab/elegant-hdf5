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
    int meetingHours = results["meeting_hours"];
    double coffeeConsumed = results["coffee_consumed"];
    cout << "meeting_hours: " << meetingHours << ", coffee_consumed: " << coffeeConsumed << endl;

    // Type conversion
    int coffeeConsumedAsInt = results["coffee_consumed"];
    cout << "coffee_consumed: " << coffeeConsumedAsInt << endl;

    // std::vector support
    vector<double> sleepHoursPerDay = {5.4, 7.8, 9.2, 6.8, 7.5, 8.6};
    results["sleep_hours_per_day"] = sleepHoursPerDay;

    // Writing Armadillo objects
    mat a = ones(2, 3);
    results["my_dataset"] = a;
    mat b = results["my_dataset"];
    cout << b << endl;

    // Support for attributes
    results.attribute("my_attribute") = std::string("My attribute information");
    results.attribute("my_numeric_attribute") = 45.2;

    // Support for groups
    Group myGroup;
    if(results.hasKey("my_group")) {
        myGroup = results["my_group"];
    } else {
        myGroup = results.createGroup("my_group");
    }
    myGroup.attribute("my_group_attribute") = 12.4;
    double myGroupAttribute = myGroup.attribute("my_group_attribute");
    cout << myGroupAttribute << endl;

    return 0;
}
