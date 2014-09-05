#ifndef FILEVIEWMODEL_H
#define FILEVIEWMODEL_H

#include <QAbstractListModel>
#include <QMainWindow>

class FileViewModel : public QAbstractListModel
{
    Q_OBJECT

public:
    explicit FileViewModel(QStringList files, QObject *parent = 0);
    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

private:
    QStringList file_list;

};

#endif // FILEVIEWMODEL_H
