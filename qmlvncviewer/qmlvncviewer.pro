TEMPLATE = lib
TARGET = qmlvncviewer
QT += qml quick
CONFIG += qt plugin c++11

TARGET = $$qtLibraryTarget($$TARGET)
uri = com.rooyeetone.vnc

win32 {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    !contains(QMAKE_TARGET.arch, x86_64) {
        ARCH = Win32## Windows x86 (32bit) specific build here
    } else {
        ARCH = x64 ## Windows x64 (64bit) specific build here
    }

    CONFIG(debug, debug|release): BUILD = debug

    CONFIG(release, debug|release){
        BUILD=release
    }
}

BASE_DESTDIR = $$PWD/../../build/vncviewer/$$BUILD

DESTDIR = $$BASE_DESTDIR\imports\com\rooyeetone\vnc

updateqm.commands = $$PWD\update.bat $$DESTDIR $$PWD
updateqm.target = updateqm

QMAKE_EXTRA_TARGETS += updateqm

PRE_TARGETDEPS += updateqm


# Input
SOURCES += \
    src/qmlvncviewer_plugin.cpp \
    src/qquickvncviewer.cpp \
    src/qvncvieweradapter.cpp \
    src/viewervncauthhandler.cpp \
    src/vnclogger.cpp

HEADERS += \
    src/qmlvncviewer_plugin.h \
    src/qquickvncviewer.h \
    src/qvncvieweradapter.h \
    src/viewervncauthhandler.h \
    src/vnclogger.h

DISTFILES = qmldir

!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir
unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}


DEFINES += _UNICODE

INCLUDEPATH += $$PWD/../tightvnc

#message($$PWD/tightvnc/$${BUILD})

win32-msvc*:LIBS += -lkernel32 -luser32 -lgdi32 -lole32 -lshell32 -ladvapi32 -limm32 -lwinspool
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/client-config-lib
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/config-lib
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/file-lib
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/ft-client-lib
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/ft-common
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/gui
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/io-lib
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/log-writer
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/network
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/region
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/rfb
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/thread
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/util
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/viewer-core
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/tcp-dispatcher
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/win-system
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/zlib
win32-msvc*:LIBS += -l$$PWD/../tightvnc/$${BUILD}/libjpeg

