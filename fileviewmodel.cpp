#include "fileviewmodel.h"

FileViewModel::FileViewModel(QObject *parent) :
    QAbstractListModel(parent)
{

}

int FileViewModel::rowCount(const QModelIndex & /*parent*/) const
{
   return 2;
}


QVariant FileViewModel::data(const QModelIndex & index, int role) const
{
    if (role == Qt::DisplayRole)
        return QString("haha");
    return QVariant();
}
