#-------------------------------------------------
#
# Project created by QtCreator 2016-08-01T22:36:48
#
#-------------------------------------------------

QT       += core gui widgets sql

TARGET = KMPZ
CONFIG += c++11 shared debug_and_release
TEMPLATE = lib

DEFINES += ULTRA_STATIC LIBXML_STATIC STATIC_LIB 

VPATH += ../../../src/plugins/KMPZ

INCLUDEPATH += ../../../src/editors/KMPZ ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES +=  kmpz.cpp \
            YOSDIInterpreter/YOSDIInterpreter.cpp \
            YOSDIInterpreter/YOSDI_forms_manager.cpp \
            YOSDIInterpreter/scenario_loader.cpp \
            pz_creator/pz_creator.cpp \
            pz_creator/condition_stack.cpp \
            dictionary/temporal_dictionary.cpp \
            ../../libs/KnowledgeField/knowledge_field_t.cpp \
            forms/common/task_type_definition_form.cpp \
            forms/common/temporal_form.cpp \
            forms/common/task_type_alert_dialog.cpp \
            forms/common/debug_pz.cpp \
            forms/diagnostics/question_answer_form.cpp \
            forms/diagnostics/clearifying_information_form.cpp \
            forms/diagnostics/conclusion_form.cpp \
            forms/diagnostics/has_other_answer.cpp \
            forms/diagnostics/can_make_conclusion_form.cpp \
                        target_selector/target_selector.cpp\
            atn/atn.cpp

HEADERS +=  kmpz_global.h \
            kmpz.h \
            YOSDIInterpreter/IInterpreter.h \
            YOSDIInterpreter/YOSDIInterpreter.h \
            YOSDIInterpreter/YOSDI_forms_manager.h \
            YOSDIInterpreter/scenario.h \
            YOSDIInterpreter/scenario_loader.h \
            pz_creator/pz_creator.h \
            pz_creator/condition_stack.h \
            dictionary/temporal_dictionary.h \
            ../../libs/KnowledgeField/knowledge_field_t.h\
            forms/YOSDI_form.h \
            forms/common/task_type_definition_form.h \
            forms/common/temporal_form.h \
            forms/common/task_type_alert_dialog.h \
            forms/common/debug_pz.h \
            forms/diagnostics/question_answer_form.h \
            forms/diagnostics/clearifying_information_form.h \
            forms/diagnostics/conclusion_form.h \
            forms/diagnostics/has_other_answer.h \
            forms/diagnostics/can_make_conclusion_form.h \
                        target_selector/target_selector.h\
            atn/atn.h

FORMS +=    forms/common/task_type_definition_form.ui \
            forms/common/temporal_form.ui \
            forms/common/task_type_alert_dialog.ui \
            forms/common/debug_pz.ui \
            forms/diagnostics/question_answer_form.ui \
            forms/diagnostics/clearifying_information_form.ui \
            forms/diagnostics/conclusion_form.ui \
            forms/diagnostics/has_other_answer.ui \
            forms/diagnostics/can_make_conclusion_form.ui

LIBS += -L../ATProject -lATProject -L../ATGUI -lATGUI -L../ATCore -lATCore -L../libiconv -liconv -L../libxml2 -lxml2
