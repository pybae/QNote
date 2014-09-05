#include "fileviewmodel.h"
#include <QAbstractListModel>

FileViewModel::FileViewModel(QObject *parent) :
    QAbstractListModel(parent)
{
    return;
}

int FileViewModel::rowCount(const QModelIndex &parent) const
{
   return 2;
}


QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    return QVariant();
}
