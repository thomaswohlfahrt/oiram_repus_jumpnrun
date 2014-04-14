TEMPLATE = app
#CONFIG += console
CONFIG -= qt

INCLUDEPATH = .
DEPENDPATH = N:\SDL-1.2.15\lib
LIBS += -LN:\SDL-1.2.15\lib
SOURCES += \
    main.cpp \
    item.cpp \
    environment.cpp \
    timer.cpp \
    application.cpp \
    logic.cpp \
    event.cpp \
    movingItem.cpp \
    player.cpp \
    map.cpp \
    editor.cpp

HEADERS += \
    item.h \
    environment.h \
    timer.h \
    constants.h \
    application.h \
    logic.h \
    event.h \
    movingItem.h \
    player.h \
    map.h \
    editor.h


LIBS += -lSDLmain \
    -lSDL \
    -lSDL_image \
    -lSDL_mixer \
    #-llibtiff-5 \
    -lSDL_ttf
