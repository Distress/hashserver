#-------------------------------------------------
#
# Project created by QtCreator 2021-06-22T11:28:12
#
#-------------------------------------------------

QT       += network testlib

QT       -= gui

TARGET = hashsocket_ut
CONFIG += c++14
CONFIG   += console
CONFIG   -= app_bundle

CONFIG += testcase no_testcase_installs

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

INCLUDEPATH += ../../external \
        ../../src/server

SOURCES += \
        hashsocket_ut.cpp \
        $$files("../../src/server/*.cpp")
SOURCES -= ../../src/server/main.cpp

HEADERS += \
        $$files("../../src/server/*.h")
