#ifndef MODELS_H
#define MODELS_H

#include "featuremodel.h"
#include "typemodel.h"
#include "defectmodel.h"

#include <QStandardItemModel>


class Models
{
public:
    Models();
    virtual ~Models();
    virtual QAbstractItemModel* getModel()=0;
    virtual void addObject(QVariant object)=0;
    virtual void deleteSelectedObject()=0;
    virtual void editSelectedObject(QVariant object)=0;
    virtual QVariant getObject(int index)=0;
    virtual int rowCount()=0;
    virtual void clearModel();


    int selected_row;
};

class FeatureModels: public Models
{
public:
    FeatureModels();
    ~FeatureModels() override;
    QAbstractItemModel* getModel() override;
    void addObject(QVariant object) override;
    void deleteSelectedObject() override;
    void editSelectedObject(QVariant object) override;
    QVariant getObject(int index) override;
    int rowCount() override;

private:
    FeatureModel *model;
};

class TypeModels: public Models
{
public:
    TypeModels();
    ~TypeModels() override;
    QAbstractItemModel* getModel() override;
    void addObject(QVariant object) override;
    void deleteSelectedObject() override;
    void editSelectedObject(QVariant object) override;
    QVariant getObject(int index) override;
    int rowCount() override;

    void deletedFeature(int index);

private:
    TypeModel *model;
};

class DefectModels: public Models
{
public:
    DefectModels();
    ~DefectModels() override;
    QAbstractItemModel* getModel() override;
    void addObject(QVariant object) override;
    void deleteSelectedObject() override;
    void editSelectedObject(QVariant object) override;
    QVariant getObject(int index) override;
    int rowCount() override;

    void deletedType(int index);

private:
    DefectModel *model;
};

#endif // MODELS_H
