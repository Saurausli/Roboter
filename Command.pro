TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        command.cpp \
        doublejointmotor.cpp \
        joint.cpp \
        main.cpp

HEADERS += \
    command.h \
    doublejointmotor.h \
    joint.h
