#-------------------------------------------------
#
# Project created by QtCreator 2015-03-31T11:31:23
#
#-------------------------------------------------

QT       += network sql xml
QT       -= gui

TARGET = mediasrv-qtplugin-user
TEMPLATE = lib

DEFINES += MEDIASRVQTPLUGIN_LIBRARY

SOURCES += mediasrvqtplugin.cpp \
    sessionnotifyobj.cpp

HEADERS += mediasrvqtplugin.h\
        mediasrv-qtplugin_global.h \
    sessionnotifyobj.h

unix {

    UNAME = $$system(uname)
    BUILD_DIR = $$system(pwd)
    DESTDIR=../../../bin/$$UNAME/

    target.path = /usr/lib
    INSTALLS += target

    UNAME = $$system(uname)

    LIBS += -L../../../bin/$$UNAME/
    LIBS += -L/usr/local/lib

    LIBS +=  -lcpputil
    LIBS +=  -lrde
    LIBS +=  -ladapi


}

win32 {
LIBS += -L"c:/mylib/msvc2013"
}

INCLUDEPATH += ../../include
