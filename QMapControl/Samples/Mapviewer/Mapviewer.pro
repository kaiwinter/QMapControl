include(../../QMapControl.pri)
QT+=network
DEPENDPATH += src
MOC_DIR = tmp
OBJECTS_DIR = obj
DESTDIR = ../bin
TARGET = Mapviewer

# Input
HEADERS += src/mapviewer.h
SOURCES += src/main.cpp src/mapviewer.cpp

