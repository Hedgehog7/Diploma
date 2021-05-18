#-------------------------------------------------
#
# Project created by QtCreator 2017-03-11T20:17:54
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = tst_edfdcovertest
CONFIG   += console c++11
CONFIG   -= app_bundle

QMAKE_CXXFLAGS += -fpermissive

TEMPLATE = app

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

VPATH = ../../../../src/tests

SOURCES += tst_edfdcovertest.cpp \
    test_edfd_cover.cpp \
    test_helpers.cpp \
    test_helpers.h \
    transition_system_helpers.h

DEFINES += SRCDIR=\\\"$$PWD/\\\"

INCLUDEPATH += ../../../../src ../../../../lib/src/libxml2/include ../../../../src/planner_core/ ../../../../include

LIBS += -L../../libiconv -liconv -L../../ATGUI -lATGUI -L../../ATProject -lATProject -L../../EDFDCover -lEDFDCover -L../../ATPlanner -lATPlanner -L../../ATCore -lATCore -L../../libxml2 -lxml2 -L../../UCore -lUCore -ldl -lm
