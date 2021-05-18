QT       += core gui sql widgets

TARGET = Merger
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB

VPATH += ../../../src/plugins/Merger

INCLUDEPATH += ../../../src ../../../src/libs/KnowledgeField

SOURCES += mergerplugin.cpp \
    mainwindow.cpp \
    merger_t.cpp \
    tester_t.cpp \

HEADERS  += mergerplugin.h \
    mainwindow.h \
    merger_t.h \
    tester_t.h \

FORMS    += mainwindow.ui

LIBS += -L../ATProject -lATProject -L../ATGUI -lATGUI -L../ATCore -lATCore -L../libiconv -liconv -L../libxml2 -lxml2 -L../KnowledgeField -lKnowledgeField
