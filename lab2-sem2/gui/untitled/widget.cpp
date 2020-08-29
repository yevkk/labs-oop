#include "widget.h"
#include "ui_widget.h"
#include <QToolButton>
#include <QTabWidget>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    QToolButton *newTabButton = new QToolButton(this);
    ui->tabWidget->setCornerWidget(newTabButton);
    newTabButton->setCursor(Qt::ArrowCursor);
    newTabButton->setAutoRaise(true);
    newTabButton->setIcon(QIcon());
    QObject::connect(newTabButton, SIGNAL(clicked()), this, SLOT(newTab()));
    newTabButton->setToolTip(tr("Add page"));
}

void Widget::newTab(){
    ui->pushButton->setText("Hello");
}

Widget::~Widget()
{
    delete ui;
}

