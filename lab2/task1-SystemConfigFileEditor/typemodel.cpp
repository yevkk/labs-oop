#include "typemodel.h"

#include <QDebug>

TypeModel::TypeModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant TypeModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                static QStringList headers{
                    "id",
                    "Name",
                    "Cost",
                    "Features",
                    "Subcomponents'\ntypes"
                };
                return headers[section];
            }
    }
    return QVariant();
}

int TypeModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return int(types.size());
}

int TypeModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 5;
}

QVariant TypeModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole) {

            int row = index.row();
            int column = index.column();
            const Type& type = types[unsigned(row)];
            switch(column) {
            case 0:
                return type.id;
            case 1:
                return type.name;
            case 2:
                return type.cost;
            case 3:{
                QString res;
                for(auto &e:type.features)
                    res+= QString::number(e) + "  ";

                return res;
            }
            case 4:QString res;
                for(auto &e:type.sub_components)
                    res+= QString::number(e) + "  ";

                return res;
            }
        }
        return QVariant();
}

Qt::ItemFlags TypeModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

bool TypeModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();

    return true;
}

bool TypeModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();

    return true;
}

QVariant TypeModel::getType(int index){
    return QVariant::fromValue<Type>(types[unsigned(index)]);
}

void TypeModel::editType(int index, Type type){
    types[unsigned(index)] = type;
}

void TypeModel::addType(Type type){
    this->insertRow(int(types.size()));
    type.id = int(types.size());
    types.push_back(type);
}

void TypeModel::deleteType(int index){
    if(types.empty()) return;

    this->removeRow(index);
    types.erase(types.begin() + index);

    for(auto &type:types){
        if (type.id > index)
            type.id--;
        int i = 0;
        while(i < type.sub_components.size()){
            if (type.sub_components[i] == index){
                type.sub_components.erase(type.sub_components.begin() + i);
            } else if (type.sub_components[i] > index)
            {
                type.sub_components[i]--;
                i++;
            }
        }

    }

}

void TypeModel::deletedFeature(int index){
    for(auto &type:types){
        int i = 0;
        while(i < type.features.size()){
            if (type.features[i] == index){
                type.features.erase(type.features.begin() + i);
            } else if (type.features[i] > index){
                type.features[i]--;
                i++;
            }

        }

    }
}

