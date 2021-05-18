QT += core gui widgets sql

TEMPLATE = lib
DEFINES += RUC1_LIBRARY

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
VPATH += ../../../src/tutor_plugins/RUC2
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0
INCLUDEPATH +=  ../../../src ../../../lib/src ../../../lib/src/libxml2/include ../../../lib/src/libiconv/include

SOURCES += \
    mainwidget.cpp \
    ruc2.cpp

HEADERS += \
    RUC2_global.h \
    mainwidget.h \
    ruc2.h

# Default rules for deployment.
unix {
    TARGET = ruc2
}
!isEmpty(target.path): INSTALLS += target

LIBS += -L../ATProject -lATProject -L../ATGUI -lATGUI -L../ATCore -lATCore -L../libiconv -liconv -L../libxml2 -lxml2

FORMS += \
    mainwidget.ui
