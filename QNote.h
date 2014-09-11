#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QDir>
#include <QBuffer>
#include "fileviewmodel.h"

namespace Ui {
class QNote;
}

// The main class and core of the application
// QNote serves as an interface to the Ui and does most of the processing
// for user input as well
class QNote : public QMainWindow
{
    Q_OBJECT

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

#endif // NOTEPAD_H
