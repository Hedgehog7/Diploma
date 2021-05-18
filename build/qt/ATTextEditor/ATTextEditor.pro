#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ATTextEditor
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/ATTextEditor

INCLUDEPATH += ../../../src/ATTextEditor ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += attexteditor.cpp

HEADERS += attexteditor.cpp attexteditor_global.h

LIBS += -L../libxml2 -lxml2 -L../libiconv -liconv -L../ATCore -lATCore -L../ATGUI -lATGUI -L../ATProject -lATProject
