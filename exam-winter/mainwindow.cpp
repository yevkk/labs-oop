#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "qcustomplot/qcustomplot.h"

#include <QMessageBox>
#include <QInputDialog>
#include <QStatusBar>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
   : QMainWindow(parent)
   , ui(new Ui::MainWindow) {
   ui->setupUi(this);

   ui->productsTreeWidget->setHeaderLabels(QStringList() << "Product Name" << "Required amount");
   ui->productsTreeWidget->setColumnWidth(0, 200);

   ui->productRawTable->setHorizontalHeaderLabels(QStringList() << "Product Name" << "Required amount");
   ui->productRawTable->setColumnWidth(0, 120);

   ui->productUsageTable->setHorizontalHeaderLabels(QStringList() << "Product Name" << "Required amount");
   ui->productUsageTable->setColumnWidth(0, 120);

   ui->lastSimulationTable->setHorizontalHeaderLabels(QStringList() << "Year" << "Export" << "Import" << "Balance");
   ui->lastSimulationTable->setColumnWidth(0, 80);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::reset() {
   ui->productsTreeWidget->clear();

   ui->productNameValueLabel->clear();

   ui->productRawTable->clearContents();
   ui->productRawTable->setRowCount(0);

   ui->productUsageTable->clearContents();
   ui->productUsageTable->setRowCount(0);

   ui->producedInList->clear();

   countries.clear();
   products.clear();
   simulations.clear();

   ui->stackedWidget->setCurrentIndex(0);
}

void MainWindow::updateStatusBar() {
   statusBar()->showMessage("Products number: " + QString::number(products.size()) +
                            ";   Countries number: " + QString::number(countries.size()) + ";");
}

void MainWindow::visualizeSimulation(Simulation simulation) {
   if (ui->simulationScrollAreaContents->layout()){
      QLayoutItem* item;
      while (item = ui->simulationScrollAreaContents->layout()->takeAt(0)) {
         delete item->widget();
      }
   }

   delete ui->simulationScrollAreaContents->layout();
   ui->simulationScrollAreaContents->setLayout(new QVBoxLayout());

   auto data = simulation.getData();
   if (data.empty()) return;

   int products_number = data[0].size();
   //int first_year = simulation.getStartingYear();

   int i = 0;
   //for (int i = 0; i < products_number; i++){
      QLabel* label = new QLabel(QString::fromStdString(data[0][i].getProduct()->getName()));
      label->setFont(QFont("Segoi UI", 10));

      QCustomPlot* plot = new QCustomPlot;

      plot->legend->setVisible(true);
      plot->legend->setFont(QFont("Segoi UI", 10));

      QPen pen;
      QStringList lineNames;
      lineNames << "Imported" << "Exported" << "Balance";
      QVector<QColor> colors = {QColor(70, 90, 190), QColor(240, 150, 20), QColor(240, 25, 55)};

      for (int k = 0; k < 3; k++) {
         plot->addGraph();
         pen.setColor(colors[k]);
         plot->graph()->setPen(pen);
         plot->graph()->setName(lineNames[k]);
         plot->graph()->setLineStyle(QCPGraph::lsLine);
         plot->graph()->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, 5));
         QVector<double> x, y;
         for(unsigned j = 0; j < data.size(); j++){
            x << j;
            switch (k){
               case 0: {
                  y << data[j][i].getImport();
                  break;
               }
               case 1: {
                  y << data[j][i].getExport();
                  break;
               }
               case 2: {
                  y << data[j][i].balance();
                  break;
               }
            }
         }

         plot->graph()->setData(x, y);
         plot->graph()->rescaleAxes(true);

         plot->yAxis->scaleRange(1.1, plot->yAxis->range().center());
         plot->xAxis->scaleRange(1.1, plot->xAxis->range().center());

         plot->xAxis->setTicks(true);
         plot->yAxis->setTicks(true);
         plot->xAxis->setTickLabels(true);
         plot->yAxis->setTickLabels(true);
         plot->resize(plot->width(), 500);
      }

      QGroupBox* gb = new QGroupBox;
      QVBoxLayout* lyt = new QVBoxLayout;
      lyt->addWidget(label);
      lyt->addWidget(plot);
      gb->setLayout(lyt);

      ui->simulationScrollAreaContents->layout()->addWidget(gb);
   //}

}

void MainWindow::productsTreeAddChildren(std::shared_ptr<RawProduct> product, QTreeWidgetItem* tree_item) {
   for(auto &e:product->getRawList()) {
      QTreeWidgetItem* itm = new QTreeWidgetItem(tree_item);
      itm->setText(0, QString::fromStdString(e.first->getName()));
      itm->setText(1, QString::number(e.second));
      productsTreeAddChildren(e.first, itm);
   }
}

void MainWindow::on_addRandomProductBtn_clicked() {
   auto tmp = randomProduct(products);
   products.push_back(tmp);

   QTreeWidgetItem* itm = new QTreeWidgetItem(ui->productsTreeWidget);
   itm->setText(0, QString::fromStdString(tmp->getName()));

   productsTreeAddChildren(tmp, itm);

   updateStatusBar();
}

void MainWindow::on_productsTreeWidget_itemClicked(QTreeWidgetItem *item) {
   unsigned i = 0;
   while (QString::fromStdString(products[i]->getName()) != item->text(0))
      i++;

   ui->productNameValueLabel->setText(QString::fromStdString(products[i]->getName()));

   ui->productRawTable->clearContents();
   ui->productUsageTable->clearContents();
   ui->productUsageTable->setRowCount(0);
   ui->producedInList->clear();
   ui->lastSimulationTable->clearContents();
   ui->lastSimulationTable->setRowCount(0);

   ui->productRawTable->setRowCount(products[i]->getRawList().size());
   for (unsigned  j = 0; j < products[i]->getRawList().size(); j++){
      auto name_itm = new QTableWidgetItem(QString::fromStdString(products[i]->getRawList()[j].first->getName()));
      ui->productRawTable->setItem(j, 0, name_itm);

      auto amount_itm = new QTableWidgetItem(QString::number(products[i]->getRawList()[j].second));
      ui->productRawTable->setItem(j, 1, amount_itm);
   }

   for (unsigned j = 0; j < products.size(); j++){
      if (i == j) continue;

      for(unsigned k = 0; k < products[j]->getRawList().size(); k++) {
         if (products[i] == products[j]->getRawList()[k].first){
            int rows = ui->productUsageTable->rowCount();
            ui->productUsageTable->setRowCount(rows + 1);

            auto name_itm = new QTableWidgetItem(QString::fromStdString(products[j]->getName()));
            ui->productUsageTable->setItem(rows, 0, name_itm);

            auto amount_itm = new QTableWidgetItem(QString::number(products[j]->getRawList()[k].second));
            ui->productUsageTable->setItem(rows, 1, amount_itm);
         }
      }
   }

   for (auto &country:countries) {
      for (auto &product:country->getProductionList()) {
            if (product.first == products[i])
               ui->producedInList->addItem(QString::fromStdString(country->getName()));
      }
   }

   if (simulations.empty()) return;
   auto simulation = simulations.back().getData();
   int first_year = simulations.back().getStartingYear();

   if (i < simulation[0].size() || !simulation.empty()) {
      ui->lastSimulationTable->setRowCount(simulation.size());
      for (unsigned  j = 0; j < simulation.size(); j++){
         auto year_itm = new QTableWidgetItem(QString::number(first_year + j));
         ui->lastSimulationTable->setItem(j, 0, year_itm);

         auto export_itm = new QTableWidgetItem(QString::number(simulation[j][i].getExport()));
         ui->lastSimulationTable->setItem(j, 1, export_itm);

         auto import_itm = new QTableWidgetItem(QString::number(simulation[j][i].getImport()));
         ui->lastSimulationTable->setItem(j, 2, import_itm);

         auto balance_itm = new QTableWidgetItem(QString::number(simulation[j][i].balance()));
         ui->lastSimulationTable->setItem(j, 3, balance_itm);
      }
   }
}

void MainWindow::on_actionGenerate_countries_triggered() {
   int number = QInputDialog::getInt(0, "Input", "Countries number:", QLineEdit::Normal, 0);

   for (int i = 0; i < number; i++){
      auto tmp = randomCountry(products);
      countries.push_back(tmp);
   }

   updateStatusBar();
}

void MainWindow::on_actionClear_everything_triggered() {
    reset();
}

void MainWindow::on_actionRun_simulation_triggered() {
   int first_year = QInputDialog::getInt(0, "Input", "Start year:", QLineEdit::Normal, 2000);
   int years_number = QInputDialog::getInt(0, "Input", "Years number:", QLineEdit::Normal, 2);

    simulations.emplace_back(first_year, years_number, products, countries);

    visualizeSimulation(simulations.back());
    ui->stackedWidget->setCurrentIndex(1);
}

void MainWindow::on_returnButton_clicked() {
    ui->stackedWidget->setCurrentIndex(0);
}
