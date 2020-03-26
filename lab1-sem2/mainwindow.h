#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "product.h"
#include "country.h"
#include "simulation.h"

#include <QMainWindow>
#include <QTreeWidget>

#include <memory>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_addRandomProductBtn_clicked();

    void on_productsTreeWidget_itemClicked(QTreeWidgetItem *item);

    void on_actionGenerate_countries_triggered();

    void on_actionClear_everything_triggered();

    void on_actionRun_simulation_triggered();

    void on_returnButton_clicked();

private:
    void productsTreeAddChildren(std::shared_ptr<RawProduct> product, QTreeWidgetItem* tree_item);

    void reset();

    void updateStatusBar();

    void visualizeSimulation(Simulation simulation);

    Ui::MainWindow *ui;

    std::vector<std::shared_ptr<RawProduct>> products;

    std::vector<std::shared_ptr<Country>> countries;

    std::vector<Simulation> simulations;
};
#endif // MAINWINDOW_H
