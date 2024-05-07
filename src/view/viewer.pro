QT       += core gui
QT += openglwidgets #in comment for qmake!

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
#CONFIG += c11

#CONFIG += console   # delete!

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ../controller/controller.cpp \
    ../main.cpp \
    ../model/command.cpp \
    ../model/model.cpp \
    ../model/observable.cpp \
    ../model/observer.cpp \
    ../model/snapshot.cpp \
    mainwindow.cpp \
    myglwidget.cpp


HEADERS += \
    ../controller/controller.h \
    ../model/command.h \
    ../model/model.h \
    ../model/movecommand.h \
    ../model/observable.h \
    ../model/observer.h \
    ../model/rotatecommand.h \
    ../model/scalingcommand.h \
    ../model/snapshot.h \
    mainwindow.h \
    myglwidget.h \
    transformation.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
