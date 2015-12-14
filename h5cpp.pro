TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/hdf5/serial
LIBS += -lhdf5_cpp -lhdf5_serial

SOURCES += \
    src/dataset.cpp \
    src/file.cpp \
    src/group.cpp \
    src/object.cpp \
    tests/tests.cpp

HEADERS += \
    src/conversions.h \
    src/dataset.h \
    src/file.h \
    src/group.h \
    src/object.h

