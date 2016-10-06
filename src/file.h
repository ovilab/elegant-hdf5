#ifndef FILE_H
#define FILE_H

#include "utils/logging.h"

#include <string>
#include <hdf5.h>
#include <vector>

#include "group.h"

namespace elegant {
namespace hdf5 {

class File : public Group
{
public:
    enum class OpenMode {
        ReadOnly,
        ReadWrite,
        Truncate
    };

    File(std::string fileName,
         File::OpenMode mode = File::OpenMode::ReadWrite,
         File::ConversionFlags conversionFlags = ConversionFlags::NoFlags);
    virtual ~File();

    void close();

private:
    std::string m_fileName;
};

} // namespace
} // namespace

#endif // FILE_H
