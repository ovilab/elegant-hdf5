#include "../src/file.h"
#include "../src/object.h"
#include "../src/dataset.h"

#include <iostream>

using namespace std;
using namespace h5cpp;
using namespace arma;

int main()
{
    {
        File file("new.h5", File::OpenMode::ReadWrite);
        mat A = ones(2, 5);
        cout << "Writing:" << endl;

        cout << A << endl;
        file["mymatrix"] = A;
    }
    {
        File file("new.h5", File::OpenMode::ReadOnly);
        cout << "Reading:" << endl;

        mat A = file["mymatrix"];
        cout << A << endl;
    }
    return 0;
}

