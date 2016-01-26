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
    errorhelper.cpp

HEADERS += \
    dataset.h \
    file.h \
    group.h \
    object.h \
    attribute.h \
    logging.h \
    typehelper.h \
    demangle.h \
    h5cpp/armadillo \
    converters/native-converters.h \
    converters/armadillo-converters.h \
    converters/std-converters.h \
    datatype.h \
    dataspace.h \
    elegant/hdf5/Attribute \
    elegant/hdf5/Dataset \
    elegant/hdf5/File \
    elegant/hdf5/Group \
    elegant/hdf5/Object \
    elegant/hdf5/hdf5 \
    errorhelper.h

release {
    !debug {
        DEFINES += NDEBUG
    }
}

DISTFILES += \
    ../README.md
