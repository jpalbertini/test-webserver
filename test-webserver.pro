QT += core
QT -= gui

CONFIG += c++11

TARGET = test-webserver
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += src/main.cpp \
            3rdparties/civetweb/src/civetweb.c \
            3rdparties/civetweb/src/CivetServer.cpp \
    src/webserver.cpp

INCLUDEPATH += 3rdparties/civetweb/include inc

HEADERS += 3rdparties/civetweb/include/civetweb.h \
            3rdparties/civetweb/include/CivetServer.h \
    inc/webserver.h \
    inc/ressourceaccess.h


DEFINES += QT_DEPRECATED_WARNINGS

win32 {
    LIBS += -lws2_32 -lComdlg32 -lUser32 -lShell32 -lAdvapi32
}

RESOURCES += \
    res/res.qrc
