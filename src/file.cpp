#include "file.h"

#include <hdf5.h>
#include <iostream>

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
#ifdef H5CPP_VERBOSE
        cerr << "Opened file with ID: " << m_id << endl;
#endif
    } else {
        m_id = H5Fcreate(fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
#ifdef H5CPP_VERBOSE
        cerr << "Created file with ID: " << m_id << endl;
#endif
    }
}

File::~File()
{
    close();
}

void File::close()
{
    if(m_id > 0) {
#ifdef H5CPP_VERBOSE
        cerr << "Closing file " << m_id << endl;
#endif
        H5Fclose(m_id);
        m_id = 0;
    }
}

}
