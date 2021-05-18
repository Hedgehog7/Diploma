#-------------------------------------------------
#
# Project created by QtCreator 2016-05-13T15:01:20
#
#-------------------------------------------------

QT       += core gui widgets

CONFIG += c++11 static debug_and_release
#CONFIG -= app_bundle
TARGET = ATWorkbench
TEMPLATE = app

DEFINES += ULTRA_STATIC STATIC_LIB USE_LIBAVOID

VPATH += ../../../src/ATShell

INCLUDEPATH += ../../../src\
               ../../../src/ATShell\
               ../../../lib/include\
               ../../../lib/src\
               ../../../lib/src/libxml2/include\
               ../../../lib/src/libiconv/include

SOURCES += ACommandExecutor.cpp\
           AConsoleWidget.cpp\
           APluginsWidget.cpp\
           AProjectExplorer.cpp\
           AQNewFileDialog.cpp\
           AQNewProjectDialog.cpp\
           AQProjectTreeWidget.cpp\
           ATApplication.cpp\
           ATEnvironment.cpp\
           main.cpp\
           AProjectManager.cpp

HEADERS  += ACommandExecutor.h\
            AConsoleWidget.h\
            APluginsWidget.h\
            AProjectExplorer.h\
            AQNewFileDialog.h\
            AQNewProjectDialog.h\
            AQProjectTreeWidget.h\
            ATApplication.h\
            ATEnvironment.h

RESOURCES += ATEnvironment.qrc

FORMS += AConsoleWidget.ui\
         APluginsWidget.ui\
         AQNewFileDialog.ui\
         AQNewProjectDialog.ui\
         ATEnvironment.ui

LIBS += -L../libiconv\
        -liconv\
        -L../ATPlanner\
        -lATPlanner\
        -L../ATGUI\
        -lATGUI\
        -L../ATProject\
        -lATProject\
        -L../ATCore\
        -lATCore\
        -L../EDFDCover\
        -lEDFDCover\
        -L../libxml2\
        -lxml2\
        -L../UCore\
        -lUCore\
        -ldl\
        -lm
