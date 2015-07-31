TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    lamport.cpp \
    actor.cpp \
    menel.cpp \
    communicator.cpp \
    packet.cpp

HEADERS += \
    headers.h \
    lamport.h \
    actor.h \
    menel.h \
    communicator.h \
    constants.h \
    packet.h

