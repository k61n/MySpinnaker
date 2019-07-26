QT       += core gui concurrent

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myspinnaker
TEMPLATE = app

DEFINES += QT_DEPRECATED_WARNINGS

CONFIG += c++11

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        myspinnaker.cpp

HEADERS += \
        mainwindow.h \
        myspinnaker.h

FORMS += \
        mainwindow.ui

INCLUDEPATH +=  $$PWD/spinnaker/include \
                $$PWD/opencv/include/opencv4 \

win32 {
}
unix {
    LIBS += -L$$PWD/spinnaker/lib/linux_x64/ \
            -lSpinnaker -lGenApi_gcc540_v3_0 -lGCBase_gcc540_v3_0
    LIBS += -L$$PWD/opencv/lib/linux_x64/ \
            -lopencv_core -lopencv_imgproc
}

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
