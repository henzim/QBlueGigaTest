#-------------------------------------------------
#
# Project created by QtCreator 2017-11-02T13:34:24
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# UART
LIBS += -lsetupapi

TARGET = QBlueGigaTest
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

DEFINES += LOG_FUNCTIONCALLS

SOURCES += \
        main.cpp \
        blewindow.cpp \
    BluegigaUART.cpp \
    bluegigablehandler.cpp \
    logger.cpp \
    cmd_def.cpp \
    stubs.c

HEADERS += \
        blewindow.h \
    BluegigaUART.h \
    bluegigablehandler.h \
    logger.h \
    apitypes.h \
    cmd_def.h

FORMS += \
        blewindow.ui
