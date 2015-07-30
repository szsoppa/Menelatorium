TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    comunicator.cpp \
    lamport.cpp \
    actor.cpp

HEADERS += \
    headers.h \
    comunicator.h \
    lamport.h \
    actor.h

