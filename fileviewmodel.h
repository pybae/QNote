#ifndef FILEVIEWMODEL_H
#define FILEVIEWMODEL_H

#include <QAbstractListModel>

class FileViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FileViewModel(QObject *parent);
    ~FileViewModel();
    int rowCount(const QModelIndex &parent = QModelIndex()) const ;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

};

#endif // FILEVIEWMODEL_H
