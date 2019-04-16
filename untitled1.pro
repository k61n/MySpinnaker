QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = untitled1
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

QT_CONFIG -= no-pkg-config
CONFIG += c++11 \
        link_pkgconfig
PKGCONFIG += opencv4
#QMAKE_LFLAGS += -Wl,-rpath,"'\$$ORIGIN'" // to make it later distribute with libs in the executable folder
QMAKE_LFLAGS += -Wl,-rpath,"/home/user/opencv/debug/install/lib/"

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        myspinnaker.cpp

HEADERS += \
        mainwindow.h \
        myspinnaker.h

FORMS += \
        mainwindow.ui

INCLUDEPATH += "/usr/include/spinnaker/"
LIBS += -lSpinnaker
LIBS += -lGenApi_gcc540_v3_0
LIBS += -lGCBase_gcc540_v3_0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
