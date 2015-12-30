#include "file.h"
#include "logging.h"

#include <hdf5.h>
#include <iostream>
#include <spdlog/spdlog.h>

using namespace std;

namespace h5cpp {

File::File(std::string fileName, File::OpenMode mode)
    : Group()
{
    ifstream f(fileName);
    bool good = f.good();
    f.close();
    if (good && mode != OpenMode::Truncate) {
        switch(mode) {
        case OpenMode::ReadOnly:
            m_id = H5Fopen(fileName.c_str(), H5F_ACC_RDONLY, H5P_DEFAULT);
            break;
        case OpenMode::ReadWrite:
            m_id = H5Fopen(fileName.c_str(), H5F_ACC_RDWR, H5P_DEFAULT);
            break;
        default:
            break;
        }
        DLOG(INFO) << "Opened file with ID: " << m_id;
    } else {
        m_id = H5Fcreate(fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
        DLOG(INFO) << "Created file with ID: " << m_id;
    }
}

File::~File()
{
    close();
}

void File::close()
{
    if(m_id > 0) {
        DLOG(INFO) << "Closing file " << m_id;
        H5Fclose(m_id);
        m_id = 0;
    }
}

}
