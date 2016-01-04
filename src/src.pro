TEMPLATE = lib
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

TARGET = h5cpp

INCLUDEPATH += /usr/include/hdf5/serial

LIBS += -lhdf5_serial

SOURCES += \
    dataset.cpp \
    file.cpp \
    group.cpp \
    object.cpp \
    attribute.cpp

HEADERS += \
    conversions.h \
    dataset.h \
    file.h \
    group.h \
    object.h \
    attribute.h \
    utils.h \
    logging.h \
    h5cpp/Dataset \
    h5cpp/File \
    h5cpp/Group \
    h5cpp/Object \
    h5cpp/h5cpp

release {
    !debug {
        DEFINES += NDEBUG
    }
}
