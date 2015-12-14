#include "file.h"

#include <hdf5.h>
#include <iostream>

using namespace std;

namespace h5cpp {

File::File(std::string fileName, File::OpenMode mode)
    : Group(0, 0, "/")
{
    int cOpenMode = H5F_ACC_RDONLY;
    switch(mode) {
    case OpenMode::ReadOnly:
        cOpenMode = H5F_ACC_RDONLY;
        break;
    case OpenMode::ReadWrite:
        cOpenMode = H5F_ACC_RDWR;
        break;
    default:
        break;
    }

    ifstream f(fileName);
    if (f.good()) {
        f.close();
        m_id = H5Fopen(fileName.c_str(), cOpenMode, H5P_DEFAULT);
    } else {
        f.close();
        m_id = H5Fcreate(fileName.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);
    }
}

File::~File()
{
    if(m_id != 0) {
        H5Fclose(m_id);
        m_id = 0;
    }
}

}
