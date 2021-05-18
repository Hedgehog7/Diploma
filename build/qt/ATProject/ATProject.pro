#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       -= core

TARGET = ATProject
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/ATProject

INCLUDEPATH += ../../../src/ATProject ../../../src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += AProject.cpp AProjectNode.cpp AProjectTemplate.cpp ADocumentRef.cpp

HEADERS += AProjectTemplate.h ADocumentRef.h AProject.h

LIBS += -L../libxml2 -lxml2 -L../ATCore -lATCore

