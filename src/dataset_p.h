#ifndef DATASET_P_H
#define DATASET_P_H

#include "io/reader.h"
#include "io/writer.h"
#include "hdf5.h"

namespace elegant {
namespace hdf5 {

class Dataset;

class DatasetWriter : public Writer
{
public:
    DatasetWriter(Dataset* dataset, hid_t datatype);
    virtual void write(const void *buffer) override;
private:
    Dataset* m_dataset = nullptr;
    hid_t m_datatype;
};

class DatasetReader : public Reader
{
public:
    DatasetReader(hid_t dataset, hid_t datatype);
    virtual void read(void *buffer) override;
private:
    hid_t m_dataset;
    hid_t m_datatype;
};

}
}

#endif // DATASET_P_H
