#ifndef H5CPP_LOGGING
#define H5CPP_LOGGING

#include <ostream>
#include <iostream>

#ifdef H5CPP_USE_GLOG

#include <glog/logging.h>

#else

#ifndef H5CPP_DLL_EXPORT_FOR_WINDOWS
# if defined(_WIN32) && !defined(__CYGWIN__)
#   define H5CPP_DLL_EXPORT_FOR_WINDOWS  __declspec(dllimport)
# else
#   define H5CPP_DLL_EXPORT_FOR_WINDOWS
# endif
#endif

namespace elegant {
namespace hdf5 {

class H5CPP_DLL_EXPORT_FOR_WINDOWS DummyLog {
 public:
  DummyLog() { }
  void operator&(std::ostream&) { }
};
}
}

#define DLOG(severity) true ? (void)0 : elegant::hdf5::DummyLog() & std::cout
#define DVLOG(verboselevel) true ? (void)0 : elegant::hdf5::DummyLog() & std::cout

#endif

#endif // LOGGING

