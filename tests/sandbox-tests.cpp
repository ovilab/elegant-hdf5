#include <catch.hpp>

#include <h5cpp/h5cpp>
#include <armadillo>

using namespace std;
using namespace h5cpp;
using namespace arma;

SCENARIO("Sandbox tests", "[sandbox]") {
    GIVEN("a truncated file") {
        File file("armadillo.h5", File::OpenMode::Truncate);
    }
    GIVEN("a read-write file") {
        File file("armadillo.h5", File::OpenMode::ReadWrite);
//        WHEN("writing an operation result") {
//            mat A = ones(2, 4);
//            mat B = ones(2, 4);
//            file["my_add"] = A + 2*B;
//            THEN("the result should be read back") {
//                mat C = A + 2*B;
//                mat D = file["my_add"];
//                REQUIRE(0 == Approx(max(max(abs(C - D)))));
//            }
//        }
        WHEN("writing other stuff") {
            cube cu(4, 2, 3);
            for(unsigned int i = 0; i < cu.n_slices; i++) {
                for(unsigned int j = 0; j < cu.n_rows; j++) {
                    for(unsigned int k = 0; k < cu.n_cols; k++) {
                        cu(j, k, i) = k + j * cu.n_cols + i * cu.n_cols * cu.n_rows;
                    }
                }
            }
            file["cubic"] = cu;
            file["cubic"].attribute("milad") = 92.5;
            file["cubic"].attribute("some_number") = 410.87;
            file["cubic"].attribute("blabla") = 912.5;
            cout << cu << endl;
            THEN("the same should be read back") {
                cube cur = file["cubic"];
                cube diff = cu - cur;
                diff = abs(diff);
                double diffs = diff.max();
                REQUIRE(0 == Approx(diffs));
            }
        }
    }
}
