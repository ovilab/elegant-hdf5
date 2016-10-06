#ifndef TEMP_H
#define TEMP_H

#include "utils/logging.h"
#include "utils/demangle.h"

#include <hdf5.h>
#include <string>
#include <iostream>
#include <sstream>
#include <typeinfo>
#include <vector>

namespace elegant {
namespace hdf5 {

class Dataset;
class Group;
class File;
class Attribute;

class Object
{
public:
    enum class Type {
        Invalid = -1,
        File,
        Group,
        Datatype,
        Dataspace,
        Dataset,
        Attribute
    };

    enum ConversionFlags {
        NoFlags = 0b00000000,
        EqualDimensionCount = 0b00000001,
        GreaterThanOrEqualDimensionCount = 0b00000010,
        EqualTypes = 0b00000100,

        InheritedFlags = 0b10000000
    };

    Object(ConversionFlags conversionFlags = ConversionFlags::NoFlags);
    Object(hid_t id, hid_t parentID, std::string name, ConversionFlags inheritedConversionFlags);

    enum class CopyMode {
        OpenOnCopy,
        DontOpenOnCopy
    };

    Object(const Object &other, CopyMode mode = CopyMode::OpenOnCopy);
    Object& operator=(const Object &other);
    Object& operator=(const Dataset &other);
    Object& operator=(const Group &other);

    // TODO implement move operators

    template<typename T>
    void operator=(const T& other); // TODO: Consider operator chaining support

    virtual ~Object();

    const std::string &name() const;
    Type type() const;
    hid_t id() const;

#ifndef ELEGANT_HDF5_NO_USER_DEFINED_CONVERSION_OPERATORS
    template<typename T>
    operator T() const;
#endif

    template<typename T>
    T value(ConversionFlags mode = ConversionFlags::InheritedFlags) const;

    bool isValid() const;
    bool isDataset() const;
    bool isGroup() const;
    bool isNonExistingNamed() const;

    static Type fromHdf5Type(H5I_type_t hType);
    static H5I_type_t toHdf5Type(Object::Type hType);
    hid_t parentID() const;

    std::vector<Attribute> attributes() const;
    std::vector<std::string> attributeKeys() const;
    Attribute operator ()(std::string key) const;
    Attribute attribute(std::string key) const;
    bool hasAttribute(std::string name) const;

    void close();

    friend std::ostream& operator<<(std::ostream&, const Object&);

protected:
    void constructFromOther(const Object &other);

    hid_t m_id = 0;
    hid_t m_parentId = 0;
    std::string m_name;
    ConversionFlags m_inheritedConversionFlags = ConversionFlags::NoFlags;

private:
};

} // namespace h5cpp
}

#include "object.tpp"

#endif // TEMP_H
