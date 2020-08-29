#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "drawingwidget.h"
#include "drawingwindow.h"
#include <QMainWindow>
#include <QDesktopWidget>



namespace Ui {
class MainWindow;
}

#include <QMainWindow>

class drawingWidget;
class drawingWindow;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void addTab();
    void closeTab(const int& index);

private:
    std::vector<QLabel*> tabs;
    std::vector<QWidget*> widgets;
    std::vector<QPushButton*> buttons;
    std::vector<QGridLayout*> layouts;

    Ui::MainWindow *ui;
    drawingWidget* widget;
};

#endif // MAINWINDOW_H
