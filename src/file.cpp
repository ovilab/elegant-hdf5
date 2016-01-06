#include "file.h"
#include "logging.h"
#include "errorhelper.h"

#include <hdf5.h>
#include <iostream>
#include <fstream>

using namespace std;

namespace h5cpp {

File::File(string fileName, File::OpenMode mode)
    : Group()
    , m_fileName(fileName)
{
    ErrorHelper errorHelper;

    // Open file
    ifstream f(fileName);
    bool good = f.good();
    f.close();
    DVLOG(1) << "Opening file '" << fileName << "'";
    if (good && mode != OpenMode::Truncate) {
        switch(mode) {
        case OpenMode::ReadOnly:
            DVLOG(1) << "in readonly mode";
            m_id = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
            break;
        case OpenMode::ReadWrite:
            DVLOG(1) << "in readwrite mode";
            m_id = H5Fopen(fileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            break;
        default:
            break;
        }
        if(m_id < 1) {
            stringstream error;
            error << "Could not open file '" << fileName << "'. " << endl;
            error << errorHelper.walk();
            throw(runtime_error(error.str()));
        }
        DVLOG(1) << "with ID: " << m_id;
    } else if(mode != OpenMode::ReadOnly) {
        m_id = H5Fcreate(fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        if(m_id < 1) {
            stringstream error;
            error << "Could not create file '" << fileName << "'. " << endl;
            error << errorHelper.walk();
            throw(runtime_error(error.str()));
        }
        DVLOG(1) << "in truncate/create mode with ID: " << m_id;
    } else {
        stringstream error;
        error << "Could not open file '" << fileName << "'. OpenMode was ReadOnly.";
        throw runtime_error(error.str());
    }
}

File::~File()
{
    close();
}

void File::close()
{
    if(m_id > 0) {
        DVLOG(1) << "Closing file '" << m_fileName << "' with ID " << m_id;
        H5Fclose(m_id);
        m_id = 0;
    }
}

}
