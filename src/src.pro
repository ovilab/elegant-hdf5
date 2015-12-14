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
    object.cpp

HEADERS += \
    conversions.h \
    dataset.h \
    file.h \
    group.h \
    object.h

DESTDIR = $$h5cpp_builddir/dist/h5cpp
HEADERS_DESTDIR = $$h5cpp_builddir/dist/include
