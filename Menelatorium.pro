TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lamport.cpp \
    actor.cpp \
    menel.cpp \
    communicator.cpp

HEADERS += \
    headers.h \
    lamport.h \
    actor.h \
    menel.h \
    communicator.h

