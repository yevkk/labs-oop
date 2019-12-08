#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "graphs/graph_adj_str.h"
#include "models.h"
#include "xmlparse.h"
#include "previewhighlighter.h"

#include <QDebug>
#include <QCheckBox>
#include <QFileDialog>
#include <QMessageBox>
#include <iostream>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , current_file("")
    , saved(1)
{
    ui->setupUi(this);

    new PreviewHighlighter(ui->xmlPreviewText->document());

    features_model = std::allocate_shared<FeatureModels>(std::allocator<FeatureModels>());
    ui->featuresTableView->setModel(features_model->getModel());
    ui->featuresTableView->setColumnWidth(0, 10);
    ui->featuresTableView->setColumnWidth(1, 180);
    ui->featuresTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->featuresTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    types_model = std::allocate_shared<TypeModels>(std::allocator<TypeModels>());
    ui->typesTableView->setModel(types_model->getModel());
    ui->typesTableView->setColumnWidth(0, 10);
    ui->typesTableView->setColumnWidth(2, 70);
    ui->typesTableView->setColumnWidth(4, 100);
    ui->typesTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->typesTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    defects_model = std::allocate_shared<DefectModels>(std::allocator<DefectModels>());
    ui->defectsTableView->setModel(defects_model->getModel());
    ui->defectsTableView->setColumnWidth(0, 10);
    ui->defectsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->defectsTableView->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->stackedWidget->setCurrentIndex(FEATURES_PAGE_INDEX);
}

MainWindow::~MainWindow()
{
    delete ui;
}

bool MainWindow::saveModelsToFile(bool ignore_current_file){
    if (current_file == "" || ignore_current_file){
        QFileDialog dialog;
        dialog.setFileMode(QFileDialog::AnyFile);
        QString filename = dialog.getSaveFileName(this,
            tr("Save config file"), "", tr("XML Files (*.xml)"));
        if (filename == "") return false;
        current_file = filename;
    }

    writeModelsToXml(current_file, features_model, types_model, defects_model);
    saved = true;
    return true;
}

void MainWindow::modelSelectRow(const std::shared_ptr<Models>& model, int index){
    model->selected_row = index;
}

void MainWindow::modelDeleteSelectedRow(const std::shared_ptr<Models>& model){
    model->deleteSelectedObject();
    modelSelectRow(model, 0);
}

void MainWindow::modelEditSelectedRow(const std::shared_ptr<Models>& model, QVariant object){
    model->editSelectedObject(object);
}

// ===== [EDIT SETUP] =====

void MainWindow::editFeatureSetup(){
    Feature feature = features_model->getObject(features_model->selected_row).value<Feature>();
    ui->fComponentsRequiredSpinBox->setValue(feature.components_required);
    ui->fRewardLineEdit->setText(QString::number(feature.reward));
    ui->stackedWidget->setCurrentIndex(FEATURES_EDIT_PAGE_INDEX);
}

//GraphAdjStr<int, int> MainWindow::buildTypesGraph(){
//    GraphAdjStr<int, int> graph(true);

//    for (int i = 0; i < types_model->rowCount(); i++)
//        graph.add_node(i);

//    for (int i = 0; i < types_model->rowCount(); i++){
//        Type type = types_model->getObject(i).value<Type>();
//        for(auto &e:type.sub_components){
//            graph.add_edge(i, e, 0);
//        }
//    }

//    return graph;
//}

void MainWindow::editTypeSetup(){
    Type type = types_model->getObject(types_model->selected_row).value<Type>();
    ui->tNameLineEdit->setText(type.name);
    ui->tCostLineEdit->setText(QString::number(type.cost));


    QVector<DynamicCheckbox*> vec;
    delete ui->tFeaturesSAWidget->layout();
    ui->tFeaturesSAWidget->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
    for(int i = 0; i < features_model->rowCount(); i++){
        auto tmpCheckBox = new DynamicCheckbox(QString::number(i));
        ui->tFeaturesSAWidget->layout()->addWidget(tmpCheckBox);
        vec << tmpCheckBox;
    }

    for(auto &featureId:type.features){
        vec[featureId]->setChecked(true);
    }


    vec.clear();
    delete ui->tSubcomponentsSAWidget->layout();
    ui->tSubcomponentsSAWidget->setLayout(new QBoxLayout(QBoxLayout::LeftToRight));
    for(int i = 0; i < types_model->rowCount(); i++){
        //GraphAdjStr<int, int> tmp = buildTypesGraph();
        //tmp.add_edge(types_model->selected_row, i, 0);
        if(i != types_model->selected_row /*&& !tmp.cycle_exist()*/){
            auto tmpCheckBox = new DynamicCheckbox(QString::number(i));
            ui->tSubcomponentsSAWidget->layout()->addWidget(tmpCheckBox);
            vec << tmpCheckBox;
        }
    }

    for(auto &sub_component:type.sub_components){
        vec[(sub_component < types_model->selected_row) ? sub_component : sub_component - 1]->setChecked(true);
    }

    ui->stackedWidget->setCurrentIndex(TYPES_EDIT_PAGE_INDEX);
}

void MainWindow::editDefectSetup(){
    Defect defect = defects_model->getObject(defects_model->selected_row).value<Defect>();
    ui->dDetectChanceSpinBox->setValue(defect.detect_chance);

    QVector<DynamicGroupBox*> vec;
    delete ui->dCTypesSAWidget->layout();
    ui->dCTypesSAWidget->setLayout(new QVBoxLayout());
    for(int i = 0; i < types_model->rowCount(); i++){
        auto tmpLayout = new DynamicGroupBox(QString::number(i), false, 0, "");
        ui->dCTypesSAWidget->layout()->addWidget(tmpLayout);
        vec << tmpLayout;
    }

    for(auto &type:defect.component_types){
        vec[type.first]->setChecked(true);
        vec[type.first]->spinBox->setValue(type.second.first);
        vec[type.first]->lineEdit->setText(QString::number(type.second.second));
    }

    ui->stackedWidget->setCurrentIndex(DEFECTS_EDIT_PAGE_INDEX);
}

// ===== [TABLEVIEW CLICKED] =====

void MainWindow::on_featuresTableView_clicked(const QModelIndex &index)
{
    ui->featuresTableView->selectRow(index.row());
    modelSelectRow(features_model, index.row());
}

void MainWindow::on_typesTableView_clicked(const QModelIndex &index)
{
    ui->typesTableView->selectRow(index.row());
    modelSelectRow(types_model, index.row());
}

void MainWindow::on_defectsTableView_clicked(const QModelIndex &index)
{
    ui->defectsTableView->selectRow(index.row());
    modelSelectRow(defects_model, index.row());
}

// ===== [DELETE BTN CLICKED] =====

void MainWindow::on_featuresDeleteBtn_clicked()
{
    saved = false;
    ui->featuresTableView->selectRow(0);
    types_model->deletedFeature(features_model->selected_row);
    modelDeleteSelectedRow(features_model);
}

void MainWindow::on_typesDeleteBtn_clicked()
{
    saved = false;
    ui->typesTableView->selectRow(0);
    modelDeleteSelectedRow(types_model);
}

void MainWindow::on_defectsDeleteBtn_clicked()
{
    saved = false;
    ui->defectsTableView->selectRow(0);
    modelDeleteSelectedRow(defects_model);
}

// ===== [EDIT BTN CLICKED] =====

void MainWindow::on_featuresEditBtn_clicked()
{
    if(features_model->rowCount() != 0){
        saved = false;
        editFeatureSetup();
    }
}

void MainWindow::on_typesEditBtn_clicked()
{
    if(types_model->rowCount() != 0){
        saved = false;
        editTypeSetup();
    }
}

void MainWindow::on_defectsEditBtn_clicked()
{
    if(defects_model->rowCount() != 0){
        saved = false;
        editDefectSetup();
    }
}

// ===== [CREATE BTN CLICKED] =====

void MainWindow::on_featuresCreateBtn_clicked()
{
    saved = false;
    features_model->addObject(QVariant::fromValue<Feature>(Feature()));
    ui->featuresTableView->selectRow(features_model->rowCount() - 1);
    modelSelectRow(features_model, features_model->rowCount() - 1);
    editFeatureSetup();
}


void MainWindow::on_typesCreateBtn_clicked()
{
   saved = false;
   types_model->addObject(QVariant::fromValue<Type>(Type()));
   ui->typesTableView->selectRow(types_model->rowCount() - 1);
   modelSelectRow(types_model, types_model->rowCount() - 1);
   editTypeSetup();

}

void MainWindow::on_defectsCreateBtn_clicked()
{
    saved = false;
    defects_model->addObject(QVariant::fromValue<Defect>(Defect()));
    ui->defectsTableView->selectRow(defects_model->rowCount() - 1);
    modelSelectRow(defects_model, defects_model->rowCount() - 1);
    editDefectSetup();
}

// ===== [SAVE BTN CLICKED] =====

void MainWindow::on_fSaveBtn_clicked()
{
    Feature feature;
    feature.id = features_model->getObject(features_model->selected_row).value<Feature>().id;
    feature.components_required = ui->fComponentsRequiredSpinBox->value();
    feature.reward = ui->fRewardLineEdit->text().toInt();

    modelEditSelectedRow(features_model, QVariant::fromValue<Feature>(feature));

    ui->stackedWidget->setCurrentIndex(FEATURES_PAGE_INDEX);
}

void MainWindow::on_tSaveBtn_clicked()
{
    Type type;
    type.id = types_model->getObject(types_model->selected_row).value<Type>().id;
    type.name = ui->tNameLineEdit->text();
    type.cost = ui->tCostLineEdit->text().toInt();

    QList<DynamicCheckbox> featuresCheckBoxList = ui->tFeaturesScrollArea->findChildren<DynamicCheckbox>("");

    foreach(auto &checkBox, featuresCheckBoxList){
        if(checkBox.isChecked())
            type.features << checkBox.text().toInt();
    }

    QList<DynamicCheckbox> subcomponentsCheckBoxList = ui->tSubcomponentsScrollArea->findChildren<DynamicCheckbox>("");

    foreach(auto &checkBox, subcomponentsCheckBoxList){
        if(checkBox.isChecked())
            type.sub_components << checkBox.text().toInt();
    }

    modelEditSelectedRow(types_model, QVariant::fromValue<Type>(type));

    ui->stackedWidget->setCurrentIndex(TYPES_PAGE_INDEX);
}


void MainWindow::on_dSaveBtn_clicked()
{
    Defect defect;
    defect.id = defects_model->getObject(defects_model->selected_row).value<Defect>().id;
    defect.detect_chance = ui->dDetectChanceSpinBox->value();

    QList<DynamicGroupBox> typesGroupBoxList = ui->dCTypesScrollArea->findChildren<DynamicGroupBox>("");

    foreach(auto &groupBox, typesGroupBoxList){
        if(groupBox.checkBox->isChecked()){
            QPair<int, QPair<int, int>> tmp;
            tmp.first = groupBox.checkBox->text().toInt();
            tmp.second.first = groupBox.spinBox->value();
            tmp.second.second = groupBox.lineEdit->text().toInt();
            defect.component_types << tmp;
        }
    }

    modelEditSelectedRow(defects_model, QVariant::fromValue<Defect>(defect));

    ui->stackedWidget->setCurrentIndex(DEFECTS_PAGE_INDEX);
}

// ===== [ACTIONS TRIGGERED] =====

void MainWindow::on_actionFeatures_triggered()
{
    ui->stackedWidget->setCurrentIndex(FEATURES_PAGE_INDEX);
}

void MainWindow::on_actionTypes_triggered()
{
    ui->stackedWidget->setCurrentIndex(TYPES_PAGE_INDEX);
}

void MainWindow::on_actionDefects_triggered()
{
    ui->stackedWidget->setCurrentIndex(DEFECTS_PAGE_INDEX);
}

void MainWindow::on_actionOpen_triggered()
{
    if (!saved){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close file", "File is about to close, whould you like to save changes?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (!saveModelsToFile()) return;
        }
    }

    saved = true;
    QString file = QFileDialog::getOpenFileName(this,
        tr("Open config file"), "", tr("XML Files (*.xml)"));
    if (file == "") return;
    if(readXmlToModels(file, features_model, types_model, defects_model)){
        current_file = file;
        saved = 1;
         ui->stackedWidget->setCurrentIndex(FEATURES_PAGE_INDEX);
    } else {
        QMessageBox::critical(this, "File loading error", "XML file is empty or includes invalid data");
    }
}

void MainWindow::on_actionClose_triggered()
{
    if (!saved){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close file", "File is about to close, whould you like to save changes?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (!saveModelsToFile()) return;
        }
    }

    close();
}


void MainWindow::on_actionSave_triggered()
{
    saveModelsToFile();
}

void MainWindow::on_actionSave_As_triggered()
{
    saveModelsToFile(true);
}

void MainWindow::on_actionNew_triggered()
{
    if (!saved){
        QMessageBox::StandardButton reply;
        reply = QMessageBox::question(this, "Close file", "File is about to close, whould you like to save changes?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::Yes) {
            if (!saveModelsToFile()) return;
        }
    }

    saved = true;
    features_model->clearModel();
    types_model->clearModel();
    defects_model->clearModel();
    ui->stackedWidget->setCurrentIndex(FEATURES_PAGE_INDEX);
}

void MainWindow::on_actionXML_triggered()
{
    std::string str;
    print(std::back_inserter(str), *modelsToXml(features_model, types_model, defects_model), 0);
    ui->xmlPreviewText->setPlainText(QString::fromStdString(str));

    ui->stackedWidget->setCurrentIndex(XML_PREVIEW_PAGE);
}
