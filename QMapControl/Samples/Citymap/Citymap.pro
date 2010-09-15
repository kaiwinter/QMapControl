include(../../QMapControl.pri)
QT+=network
DEPENDPATH += src
MOC_DIR = tmp
OBJECTS_DIR = obj
DESTDIR = ../bin
TARGET = Citymap

# Input
SOURCES += citymap.cpp \
           main.cpp \
 dialogs.cpp
HEADERS += citymap.h \
 dialogs.h
