QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += /usr/lib/gcc/x86_64-linux-gnu/9/include

LIBS += -lusb-1.0 -llogsys-drv

SOURCES += \
    backthread.cpp \
    bitbangwidget.cpp \
    confwidget.cpp \
    i2cwidget.cpp \
    main.cpp \
    mainwindow.cpp \
    spiwidget.cpp \
    usartwidget.cpp

HEADERS += \
    backthread.h \
    bitbangwidget.h \
    confwidget.h \
    i2cwidget.h \
    logsys/common.h \
    logsys/control.h \
    logsys/jconf.h \
    logsys/serio.h \
    logsys/status.h \
    logsys/usb.h \
    logsys/usb.private.h \
    mainwindow.h \
    spiwidget.h \
    usartwidget.h

FORMS += \
    bitbangwidget.ui \
    confwidget.ui \
    i2cwidget.ui \
    mainwindow.ui \
    spiwidget.ui \
    usartwidget.ui

TRANSLATIONS += \
    Logsys-GUI_en_150.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
