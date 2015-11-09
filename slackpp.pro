#-------------------------------------------------
#
# Project created by QtCreator 2015-10-23T10:44:49
#
#-------------------------------------------------

QT       += core gui websockets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = slackpp
TEMPLATE = app


SOURCES += main.cpp\
        slackui.cpp \
    roster.cpp \
    input.cpp \
    chat.cpp \
    slackclient.cpp

HEADERS  += slackui.h \
    roster.h \
    input.h \
    chat.h \
    slackclient.h

FORMS    += slackui.ui

CONFIG += c++14
