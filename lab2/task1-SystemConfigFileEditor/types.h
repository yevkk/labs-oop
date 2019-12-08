#ifndef TYPES_H
#define TYPES_H

#include<QObject>
#include<QCheckBox>
#include<QGroupBox>
#include<QHBoxLayout>
#include<QSpinBox>
#include<QLineEdit>

#include<QVariant>
#include<QString>
#include<QVector>
#include<QPair>

class Feature {
public:
    int id;
    int components_required;
    int reward;

    Feature() : id(0), components_required(0), reward(0){}
};

class Type {
public:
    int id;
    QString name;
    int cost;
    QVector<int> features;
    QVector<int> sub_components;

    Type() : id(0), name(), cost(0), features(0), sub_components(0) {}
};

class Defect {
public:
    int id;
    //{componentTypeId, {repairTime, repairCost}}
    QVector<QPair<int, QPair<int, int>>> component_types;
    double detect_chance;
};

Q_DECLARE_METATYPE(Feature);
Q_DECLARE_METATYPE(Type);
Q_DECLARE_METATYPE(Defect);

class DynamicCheckbox : public QCheckBox{
public:
    DynamicCheckbox(const QString& name, bool checked = false):QCheckBox()
    {
        setCheckable (true);
        setChecked (checked);
        setGeometry (10, 10, 40, 20);
        setText (name);
    }

    DynamicCheckbox(const DynamicCheckbox &other){}

    DynamicCheckbox& operator=(const DynamicCheckbox &other){
        return *this;
    }
};

class DynamicLayout : public QGroupBox {
public:
    DynamicCheckbox* checkBox;
    QLineEdit* lineEdit;
    QSpinBox* spinBox;

    DynamicLayout(const QString& checkbox_name, bool checkbox_checked, int spinbox_value, QString lineedit_value) : QGroupBox()
    {
        QHBoxLayout* lyt = new QHBoxLayout;
        checkBox = new DynamicCheckbox(checkbox_name, checkbox_checked);
        lyt->addWidget(checkBox);

        spinBox = new QSpinBox;
        spinBox->setFont(QFont("Yu Gothic UI", 11));
        spinBox->setValue(spinbox_value);
        lyt->addWidget(spinBox);

        lineEdit = new QLineEdit;
        lineEdit->setFont(QFont("Yu Gothic UI", 11));
        lineEdit->setText(lineedit_value);
        lyt->addWidget(lineEdit);

        setLayout(lyt);
    }

    DynamicLayout(const DynamicLayout &other){}

    DynamicLayout& operator=(const DynamicLayout &other){
        return *this;
    }

    void setChecked(bool value){
        checkBox->setChecked(value);
    }

};
#endif // TYPES_H
