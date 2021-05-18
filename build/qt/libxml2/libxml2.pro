#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

#QMAKE_MAC_SDK = macosx10.12

QT       += core

TARGET = xml2
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += STATIC_LIB LIBICONV_PLUG

VPATH += ../../../lib/src/libxml2/src

INCLUDEPATH += ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

LIBS += -L../libiconv -liconv

SOURCES += DOCBparser.c \
           hash.c \
           parser.c \
           threads.c \
           trionan.c \
           valid.c \
           xmlmodule.c \
           xmlstring.c \
           HTMLparser.c \
           c14n.c \
           legacy.c \
           parserInternals.c \
           xinclude.c \
           xmlreader.c \
           xmlunicode.c \
           HTMLtree.c \
           catalog.c \
           encoding.c \
           pattern.c \
           tree.c \
           xlink.c \
           xmlregexp.c \
           xmlwriter.c \
           SAX.c \
           chvalid.c \
           entities.c \
           list.c \
           relaxng.c \
           #trio.c \
           #triostr.c \
           xmlIO.c \
           xmlsave.c \
           xpath.c \
           SAX2.c \
           debugXML.c \
           error.c \
           xmllint.c \
           xmlschemas.c \
           xpointer.c \
           buf.c \
           dict.c \
           globals.c \
           schematron.c \
           uri.c  \
           xmlmemory.c \
           xmlschemastypes.c \
           xzlib.c

# nanohttp.c \ nanoftp.c \
