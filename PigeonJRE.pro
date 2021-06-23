TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lzip

SOURCES += \
        class.cpp \
        frame.cpp \
        jre.cpp \
        main.cpp

HEADERS += \
    class.h \
    debug.h \
    frame.h \
    jre.h

DISTFILES += \
    .gdbinit \
    Add.class \
    Main.class
