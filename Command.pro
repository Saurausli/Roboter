QT += core gui qml quick quickwidgets sql quickcontrols2 serialport
CONFIG += c++11
TEMPLATE = app

#DEFINES += RASPBERRYPI
#DEFINES += QT_DEPRECATED_WARNINGS
#LIBS += -L/usr/local/lib -lwiringPi
#INCLUDEPATH += /usr/local/include

srcPath = src

SOURCES += \
        $$srcPath/command.cpp \
        $$srcPath/consoleui.cpp \
        $$srcPath/doublejointmotor.cpp \
        $$srcPath/error.cpp \
        $$srcPath/joint.cpp \
        $$srcPath/main.cpp \
        $$srcPath/operation.cpp \
        $$srcPath/programm.cpp \
        $$srcPath/suboperation.cpp \
        $$srcPath/variable.cpp

HEADERS += \
    $$srcPath/command.h \
    $$srcPath/consoleui.h \
    $$srcPath/doublejointmotor.h \
    $$srcPath/error.h \
    $$srcPath/joint.h \
    $$srcPath/operation.h \
    $$srcPath/programm.h \
    $$srcPath/suboperation.h \
    $$srcPath/variable.h


# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target qml

RESOURCES += \
    qml/Ressourcen.qrc

