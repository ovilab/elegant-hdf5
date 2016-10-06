TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/hdf5/serial
INCLUDEPATH += ../libs/Catch/include

LIBS += -lhdf5_cpp -lhdf5_serial

INCLUDEPATH += ../src
LIBS += -L../src -lelegant_hdf5

SOURCES += \
    tests.cpp \
    attribute-tests.cpp \
    armadillo-tests.cpp \
    file-tests.cpp \
    datatype-tests.cpp \
    group-tests.cpp \
    std-tests.cpp \
    dataset-tests.cpp \
    armadillo-attribute-tests.cpp \
    noconversion-tests.cpp \
    sandbox-tests.cpp

CONFIG(debug, debug|release) {
    DEFINES += H5CPP_USE_GLOG
    LIBS += -lglog
} else {
}
