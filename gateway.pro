#-------------------------------------------------
#
# Project created by QtCreator 2014-05-13T00:00:53
#
#-------------------------------------------------

QT       += core
QT       -= gui

include(../Logger4Qt/Logger4Qt.pri)
include(../xPL4Qt/xPL4Qt.pri)
include(../zigbus/zigbus.pri)

CONFIG   += console
CONFIG   -= app_bundle
TEMPLATE = app

#MOC_DIR = ../../.build/gateway/moc
#INCLUDEPATH += $$MOC_DIR
#OBJECTS_DIR = ../../.build/gateway/obj
DESTDIR = ../../bin

OBJECTS_DIR = ./build/obj
MOC_DIR = ./build/moc
INCLUDEPATH += ./build/moc

CONFIG(debug, debug|release) {
    TARGET = gatewayd
}
else {
    TARGET = gateway
}


SOURCES += main.cpp \
    ZigbusGateway.cpp \
    Zigbus_xPLService.cpp \
    ModuleMessageTranslator.cpp

HEADERS += \
    ZigbusGateway.h \
    Zigbus_xPLService.h \
    ModuleMessageTranslator.h

OTHER_FILES += \
    tests/numeric_output.xpl
