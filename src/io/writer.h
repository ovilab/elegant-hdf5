#ifndef WRITER_H
#define WRITER_H

namespace elegant {
namespace hdf5 {

class Writer
{
public:
    virtual void write(const void *buffer) = 0;
};

} // namespace
} // namespace

#endif // WRITER_H
