#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = ATVerifier
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/plugins/ATVerifier

INCLUDEPATH += ../../../src/editors/ATVerifier ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += ATVerifier.cpp #\
#            verfunc.cpp \
#    verificator.cpp \
#    verification.cpp

HEADERS += ATVerifier.h #\
 #           verfunc.h \
 #   verificator.h \
 #   verification.h

LIBS += -L../libxml2 -lxml2 -L../libiconv -liconv -L../ATCore -lATCore -L../ATGUI -lATGUI

#FORMS += \
#    verificator.ui \
#    verification.ui
