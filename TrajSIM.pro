QT       += core gui charts

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Plot.cpp \
    PlotZone.cpp \
    configuration.cpp \
    configurationModel.cpp \
    graphicviewsurcharged.cpp \
    main.cpp \
    mainwindow.cpp \
    rocket.cpp \
    timemanager.cpp \
    worldmanager.cpp

HEADERS += \
    Plot.h \
    PlotZone.h \
    Tabs.h \
    configuration.h \
    configurationModel.h \
    graphicviewsurcharged.h \
    mainwindow.h \
    rocket.h \
    timemanager.h \
    worldmanager.h

FORMS += \
    Plot.ui \
    PlotZone.ui \
    configuration.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
