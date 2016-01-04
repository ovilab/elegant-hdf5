#ifndef FILE_H
#define FILE_H

#include "logging.h"

#include <string>
#include <hdf5.h>
#include <vector>

#include "group.h"

namespace h5cpp {

class File : public Group
{
public:
    enum class OpenMode {
        ReadOnly,
        ReadWrite,
        Truncate
    };

    File(std::string fileName, File::OpenMode mode = File::OpenMode::ReadWrite);
    virtual ~File();

    void close();

private:
    std::string m_fileName;
};

}

#endif // FILE_H
