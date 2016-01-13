#include "../src/file.h"
#include "../src/object.h"
#include "../src/dataset.h"
#include "../src/attribute.h"

#define CATCH_CONFIG_RUNNER
#include <catch.hpp>

#include <iostream>

int main(int argc, char* const argv[])
{
#ifdef H5CPP_USE_GLOG
    google::InitGoogleLogging(argv[0]);
#endif
    int result = Catch::Session().run(argc, argv);
    return result;

}
