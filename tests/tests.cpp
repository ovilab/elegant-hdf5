#include "../src/file.h"
#include "../src/object.h"
#include "../src/dataset.h"
#include "../src/attribute.h"

#include <hdf5.h>
#include <hdf5_hl.h>

//#define CATCH_CONFIG_MAIN
#define CATCH_CONFIG_RUNNER
#include <catch.hpp>
#include <iostream>
#include <glog/logging.h>

using namespace std;
using namespace h5cpp;
using namespace arma;

int main( int argc, char* const argv[] )
{
    google::InitGoogleLogging(argv[0]);
    int result = Catch::Session().run(argc, argv);
    return result;
}
