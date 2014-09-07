#include "fileviewmodel.h"
#include <QAbstractListModel>

FileViewModel::FileViewModel(QStringList files, QObject *parent) :
    QAbstractListModel(parent)
{
    file_list = files;
}

int FileViewModel::rowCount(const QModelIndex &parent) const
{
    return file_list.size();
}

QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row() >= file_list.size())
        return QVariant();
    if (role == Qt::DisplayRole)
        return file_list.at(index.row());
    else
        return QVariant();
}

bool FileViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.row() >= file_list.size())
        return false;
    if (role == Qt::EditRole) {
        file_list.replace(index.row(), value.toString());
        return true;
    }
    return false;
}
