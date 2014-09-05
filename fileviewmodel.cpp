#include "fileviewmodel.h"
#include <QAbstractListModel>

FileViewModel::FileViewModel(QStringList files, QObject *parent) :
    QAbstractListModel(parent)
{
    printf("inside instantiator?\n");
    file_list = files;
}

int FileViewModel::rowCount(const QModelIndex &parent) const
{
    printf("inside here?\n");
    return file_list.size();
}

QVariant FileViewModel::data(const QModelIndex &index, int role) const
{
    printf("Inside the data method\n");
    return QString("haha");
}
