#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QDir>
#include "fileviewmodel.h"

namespace Ui {
class QNote;
}

class QNote : public QMainWindow
{
    Q_OBJECT

public:
    explicit QNote(QWidget *parent = 0);
    ~QNote();

private:
    Ui::QNote *ui;
    QString working_file_name; // absolute file name
    QDir parent_dir;
    FileViewModel *fileModel;
    void saveFile(QString fileName);
    QDir readInDefaultDirectory();
    void updateDate();
    void updateListViewSelection(QString fileName);

private slots:
    void on_actionOpen_triggered();
    void on_actionSaveAs_triggered();
    void on_mainTextEdit_textChanged();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();
    void on_listView_clicked(const QModelIndex &index);
    void on_titleEdit_returnPressed();
};

#endif // NOTEPAD_H
