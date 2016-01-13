#ifndef DATASPACE_H
#define DATASPACE_H

#include <vector>

#include <hdf5.h>

namespace h5cpp {

class Dataspace
{
public:
    Dataspace(hid_t dataspaceID);
    Dataspace(const Dataspace& other) = delete;
    Dataspace(const Dataspace &&other);
    Dataspace& operator=(const Dataspace &other) = delete;
    Dataspace& operator=(const Dataspace &&other);

    ~Dataspace();

    bool isValid() const;

    std::vector<hsize_t> extents() const;
    int dimensionCount() const;

    hid_t id() const;
    void close();

    operator hid_t() const;
private:
    hid_t m_id;
};

} // namespace

#endif // DATASPACE_H
