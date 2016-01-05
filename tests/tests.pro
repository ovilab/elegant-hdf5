TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/hdf5/serial
INCLUDEPATH += ../libs/Catch/include

LIBS += -lhdf5_cpp -lhdf5_serial
LIBS += -L../src/ -lh5cpp

#include(../library_deployment.pri)

INCLUDEPATH += ../src
LIBS += -lh5cpp -L../src

SOURCES += \
    tests.cpp \
    attribute-tests.cpp \
    armadillo-tests.cpp \
    file-tests.cpp \
    group-tests.cpp \
    std-tests.cpp \
    sandbox-tests.cpp \
    dataset-tests.cpp

CONFIG(debug, debug|release) {
    DEFINES += H5CPP_USE_GLOG
    LIBS += -lglog
} else {
}
