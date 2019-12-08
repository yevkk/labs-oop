#include "featuremodel.h"

#include <QDebug>

FeatureModel::FeatureModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant FeatureModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                static QStringList headers{
                    "id",
                    "Components\nrequired",
                    "Reward"
                };
                return headers[section];
            }
    }
    return QVariant();
}

int FeatureModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return int(features.size());
}

int FeatureModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant FeatureModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole) {

            int row = index.row();
            int column = index.column();
            const Feature& feature = features[unsigned(row)];
            switch(column) {
            case 0:
                return feature.id;
            case 1:
                return feature.components_required;
            case 2:
                return feature.reward;
            }
        }
        return QVariant();
}

Qt::ItemFlags FeatureModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

bool FeatureModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();

    return true;
}

bool FeatureModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();

    return true;
}

QVariant FeatureModel::getFeature(int index){
    return QVariant::fromValue<Feature>(features[unsigned(index)]);
}

void FeatureModel::editFeature(int index, Feature feature){
    features[unsigned(index)] = feature;
}

void FeatureModel::addFeature(Feature feature){
    this->insertRow(int(features.size()));
    feature.id = int(features.size());
    features.push_back(feature);
}

void FeatureModel::deleteFeature(int index){
    if(features.empty()) return;

    this->removeRow(index);
    features.erase(features.begin() + index);

    for(unsigned i = unsigned(index); i < features.size(); i++){
        features[i].id--;
    }

}

