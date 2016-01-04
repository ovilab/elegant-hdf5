#include "file.h"
#include "logging.h"

#include <hdf5.h>
#include <iostream>

using namespace std;

namespace h5cpp {

File::File(std::string fileName, File::OpenMode mode)
    : Group()
    , m_fileName(fileName)
{
    ifstream f(fileName);
    bool good = f.good();
    f.close();
    DLOG(INFO) << "Opening file '" << fileName << "'";
    if (good && mode != OpenMode::Truncate) {
        switch(mode) {
        case OpenMode::ReadOnly:
            DLOG(INFO) << "in readonly mode";
            m_id = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
            break;
        case OpenMode::ReadWrite:
            DLOG(INFO) << "in readwrite mode";
            m_id = H5Fopen(fileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            break;
        default:
            break;
        }
        DLOG(INFO) << "with ID: " << m_id;
    } else {
        m_id = H5Fcreate(fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        DLOG(INFO) << "in truncate/create mode with ID: " << m_id;
    }
}

File::~File()
{
    close();
}

void File::close()
{
    if(m_id > 0) {
        DLOG(INFO) << "Closing file '" << m_fileName << "' with ID " << m_id;
        H5Fclose(m_id);
        m_id = 0;
    }
}

}
