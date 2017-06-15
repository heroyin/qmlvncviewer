include($$PWD/../defines.pri)

TEMPLATE = app

QT += qml quick quick-private gui-private
CONFIG += c++11

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

QMAKE_CXXFLAGS_RELEASE = -GL -Zi -Od -MD
QMAKE_CXXFLAGS += /MP


BASE_DESTDIR = $$PWD/../../build/vncviewer/$$BUILD

DESTDIR = $$BASE_DESTDIR

SOURCES += main.cpp \

RESOURCES += qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Default rules for deployment.
include(../deployment.pri)



