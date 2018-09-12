#-------------------------------------------------
#
# Project created by QtCreator 2015-02-24T20:25:47
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HelpDeskAdmin
TEMPLATE = app


SOURCES += main.cpp\
        helpdeskadmin.cpp \
    widget.cpp

HEADERS  += \
    helpdeskadmin.h \
    widget.h

FORMS    +=

RESOURCES += \
    myresources.qrc

win32:RC_ICONS += custom_icon.ico
