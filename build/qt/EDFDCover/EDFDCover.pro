#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T15:01:20
#
#-------------------------------------------------

QT       -= core

QMAKE_CXXFLAGS += -fpermissive

CONFIG += c++11 static debug_and_release
TARGET = EDFDCover
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB

VPATH += ../../../src/edfd_cover/

INCLUDEPATH += ../../../src ../../../lib/src/libxml2/include ../../../src/edfd_cover ../../../src/planner_core/ ../../../include

SOURCES +=  EDFDCover.cpp PlannerSDPRepresentation.cpp \
    ../../../src/edfd_cover/APlan.cpp

HEADERS  += edfd_cover.h heuristic.h EDFDCover.h EDFDCoverInternal.h PlannerSDPRepresentation.h \
    ../../../src/edfd_cover/APlan.h

LIBS += -L../UCore -lUCore
LIBS += -L../libiconv -liconv -L../ATCore -lATCore -L../ATGUI -lATGUI -L../libxml2 -lxml2
