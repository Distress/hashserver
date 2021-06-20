QT -= gui
QT += network

CONFIG += c++14
CONFIG -= app_bundle

QMAKE_CXXFLAGS += -Woverloaded-virtual -Wpedantic -Wextra \
    -Wfloat-equal -Wshadow -Wundef

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += \
    ../../external/xxhash

SOURCES += \
        main.cpp \
    hashserver.cpp \
    workerpool.cpp \
    workerengine.cpp \
    worker.cpp \
    hashsocket.cpp \
    xxhashwrapper.cpp

HEADERS += \
    hashserver.h \
    workerpool.h \
    workerengine.h \
    worker.h \
    hashsocket.h \
    xxhashwrapper.h
