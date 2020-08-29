#include "drawingwindow.h"
#include "ui_drawingwindow.h"

drawingWindow::drawingWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::drawingWindow)
{
    ui->setupUi(this);
}

drawingWindow::~drawingWindow()
{
    delete ui;
}
