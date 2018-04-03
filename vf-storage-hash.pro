#-------------------------------------------------
#
# Project created by QtCreator 2014-11-13T10:51:19
#
#-------------------------------------------------
TEMPLATE = lib

#dependencies
VEIN_DEP_EVENT = 1
VEIN_DEP_COMP = 1
VEIN_DEP_HELPER = 1

HEADERS +=\
    vein-hash_global.h \
    vs_veinhash.h

public_headers.files = $$HEADERS

exists( ../../vein-framework.pri ) {
  include(../../vein-framework.pri)
}

QT       -= gui

TARGET = vein-hash

DEFINES += VEINHASH_LIBRARY

SOURCES += \
    vs_veinhash.cpp

unix {
    target.path = /usr/lib
    INSTALLS += target
}


QMAKE_CXXFLAGS += -Wall -Wfloat-equal -std=c++0x
