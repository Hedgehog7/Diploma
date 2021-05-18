#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = EDFDEditor
CONFIG += c++11 plugin debug_and_release
TEMPLATE = lib

DEFINES += LIBXML_STATIC EDFDEDITOR_LIB

VPATH += ../../../src/EDFDEditor

INCLUDEPATH += ../../../src/EDFDEditor ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += ADatastreamDialog.cpp     ADatastreamWidget.cpp     ADrawWidget.cpp           AWorkspaceWidget.cpp      DFDGraphicsConnection.cpp DFDGraphicsElement.cpp    EDFDEditor.cpp

HEADERS += ADatastreamDialog.h     ADatastreamWidget.h     ADrawWidget.h           AWorkspaceWidget.h      DFDGraphicsConnection.h DFDGraphicsElement.h    EDFDEditor.h            edfdeditor_global.h

FORMS += ADatastreamDialog.ui EDFDEditor.ui

LIBS += -L../libiconv -liconv -L../ATCore -lATCore -L../ATGUI -lATGUI -L../libxml2 -lxml2


