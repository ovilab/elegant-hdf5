TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/include/hdf5/serial
LIBS += -lhdf5_serial

INCLUDEPATH += ../src
LIBS += -lelegant_hdf5 -L../src

SOURCES += main.cpp

