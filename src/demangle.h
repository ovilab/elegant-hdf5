#ifndef DEMANGLE_H
#define DEMANGLE_H

#include <cxxabi.h>
#include <typeinfo>
#include <iostream>
#include <string>
#include <memory>
#include <cstdlib>

inline std::string demangle(const char* mangled)
{
      int status;
      std::unique_ptr<char[], void (*)(void*)> result(abi::__cxa_demangle(mangled, 0, 0, &status), std::free);
      if(!result.get()) {
          return "demangle error occurred";
      }
      return std::string(result.get());
}

#endif // DEMANGLE_H

