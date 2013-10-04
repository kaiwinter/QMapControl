include(../../QMapControl.pri)
QT+=network
DEPENDPATH += src
MOC_DIR = tmp
OBJECTS_DIR = obj
DESTDIR = ../bin
TARGET = Citymap

# Input
SOURCES += src/citymap.cpp \
           src/main.cpp \
           src/dialogs.cpp
HEADERS += src/citymap.h \
           src/dialogs.h
