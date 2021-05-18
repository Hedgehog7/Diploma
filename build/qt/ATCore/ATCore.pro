#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       -= core

TARGET = ATCore
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/ATCore

INCLUDEPATH += ../../../src ../../../src/ATcore ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += ADocument.cpp        AError.cpp           ALogManager.cpp      ANamedObject.cpp \
architecture/AArchElement.cpp          architecture/AArchElementFactory.cpp   architecture/AArchElementGroup.cpp     architecture/AArchLink.cpp             architecture/AArchitectureDocument.cpp architecture/APIKInterface.cpp \
edfd/AScheme.cpp       edfd/DFDAnchor.cpp     edfd/DFDConnection.cpp edfd/DFDElement.cpp    edfd/EDFDDocument.cpp \
documents/AKBDocument.cpp \
utils/geometry.cpp utils/helpers.cpp utils/xml_helpers.cpp check.cpp \
    ../../../src/ATCore/sdp/SDP.cpp \
    ../../../src/ATCore/sdp/SDPDocument.cpp


HEADERS += ADelegatedObject.h ADocument.h        AError.h           ALogManager.h      ANamedObject.h     ASingleton.h       config.h check.h \
    ../../../src/ATCore/sdp/SDP.h \
    ../../../src/ATCore/sdp/SDPDocument.h


LIBS += -L../libxml2 -lxml2 -L../libiconv -liconv 

