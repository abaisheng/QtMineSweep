#-------------------------------------------------
#
# Project created by QtCreator 2016-06-29T19:28:38
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QtMineSweep
TEMPLATE = app


SOURCES += main.cpp\
    game_model.cpp \
    game_window.cpp \
    customdialog.cpp


HEADERS  += \
    game_model.h \
    game_window.h \
    customdialog.h


FORMS    += maingamewindow.ui \
    customdialog.ui

RESOURCES += \
    resource.qrc

DISTFILES += \
    bugs.txt

RC_ICONS = res/mine.ico

OTHER_FILES += \
    bugs.txt
