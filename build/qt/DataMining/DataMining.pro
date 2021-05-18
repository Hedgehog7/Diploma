QT       += core gui sql widgets

TARGET = DataMining
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB

VPATH += ../../../src/plugins/DataMining

INCLUDEPATH += ../../../src ../../../src/libs/KnowledgeField

SOURCES += dataminingplugin.cpp\
    mainwindow.cpp \
    database_wrapper.cpp \
    tree_node_t.cpp \
    tree_t.cpp \
    model_t.cpp \

HEADERS  += mainwindow.h\
    database_wrapper.h \
    tree_node_t.h \
    tree_t.h \
    model_t.h \
    dataminingplugin.h

FORMS    += mainwindow.ui

LIBS += -L../ATProject -lATProject -L../ATGUI -lATGUI -L../ATCore -lATCore -L../libiconv -liconv -L../libxml2 -lxml2 -L../KnowledgeField -lKnowledgeField
