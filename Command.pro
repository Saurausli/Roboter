QT += core gui qml quick quickwidgets sql quickcontrols2 serialport
CONFIG += c++11
TEMPLATE = app

DEFINES += RASPBERRYPI
DEFINES += QT_DEPRECATED_WARNINGS
LIBS += -L/usr/local/lib -lwiringPi
INCLUDEPATH += /usr/local/include

SOURCES += \
        command.cpp \
        consoleui.cpp \
        doublejointmotor.cpp \
        joint.cpp \
        main.cpp

HEADERS += \
    command.h \
    consoleui.h \
    doublejointmotor.h \
    joint.h

RESOURCES += \
    Ressourcen.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target qml
