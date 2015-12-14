#include "../src/file.h"
#include "../src/object.h"
#include "../src/dataset.h"

#include <iostream>

using namespace std;
using namespace hdf5;
using namespace arma;

int main()
{
    File file("/home/svenni/Dropbox/tmp/new.h5", File::OpenMode::ReadWrite);

    cout << file["myset"] << endl;
    arma::mat A = file["myset"];
    cout << A << endl;

    if(file.contains("nymatrise")) {
        file["nymatrise"] = A;
    } else {
        file.createDataset("nymatrise", A);
    }

    file["nymatrise"] = A;

    file.write("nymatrise", A);

    A = file.read("nymatrise");
    return 0;
}

