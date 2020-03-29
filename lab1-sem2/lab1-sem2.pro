CONFIG += c++11

CONFIG += testsConfig

SOURCES += \
    country.cpp \
    product.cpp \
    product_ie.cpp \
    simulation.cpp \

HEADERS += \
    country.h \
    product.h \
    product_ie.h \
    random.h \
    simulation.h \

mainConfig {
QT       += core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
DEFINES += QT_DEPRECATED_WARNINGS

TARGET = main

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    qcustomplot/qcustomplot.cpp

HEADERS += \
    mainwindow.h \
    qcustomplot/qcustomplot.h

FORMS += \
    mainwindow.ui

#RC_ICONS = icon.ico

RESOURCES += \
    source.qrc
}

testsConfig {
TARGET = tests

SOURCES += \
    tests/main.cpp \
    tests/tests.cpp

HEADERS += tests/catch.hpp
}


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target


