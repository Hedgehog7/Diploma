#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ATPlanner
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/ATPlanner

INCLUDEPATH += ../../../src/ATPlanner ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += APlannerWidget.cpp AStartArchitectureGenerator.cpp ATPlanner.cpp ADiagramProcessor.cpp

HEADERS += APlannerWidget.h AStartArchitectureGenerator.h ATPlanner.h config.h ADiagramProcessor.h

FORMS += APlannerWidget.ui

LIBS += -L../libxml2 -lxml2 -L../EDFDCover -lEDFDCover
