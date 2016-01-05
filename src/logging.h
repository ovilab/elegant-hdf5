#ifndef H5CPP_LOGGING
#define H5CPP_LOGGING

#ifdef H5CPP_USE_GLOG

#include <glog/logging.h>

#else

#define DLOG(severity) 0 && std::cout
#define DVLOG(verboselevel) 0 && std::cout

#endif

#endif // LOGGING

