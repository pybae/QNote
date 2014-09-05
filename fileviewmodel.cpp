#include "fileviewmodel.h"
#include <QAbstractListModel>

FileViewModel::FileViewModel(QStringList files, QObject *parent) :
    QAbstractListModel(parent)
{
    file_list = files;
    return;
}

int FileViewModel::rowCount(const QModelIndex &parent) const
{
   return file_list.size();
}


QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    return QString("haha");
}
