include(../../QMapControl.pri)
QT+=network
DEPENDPATH += src
MOC_DIR = tmp
OBJECTS_DIR = obj
DESTDIR = ../bin
TARGET = Multidemo

# Input
HEADERS += src/multidemo.h src/gps_modul.h
SOURCES += src/multidemo.cpp src/main.cpp src/gps_modul.cpp
