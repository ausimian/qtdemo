#include <qbrush.h>
#include "my_model.h"

MyModel::MyModel()
{
}

int MyModel::rowCount(const QModelIndex &parent) const
{
    return 300;
}

int MyModel::columnCount(const QModelIndex &parent) const
{
    return 48;
}

QVariant MyModel::data(const QModelIndex &index, int role) const
{
    QVariant v;
    const auto col = index.column();

    switch (role)
    {
        case Qt::DisplayRole:
            if (col < 3)
                v.setValue(QString("Some Text"));
            else if (col < 15 || col > 20)
            {
                auto r = (float)rand() / (float)rand();
                v.setValue(r);
            }
            break;
        case Qt::TextAlignmentRole:
            if (col < 3)
                v.setValue((int)(Qt::AlignHCenter | Qt::AlignLeft));
            else
                v.setValue((int)(Qt::AlignHCenter | Qt::AlignRight));
            break;
        case Qt::CheckStateRole:
            if (col >= 15 && col <= 20)
                v.setValue(col & 1 == 1);
            break;
        case Qt::BackgroundRole:
            switch(col)
            {
                case 1:
                    v.setValue(QBrush(Qt::GlobalColor::darkGreen));
                    break;
                case 3:
                    v.setValue(QBrush(Qt::GlobalColor::yellow));
                    break;
                case 7:
                    v.setValue(QBrush(Qt::GlobalColor::cyan));
                    break;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return v;
}

Qt::ItemFlags MyModel::flags(const QModelIndex& index) const
{
    if (index.column() >= 15 && index.column() <= 20)
        return Qt::ItemIsUserCheckable;
    return QAbstractTableModel::flags(index);
}
