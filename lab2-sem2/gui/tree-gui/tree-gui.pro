QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = tree-gui
TEMPLATE = app


SOURCES += main.cpp\
        drawingtree.cpp \
        drawingwidget.cpp \
        drawingwindow.cpp \
        mainwindow.cpp

HEADERS  += mainwindow.h \
    drawingtree.h \
    drawingwidget.h \
    drawingwindow.h

FORMS    += mainwindow.ui \
    drawingwindow.ui
