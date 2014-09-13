#ifndef QNOTE_H
#define QNOTE_H

#include "src_global.h"
#include "fileviewmodel.h"

#include <QMainWindow>
#include <QDir>
#include <QBuffer>

namespace Ui {
class QNote;
}

class SRCSHARED_EXPORT QNote : public QMainWindow
{

public:
    // Constructs a QNote instance
    // note that most of the actual initializiations are done in setup
    explicit QNote(QWidget *parent = 0);

    // Deconstructs a QNote instance
    ~QNote();

    // Should be called when creating a QNote instance
    void setup();

private:
    Ui::QNote *ui;                    // The main Ui of the class
    QString currentFileName;          // the current file displayed (absolute path)
    QDir parentDir;                   // the main parent directory of all the files
    FileViewModel *fileModel;         // the fileModel for the listView

    // reads in the user's configuration files at startup
    // this function is called in setup
    void readConfig();

    // saves the file given to it and updates all appropriate variables
    // returns true if saving succeeds
    bool saveFile(QFile& file);

    // updates the date of the modified file
    void updateDate();

    // updates the selected item in the list view
    void updateListViewSelection(QString fileName);

    // writes to the file provided, returns false if the the write fails
    bool writeToFile(QFile& file, QString data);

    // opens the file based on the mode given and returns true if it succeeds
    bool checkedOpenFile(QFile& file, QIODevice::OpenMode mode);

private slots:
    void on_actionOpen_triggered();
    void on_actionSaveAs_triggered();
    void on_actionNew_triggered();
    void on_actionSave_triggered();
    void on_actionPrint_triggered();
    void on_actionExit_triggered();
    void on_actionAbout_triggered();

    void on_titleEdit_returnPressed();
    void on_mainTextEdit_textChanged();

    void on_listView_clicked(const QModelIndex &index);
};

#endif // QNOTE_H
