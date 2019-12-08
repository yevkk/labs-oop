#ifndef FEATUREMODEL_H
#define FEATUREMODEL_H

#include "types.h"
#include <QAbstractTableModel>

class FeatureModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    explicit FeatureModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
    QVariant getFeature(int index);

    // Editable:
    void editFeature(int index, Feature feature);


    Qt::ItemFlags flags(const QModelIndex& index) const override;

    // Add data:
    bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void addFeature(Feature feature);

    // Remove data:
    bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex()) override;
    void deleteFeature(int index);

private:
    std::vector<Feature> features;
};

#endif // FEATUREMODEL_H
