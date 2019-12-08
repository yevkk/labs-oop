#ifndef TYPEMODEL_H
#define TYPEMODEL_H

#include "types.h"
#include <QAbstractTableModel>

class TypeModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit TypeModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant getType(int index);

    // Editable:
    void editType(int index, Type type);
    void deletedFeature(int index);


    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addType(Type type);

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void deleteType(int index);

private:
    std::vector<Type> types;
};

#endif // TYPEMODEL_H
