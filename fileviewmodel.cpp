#include "fileviewmodel.h"
#include <QAbstractListModel>

// simple constructor from a QStringList
FileViewModel::FileViewModel(QStringList files, QObject *parent) :
    QAbstractListModel(parent)
{
    file_list = files;
}

// inherting and adding the basic row count function
int FileViewModel::rowCount(const QModelIndex &parent) const
{
    return file_list.size();
}

// inheriting and adding the data function
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

// inheriting and adding the set data function
bool FileViewModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
    if (!index.isValid())
        return false;
    if (index.row() >= file_list.size())
        return false;
    if (role == Qt::EditRole) {
        file_list.replace(index.row(), value.toString());
        FileViewModel::dataChanged(index, index);
        return true;
    }
    return false;
}

// implements adding a file to the list
bool FileViewModel::addFile(const QVariant &value)
{
    file_list.append(value.toString());
    FileViewModel::dataChanged(FileViewModel::index(file_list.size()-1), FileViewModel::index(file_list.size()));
    return true;
}

// implements getting the indexOf a file name in the list
QModelIndex FileViewModel::indexOf(const QString fileName)
{
    if (!fileName.isEmpty()) {
        int idx = file_list.indexOf(fileName);
        return FileViewModel::index(idx);
    }
    return QModelIndex();
}
