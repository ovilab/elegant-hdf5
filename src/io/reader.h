#ifndef READER_H
#define READER_H

namespace elegant {
namespace hdf5 {

class Reader
{
public:
    virtual void read(void *buffer) = 0;
};

} // namespace
} // namespace

#endif // READER_H
