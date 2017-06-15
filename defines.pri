win32-msvc* {
    DEFINES += "WINVER=0x0501"#determines the minimum platform SDK required to build application(_WIN32_WINNT_WINXP-minnum value.)
    DEFINES += "_WIN32_WINNT=0x0501"
    QMAKE_CXXFLAGS += /D "_USING_V110_SDK71_"#support for xp
    contains(DEFINES,WIN64) {
        greaterThan(QT_MINOR_VERSION, 2){
            QMAKE_SUBSYSTEM_SUFFIX=",5.02"
        }else{
            QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.02"
        }
    } else {
        greaterThan(QT_MINOR_VERSION, 2){
            QMAKE_SUBSYSTEM_SUFFIX=",5.01"
        }else{
            QMAKE_LFLAGS += /SUBSYSTEM:WINDOWS",5.01"
        }
    }
}

win32{
    COPY = copy /y
    MKDIR = mkdir
}else{
    COPY = cp
    MKDIR = mkdir -p
}
defineTest(copyToDestdir){
    files = $$1
    DSTDIR = $$2

    for(FILE, files) {
        DDIR = $$DSTDIR
#message("VacantCopyFun:" $$FILE $$DDIR)

        # Replace slashes in paths with backslashes for Windows
        win32:FILE ~= s,/,\\,g
        win32:DDIR ~= s,/,\\,g

        QMAKE_POST_LINK += $$QMAKE_COPY $$quote($$FILE) $$quote($$DDIR) $$escape_expand(\\n\\t)
    }
    export(QMAKE_POST_LINK)
}

defineTest(makeDir){
    DSTDIR = $$1
    DDIR = $$DSTDIR
    !exists($$DDIR){
        win32:DDIR ~= s,/,\\,g
        system($$MKDIR $$quote($$DDIR) $$escape_expand(\\n\\t))
    }
}

win32 {
    DEFINES += _CRT_SECURE_NO_WARNINGS
    !contains(QMAKE_TARGET.arch, x86_64) {
        #message("win32 build")
        ARCH = Win32## Windows x86 (32bit) specific build here
    } else {
        #message("x64 build")
        ARCH = x64 ## Windows x64 (64bit) specific build here
    }

    CONFIG(debug, debug|release): BUILD = debug

    CONFIG(release, debug|release){
        BUILD=release
        #DEFINES += QT_NO_DEBUG_OUTPUT
        #DEFINES += QT_NO_DEBUG_OUTPUT QT_NO_WARNING_OUTPUT QT_NO_DEBUG_STREAM
        QMAKE_CFLAGS_RELEASE += -zi
        QMAKE_LFLAGS_RELEASE += /DEBUG
    }

    LIBS += -lkernel32
    INCLUDEPATH +=$$PWD/../sdk/includes/breakpad
}

#set dest directory path.
DESTDIR = $$PWD/../../build/$$BUILD
RyMessengerTarget=RooyeeMessenger
RyLoggerTarget=qxtlogger
RY_DESTDIR = $$DESTDIR

# workaround for #849: see https://bugreports.qt-project.org/browse/QTBUG-23196
mocinclude.CONFIG *= fix_target

RY_VERSION = 1.8.6
unix: VERSION = $$RY_VERSION
DEFINES *= RY_VERSION=\\\"""$$RY_VERSION"\\\""

#d_debug_build = $$(DEBUG_BUILD)
#equals(d_debug_build, "true") { CONFIG += debug }

DEFINES *= QT_NO_URL_CAST_FROM_STRING
#DEFINES *= QT_USE_QSTRINGBUILDER#QStringBuilder prepare.

CONFIG(debug, debug|release): DEFINES *= RY_DEBUG_BUILD


win32-msvc* {
    DEFINES *= W7API
    LIBS += User32.lib Ole32.lib Shell32.lib ShlWapi.lib Gdi32.lib ComCtl32.lib
}

# QtDBus not available on Mac
mac: DEFINES *= DISABLE_DBUS

!mac:unix {
    binary_folder = /usr/bin
    library_folder = /usr/lib
    arch_lib_path = /usr/lib/$${QT_ARCH}-linux-gnu
    exists($$arch_lib_path): library_folder = $$arch_lib_path
    data_folder = /usr/share/RY
    launcher_folder = /usr/share/applications
    icon_folder = /usr/share/pixmaps
    hicolor_folder = /usr/share/icons/hicolor

    !equals(d_prefix, "") {
        binary_folder = "$$d_prefix"/bin
        library_folder = "$$d_prefix"/lib
        data_folder = "$$d_prefix"/share/RY
        launcher_folder = "$$d_prefix"/share/applications
        icon_folder = "$$d_prefix"/share/pixmaps
        hicolor_folder = "$$d_prefix"/share/icons/hicolor
    }

    !equals(d_use_lib_path, ""):library_folder = $$d_use_lib_path

    DEFINES *= USE_LIBPATH=\\\"""$$library_folder"\\\""
    DEFINES *= USE_DATADIR=\\\"""$$data_folder"\\\""

    # Git revision
    rev = $$system(cd ../ && sh $$PWD/../scripts/getrevision.sh)
    !equals(rev, ""): DEFINES *= GIT_REVISION=\\\"""$$rev"\\\""

    # Define QZ_WS_X11 even with Qt5 (but only when building for X11)
    !contains(DEFINES, NO_X11) DEFINES *= QZ_WS_X11
}

isEmpty(QMAKE_LRELEASE) {#translation release.
    win32|os2:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]\\lrelease.exe
    else:QMAKE_LRELEASE = $$[QT_INSTALL_BINS]/lrelease

    # Try to use lrelease from PATH
    unix:!exists($$QMAKE_LRELEASE) {
        isEqual(QT_MAJOR_VERSION, 4): QMAKE_LRELEASE = lrelease-qt4
        else: QMAKE_LRELEASE = lrelease
    }
}

isEmpty(QMAKE_LFLAGS_RPATH) {#This is used on Unix platforms only
    QMAKE_LFLAGS_RPATH = -Wl,-rpath,
}
