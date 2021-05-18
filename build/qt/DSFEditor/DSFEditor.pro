#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = DSFEditor
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/editors/DSFEditor

INCLUDEPATH += ../../../src/editors/EDFDEditor ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += DSFEditor.cpp

HEADERS += DSFEditor.h dsfeditor_global.h

FORMS += DSFEditor.ui

LIBS += -L../libxml2 -lxml2 -L../libiconv -liconv -L../ATCore -lATCore -L../ATGUI -lATGUI -L../ATProject -lATProject
