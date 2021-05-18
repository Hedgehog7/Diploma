QT       += core gui sql widgets

TARGET = Protocols
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB

VPATH += ../../../src/plugins/Protocols

INCLUDEPATH += ../../../src ../../../src/libs/KnowledgeField

SOURCES += protocols.cpp \
    mainwindow.cpp \
    treeitem.cpp \
    treeitemedit.cpp \
    treemodel.cpp \
    treemodeledit.cpp \
    knowledgeField.cpp \

HEADERS  += protocols.h \
    mainwindow.h \
    knowledgeField.h \
    treeitem.h \
    treeitemedit.h \
    treemodel.h \
    treemodeledit.h \

FORMS    += mainwindow.ui

LIBS += -L../ATProject -lATProject -L../ATGUI -lATGUI -L../ATCore -lATCore -L../libiconv -liconv -L../libxml2 -lxml2 -L../KnowledgeField -lKnowledgeField
