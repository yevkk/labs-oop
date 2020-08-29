#include <QToolButton>
#include <QTableWidget>
#include <QLabel>
#include <QWidget>


#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QDesktopWidget dw;
    setFixedSize(dw.width()*0.8,dw.height()*0.8);

    QToolButton *tb = new QToolButton();
    tb->setText("+");
    tb->setAutoRaise(true);
    connect(tb, SIGNAL(clicked()), this, SLOT(addTab()));

    ui->tabWidget->addTab(new QLabel("You can add tabs by pressing <b>\"+\"</b>"), QString());
    ui->tabWidget->tabBar()->setTabButton(0, QTabBar::RightSide, tb);
    ui->tabWidget->tabBar()->setTabsClosable(true);
    addTab();
    ui->tabWidget->setTabEnabled(0, true);
    connect(ui->tabWidget->tabBar(), SIGNAL(tabCloseRequested(int)), this, SLOT(closeTab(int)));
}

void MainWindow::closeTab(const int& index)
{
    if (index == -1) {
        return;
    }

    QWidget* tabItem = ui->tabWidget->widget(index);
    // Removes the tab at position index from this stack of widgets.
    // The page widget itself is not deleted.
    ui->tabWidget->removeTab(index);

    delete(tabItem);
    tabItem = nullptr;
}

MainWindow::~MainWindow()
{
    for(auto tab : buttons)
        delete tab;
    for(auto tab : widgets)
        delete tab;
    for(auto tab : layouts)
        delete tab;
    for(auto tab : tabs)
        delete tab;
    delete ui;
}

void MainWindow::addTab()
{
    static int number = 0;
    QLabel *tab = new QLabel(this);
    QString tabName = QString("Tab #%1").arg(++number);
    //tab->setText( QString("Inside %1").arg(tabName) );
    QGridLayout *centralLayout = new QGridLayout;
    auto widget = new drawingWindow();

    auto button = new QPushButton("OK");
    widgets.push_back(widget);
    widget->show();
    buttons.push_back(button);
    layouts.push_back(centralLayout);

    QObject::connect(button, SIGNAL(released()), widget->Ui()->drawWidget, SLOT(updateEvents()));

    centralLayout->addWidget(widget);
    centralLayout->addWidget(button);
    tab->setLayout(centralLayout);

    ui->tabWidget->insertTab( ui->tabWidget->count() - 1, tab, tabName);
    tabs.push_back(tab);
}
