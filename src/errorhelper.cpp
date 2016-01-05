#include "errorhelper.h"
#include <sstream>
#include <iomanip>

using namespace std;

ErrorHelper::ErrorHelper()
{
    // Take over error handling
    H5Eget_auto(H5E_DEFAULT, &m_oldFunction, &m_oldClientData);
    H5Eset_auto(H5E_DEFAULT, NULL, NULL);
}

ErrorHelper::~ErrorHelper()
{
    close();
}

std::string ErrorHelper::walk()
{
    stringstream errorStream;
    H5Ewalk2(H5E_DEFAULT, H5E_WALK_UPWARD, &streamBuilder, &errorStream);
    H5Eclear(H5E_DEFAULT);
    return errorStream.str();
}

void ErrorHelper::close()
{
    // Restore error handling
    H5Eset_auto(H5E_DEFAULT, m_oldFunction, m_oldClientData);
}

herr_t ErrorHelper::streamBuilder(unsigned n, const H5E_error2_t *error, void *clientData) {
    (void)n;
    *(stringstream*)(clientData) << "#" << setfill('0') << setw(3) << n << setw(0) << " " << error->func_name
                                 << ": " << error->desc << endl;
    herr_t result = 0;
    return result;
}
