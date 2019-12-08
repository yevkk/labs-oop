#ifndef DEFECTMODEL_H
#define DEFECTMODEL_H

#include "types.h"
#include <QAbstractTableModel>

class DefectModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit DefectModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant getDefect(int index);

    // Editable:
    void editDefect(int index, Defect defetc);

    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addDefect(Defect defect);

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void deleteDefect(int index);

private:
    std::vector<Defect> defects;
};

#endif // DEFECTMODEL_H
