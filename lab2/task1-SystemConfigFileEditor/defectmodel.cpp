#include "defectmodel.h"

#include <QDebug>

DefectModel::DefectModel(QObject *parent)
    : QAbstractTableModel(parent)
{
}

QVariant DefectModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role == Qt::DisplayRole)
        {
            if (orientation == Qt::Horizontal) {
                static QStringList headers{
                    "id",
                    "Detect chance",
                    "Types\n(Repair time, Repair cost)"
                };
                return headers[section];
            }
    }
    return QVariant();
}

int DefectModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return int(defects.size());
}

int DefectModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return 3;
}

QVariant DefectModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
            return QVariant();

        if (role == Qt::DisplayRole) {

            int row = index.row();
            int column = index.column();
            const Defect& defect = defects[unsigned(row)];
            switch(column) {
            case 0:
                return defect.id;
            case 1:
                return defect.detect_chance;
            case 2:{
                QString str;
                for (auto &type:defect.component_types)
                    str += QString::number(type.first) + "(" + QString::number(type.second.first) + ", " + QString::number(type.second.second) + ")  ";
                return str;
            }
            }
        }
        return QVariant();
}

Qt::ItemFlags DefectModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return Qt::NoItemFlags;

    return (Qt::ItemIsEnabled | Qt::ItemIsSelectable);
}

bool DefectModel::insertRows(int row, int count, const QModelIndex &parent)
{
    beginInsertRows(parent, row, row + count - 1);
    endInsertRows();

    return true;
}

bool DefectModel::removeRows(int row, int count, const QModelIndex &parent)
{
    beginRemoveRows(parent, row, row + count - 1);
    endRemoveRows();

    return true;
}

QVariant DefectModel::getDefect(int index){
    return QVariant::fromValue<Defect>(defects[unsigned(index)]);
}

void DefectModel::editDefect(int index, Defect defect){
    defects[unsigned(index)] = defect;
}

void DefectModel::addDefect(Defect defect){
    this->insertRow(int(defects.size()));
    defect.id = int(defects.size());
    defects.push_back(defect);
}

void DefectModel::deleteDefect(int index){
    if(defects.empty()) return;

    this->removeRow(index);
    defects.erase(defects.begin() + index);

    for(unsigned i = unsigned(index); i < defects.size(); i++){
        defects[i].id--;
    }
}

void DefectModel::deletedType(int index){
    for(auto &defect:defects){
        int i = 0;
        while(i < defect.component_types.size()){
            if (defect.component_types[i].first == index){
                defect.component_types.erase(defect.component_types.begin() + i);
            } else if (defect.component_types[i].first > index){
                defect.component_types[i].first--;
                i++;
            } else {
                i++;
            }

        }

    }
}

