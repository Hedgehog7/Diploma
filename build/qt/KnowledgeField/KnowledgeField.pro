QT       -= core gui

TARGET = KnowledgeField
CONFIG += c++11 static debug_and_release
TEMPLATE = lib

DEFINES += STATIC_LIB

VPATH += ../../../src/libs/KnowledgeField

INCLUDEPATH += ../../../src

SOURCES += knowledge_field_t.cpp

HEADERS += knowledge_field_t.h
