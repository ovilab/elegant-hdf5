#ifndef FILE_H
#define FILE_H

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

    File(std::string fileName, File::OpenMode mode);
    virtual ~File();
};

}

#endif // FILE_H
