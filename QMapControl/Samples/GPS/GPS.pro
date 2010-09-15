include(../../QMapControl.pri)
DEPENDPATH += src
MOC_DIR = tmp
OBJECTS_DIR = obj
DESTDIR = ../bin
TARGET = GPS

# Input
SOURCES += src/gps.cpp \
src/main.cpp \
src/gps_neo.cpp
HEADERS += src/gps.h \
src/gps_neo.h
