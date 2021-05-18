#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       -= core

TARGET = ATGUI
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB

VPATH += ../../../src/ATGUI

INCLUDEPATH += ../../../src/ATGUI ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += AEditor.cpp ATGUI.cpp APlugin.cpp APluginManager.cpp ATaskExecutionWindow.cpp

HEADERS += AEditor.h      ATGUI.h        atgui_global.h config.h ATaskExecutionWindow.h APluginManager.h


LIBS += -L../libxml2 -lxml2

