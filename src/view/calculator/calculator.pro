QT       += core gui printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    credit_calc.cpp \
    debit_calc.cpp \
    default_calc.cpp \
    graph.cpp \
    main.cpp \
    ../../model/stack.c \
    ../../model/string_utils.c \
    ../../model/calc_core.c \
    mainwindow.cpp \
    qcustomplot.cpp \
    utils.cpp

HEADERS += \
    ../../model/calc_core.h \
    ../../model/stack.h \
    ../../model/string_utils.h \
    credit_calc.h \
    debit_calc.h \
    default_calc.h \
    graph.h \
    mainwindow.h \
    qcustomplot.h \
    utils.h

FORMS += \
    credit_calc.ui \
    debit_calc.ui \
    default_calc.ui \
    graph.ui \
    mainwindow.ui

DESTDIR = ../../../../Calc
#MOC_DIR = build
#OBJECTS_DIR = build
#UI_DIR = build
#MAKEFILE = build/Makefile

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    res.qrc


