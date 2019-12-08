#include "models.h"
#include "types.h"

Models::Models(): selected_row(0)
{

}

Models::~Models()
{

}

void Models::clearModel(){
    while(rowCount()){
        selected_row = 0;
        deleteSelectedObject();
    }
}

// ===== FEATUREMODELS =====

FeatureModels::FeatureModels(): Models(), model(new FeatureModel)
{

}

FeatureModels::~FeatureModels()
{

}

QAbstractItemModel* FeatureModels::getModel()
{
    return model;
}

void FeatureModels::addObject(QVariant object)
{
    Feature feature;
    feature.components_required = object.value<Feature>().components_required;
    feature.reward = object.value<Feature>().reward;

    model->addFeature(feature);
}

void FeatureModels::deleteSelectedObject(){
    model->deleteFeature(selected_row);
}

void FeatureModels::editSelectedObject(QVariant object){
    model->editFeature(selected_row, object.value<Feature>());
}

QVariant FeatureModels::getObject(int index){
    return model->getFeature(index);
}

int FeatureModels::rowCount(){
    return  model->rowCount();
}

// ===== TYPEMODELS =====

TypeModels::TypeModels(): Models(), model(new TypeModel)
{

}

TypeModels::~TypeModels()
{

}

QAbstractItemModel* TypeModels::getModel()
{
    return model;
}

void TypeModels::addObject(QVariant object)
{
    Type type;
    type.name = object.value<Type>().name;
    type.cost = object.value<Type>().cost;
    type.features = object.value<Type>().features;
    type.sub_components = object.value<Type>().sub_components;

    model->addType(type);
}

void TypeModels::deleteSelectedObject(){
    model->deleteType(selected_row);
}

void TypeModels::editSelectedObject(QVariant object){
    model->editType(selected_row, object.value<Type>());
}

QVariant TypeModels::getObject(int index){
    return model->getType(index);
}

int TypeModels::rowCount(){
    return  model->rowCount();
}

void TypeModels::deletedFeature(int index){
    model->deletedFeature(index);
}

// ===== DEFECTMODELS =====

DefectModels::DefectModels(): Models(), model(new DefectModel)
{

}

DefectModels::~DefectModels()
{

}

QAbstractItemModel* DefectModels::getModel()
{
    return model;
}

void DefectModels::addObject(QVariant object)
{
    Defect defect;
    defect.detect_chance = object.value<Defect>().detect_chance;
    defect.component_types = object.value<Defect>().component_types;

    model->addDefect(defect);
}

void DefectModels::deleteSelectedObject(){
    model->deleteDefect(selected_row);
}

void DefectModels::editSelectedObject(QVariant object){
    model->editDefect(selected_row, object.value<Defect>());
}

QVariant DefectModels::getObject(int index){
    return model->getDefect(index);
}

int DefectModels::rowCount(){
    return  model->rowCount();
}

