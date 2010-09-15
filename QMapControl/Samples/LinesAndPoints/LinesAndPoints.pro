include(../../QMapControl.pri)
QT+=network
DEPENDPATH += src
MOC_DIR = tmp
OBJECTS_DIR = obj
DESTDIR = ../bin
TARGET = Linesandpoints

# Input
HEADERS += src/LinesAndPoints.h
SOURCES += src/LinesAndPoints.cpp src/main.cpp

