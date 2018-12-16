#pragma once


#include <qabstractitemmodel.h>

class MyModel : public QAbstractTableModel
{
public:
    MyModel();

    int rowCount(const QModelIndex&) const override;
    int columnCount(const QModelIndex&) const override;
    QVariant data(const QModelIndex& index, int role) const override;

    Qt::ItemFlags flags(const QModelIndex&) const override;
    
};