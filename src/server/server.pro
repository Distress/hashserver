include(../../global.pri)

QT -= gui
QT += network

CONFIG += c++14 warn_on
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -Woverloaded-virtual -Wpedantic -Wextra \
    -Wfloat-equal -Wshadow -Wundef

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = $$PROJECT_NAME
target.path = $$PROJECT_PREFIX/bin
INSTALLS += target

INCLUDEPATH += \
    ../../external

SOURCES += \
        main.cpp \
    hashsocket.cpp \
    xxhashwrapper.cpp \
    weightedthreadpool.cpp \
    hashdaemon.cpp \
    hashserver.cpp

HEADERS += \
    hashsocket.h \
    xxhashwrapper.h \
    weightedthreadpool.h \
    hashdaemon.h \
    hashserver.h
