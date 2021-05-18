#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T15:01:20
#
#-------------------------------------------------

QT       += core

CONFIG += c++11 static debug_and_release
CONFIG -= app_bundle
TARGET = ATLinker
TEMPLATE = app

DEFINES += ULTRA_STATIC STATIC_LIB

VPATH += ../../../src/ATLinker

INCLUDEPATH += ../../../src/ATLinker ../../../src ../../../lib/src/libxml2/include

SOURCES += main.cpp

HEADERS  += 

RESOURCES += res.qrc

LIBS += -L../ATProject -lATProject -L../ATCore -lATCore -L../libxml2 -lxml2 -L../libiconv -liconv

macx{
  QMAKE_LFLAGS_RPATH =
  QMAKE_LFLAGS = -Wl,-rpath,/Users/ultrablox/Qt5.8.0/5.8/clang_64/lib
}
