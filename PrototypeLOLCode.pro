#-------------------------------------------------
#
# Project created by QtCreator 2015-10-20T20:31:44
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = PrototypeLOLCode
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    memorymanager.cpp \
    variables.cpp

OTHER_FILES += \
    test.lol

HEADERS += \
    InputFile.hpp \
    OutputFile.hpp \
    memorymanager.hpp \
    variables.hpp
