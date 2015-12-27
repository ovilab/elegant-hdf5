TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/hdf5/serial
LIBS += -lhdf5_cpp -lhdf5_serial
LIBS += -L../src/ -lh5cpp

include(../library_deployment.pri)

SOURCES += \
    tests.cpp
