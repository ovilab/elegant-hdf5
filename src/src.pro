TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = elegant_hdf5

INCLUDEPATH += /usr/include/hdf5/serial

LIBS += -lhdf5_serial

SOURCES += \
    dataset.cpp \
    file.cpp \
    group.cpp \
    object.cpp \
    attribute.cpp \
    datatype.cpp \
    dataspace.cpp \
    errorhelper.cpp \

HEADERS += \
    dataset.h \
    file.h \
    group.h \
    object.h \
    attribute.h \
    h5cpp/armadillo \
    converters/native-converters.h \
    converters/armadillo-converters.h \
    converters/std-converters.h \
    datatype.h \
    dataspace.h \
    utils/demangle.h \
    utils/errorhelper.h \
    utils/logging.h \
    object.tpp \
    attribute.tpp \
    dataset.tpp \
    dataset_p.h \
    io/reader.h \
    io/writer.h \
    io/typehelper.h \

# exported headers
HEADERS += \
    elegant/hdf5/Attribute \
    elegant/hdf5/Dataset \
    elegant/hdf5/File \
    elegant/hdf5/Group \
    elegant/hdf5/Object \
    elegant/hdf5/hdf5 \

release {
    !debug {
        DEFINES += NDEBUG
    }
}

DISTFILES += \
    ../README.md
