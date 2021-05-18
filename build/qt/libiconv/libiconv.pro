#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core

TARGET = iconv
CONFIG += static debug_and_release
TEMPLATE = lib

DEFINES += STATIC_LIB ENABLE_RELOCATABLE=1 IN_LIBRARY NO_XMALLOC set_relocation_prefix=libiconv_set_relocation_prefix relocate=libiconv_relocate HAVE_CONFIG_H USING_STATIC_LIBICONV LIBICONV_PLUG

VPATH += ../../../lib/src/libiconv/src

INCLUDEPATH += ../../../lib/src/libiconv/include

SOURCES += localcharset.c relocatable.c iconv.c