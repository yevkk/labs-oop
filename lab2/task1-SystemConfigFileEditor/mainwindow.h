#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "graphs/graph_adj_str.h"
#include "models.h"

#include <QMainWindow>

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
    void on_featuresTableView_clicked(const QModelIndex &index);

    void on_featuresDeleteBtn_clicked();

    void on_featuresEditBtn_clicked();

    void on_featuresCreateBtn_clicked();

    void on_fSaveBtn_clicked();

    void on_actionFeatures_triggered();

    void on_actionTypes_triggered();

    void on_typesTableView_clicked(const QModelIndex &index);

    void on_typesDeleteBtn_clicked();

    void on_typesEditBtn_clicked();

    void on_typesCreateBtn_clicked();

    void on_tSaveBtn_clicked();

    void on_actionOpen_triggered();

    void on_actionClose_triggered();

    void on_actionSave_triggered();

    void on_actionSave_As_triggered();

    void on_actionNew_triggered();

    void on_actionXML_triggered();

    void on_defectsTableView_clicked(const QModelIndex &index);

    void on_actionDefects_triggered();

    void on_defectsEditBtn_clicked();

    void on_defectsCreateBtn_clicked();

    void on_dSaveBtn_clicked();

    void on_defectsDeleteBtn_clicked();

private:
    Ui::MainWindow *ui;

    bool saveModelsToFile(bool ignore_current_file = false);

    void modelSelectRow(const std::shared_ptr<Models>& model, int index);
    void modelDeleteSelectedRow(const std::shared_ptr<Models>& model);
    void modelEditSelectedRow(const std::shared_ptr<Models>& model, QVariant object);
    void editFeatureSetup();
    void editTypeSetup();
    void editDefectSetup();
    GraphAdjStr<int, int> buildTypesGraph();


    const int FEATURES_PAGE_INDEX = 0;
    const int TYPES_PAGE_INDEX = 2;
    const int DEFECTS_PAGE_INDEX = 4;

    const int FEATURES_EDIT_PAGE_INDEX = 1;
    const int TYPES_EDIT_PAGE_INDEX = 3;
    const int DEFECTS_EDIT_PAGE_INDEX = 5;

    const int XML_PREVIEW_PAGE = 6;


    std::shared_ptr<FeatureModels> features_model;
    std::shared_ptr<TypeModels> types_model;
    std::shared_ptr<DefectModels> defects_model;

    QString current_file;
    bool saved;
};
#endif // MAINWINDOW_H
