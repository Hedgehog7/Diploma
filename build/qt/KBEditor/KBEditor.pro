#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = KBEditor
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/editors/KBEditor

INCLUDEPATH += ../../../src/editors/KBEditor ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += AKBEditor.cpp \
    ../../../src/editors/KBEditor/KBAttributeScrollHeader.cpp \
    ../../../src/editors/KBEditor/KBAttributeScrollItem.cpp \
    ../../../src/editors/KBEditor/KBCreateObjectDialog.cpp \
    ../../../src/editors/KBEditor/KBObject.cpp \
    ../../../src/editors/KBEditor/KBCreateIntervalDialog.cpp \
    ../../../src/editors/KBEditor/KBInterval.cpp \
    ../../../src/editors/KBEditor/KBAttribute.cpp \
    ../../../src/editors/KBEditor/Formula.cpp \
    ../../../src/editors/KBEditor/KnowledgeBase.cpp \
    ../../../src/editors/KBEditor/FormulaWidget.cpp \
    ../../../src/editors/KBEditor/KBCreateEventDialog.cpp \
    ../../../src/editors/KBEditor/KBEvent.cpp \
    ../../../src/editors/KBEditor/RuleArg.cpp \
    ../../../src/editors/KBEditor/KBRule.cpp \
    ../../../src/editors/KBEditor/KBNode.cpp \
    ../../../src/editors/KBEditor/KBCreateRuleDialog.cpp \
    ../../../src/editors/KBEditor/KBStatement.cpp \
    ../../../src/editors/KBEditor/KBElementWidget.cpp \
    ../../../src/editors/KBEditor/KBStatementDialog.cpp \
    ../../../src/editors/KBEditor/KBStatementTemporalDialog.cpp \
    ../../../src/editors/KBEditor/KBStatementTemporal.cpp \
    ../../../src/editors/KBEditor/KBStatementWidget.cpp \
    ../../../src/editors/KBEditor/KBStatementTemporalWidget.cpp \
    ../../../src/editors/KBEditor/RuleWidget.cpp

HEADERS += AKBEditor.h kbeditor_global.h \
    ../../../src/editors/KBEditor/KBAttributeScrollHeader.h \
    ../../../src/editors/KBEditor/KBAttributeScrollItem.h \
    ../../../src/editors/KBEditor/KBCreateObjectDialog.h \
    ../../../src/editors/KBEditor/KBObject.h \
    ../../../src/editors/KBEditor/KBCreateIntervalDialog.h \
    ../../../src/editors/KBEditor/KBInterval.h \
    ../../../src/editors/KBEditor/KBAttribute.h \
    ../../../src/editors/KBEditor/Formula.h \
    ../../../src/editors/KBEditor/KnowledgeBase.h \
    ../../../src/editors/KBEditor/FormulaWidget.h \
    ../../../src/editors/KBEditor/KBCreateEventDialog.h \
    ../../../src/editors/KBEditor/KBEvent.h \
    ../../../src/editors/KBEditor/RuleArg.h \
    ../../../src/editors/KBEditor/KBRule.h \
    ../../../src/editors/KBEditor/KBNode.h \
    ../../../src/editors/KBEditor/KBCreateRuleDialog.h \
    ../../../src/editors/KBEditor/KBStatement.h \
    ../../../src/editors/KBEditor/KBElementWidget.h \
    ../../../src/editors/KBEditor/KBStatementDialog.h \
    ../../../src/editors/KBEditor/KBStatementTemporalDialog.h \
    ../../../src/editors/KBEditor/KBStatementTemporal.h \
    ../../../src/editors/KBEditor/KBStatementWidget.h \
    ../../../src/editors/KBEditor/KBStatementTemporalWidget.h \
    ../../../src/editors/KBEditor/RuleWidget.h

LIBS += -L../ATGUI -lATGUI -L../ATCore -lATCore -L../libxml2 -lxml2 -L../libiconv -liconv
CONFIG += console
