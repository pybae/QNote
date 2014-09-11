#include "QNote.h"
#include "ui_notepad.h"
#include "qnotelib.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QtPrintSupport>
#include <QListView>
#include <fileviewmodel.h>
#include <cassert>
#include <QLabel>

QNote::QNote(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QNote)
{
    parent_dir = readInDefaultDirectory();
    QStringList files = parent_dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->setupUi(this);
    fileModel = new FileViewModel(files, 0);
    ui->listView->setModel(fileModel);
}

QNote::~QNote()
{
    delete ui;
}

// A helper method to read in the default directory
QDir QNote::readInDefaultDirectory()
{
    QFile metadata(QDir::homePath() + QDir::separator() + ".notetakinginfo");
    QDir parent_dir;
    if (!metadata.exists()) {
        QMessageBox::warning(this, tr("No default directory found"), tr("Please choose a default directory"));

        QFileDialog dlg(this, tr("Default directory"));
        QString working_dir_name = dlg.getExistingDirectory(this, tr("Default directory"),
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        if (!working_dir_name.isEmpty())
            parent_dir = QDir(working_dir_name);
        else {
            QMessageBox::critical(this, tr("No default directory"), tr(""));
            exit(0);
        }
        if (!metadata.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not write to file"));
            exit(0);
        } else {
            QTextStream stream(&metadata);
            stream << parent_dir.absolutePath() << "\n";
            stream.flush();
            metadata.close();
        }
    }
    else {
        if (!metadata.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not read file"));
            exit(0);
        }
        QTextStream in(&metadata);
        parent_dir = QDir(in.readLine());
        metadata.close();
    }
    return parent_dir;
}

// A helper method to save a file, given a fileName in the current working directory
void QNote::saveFile(QString fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("IO Error"), tr("Cannot save to file"));
            return;
        } else {
            QTextStream stream(&file);
            stream << ui->mainTextEdit->toPlainText();
            stream.flush();
            file.close();
        }
    }
    else {
        qDebug("File does not exist");
    }
}

// Called when the "New" option is triggered by C-n or menu
void QNote::on_actionNew_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), parent_dir.absolutePath(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    ui->mainTextEdit->clear();
    saveFile(fileName);
    updateDate();
    // Clear the buffer
    QFileInfo fileInfo(fileName);
    QString localFileName(fileInfo.fileName());

    fileModel->addFile(localFileName);
    working_file_name = fileName;
}

// Called when the "Open" option is triggered by C-o or menu
void QNote::on_actionOpen_triggered()
{
    saveFile(working_file_name);
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), parent_dir.absolutePath(),
            tr("All Files (*);;Text Files (*.txt);;RTF Files(*.rtf);;C++ Files (*.cpp *.h)"));
    if (!fileName.isEmpty()) {
        if(fileName.contains(".o", Qt::CaseInsensitive)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open this file format yet"));
            return;
        }
        updateListViewSelection(fileName);
        QFile file(fileName);
        QFileInfo fileInfo(file);
        working_file_name = file.fileName();
        QString simpleFileName = fileInfo.fileName();

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        ui->mainTextEdit->setText(in.readAll());
        file.close();

        QTextStream titleIn(&simpleFileName);
        ui->titleEdit->setText(titleIn.readAll());
    }
}

// Called when the "Save" option is triggered by C-s or menu
void QNote::on_actionSave_triggered()
{
    saveFile(working_file_name);
    updateDate();
}

// Called when the "Save As" option is triggered by C-S (Ctrl shift s) or menu
void QNote::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),  parent_dir.absolutePath(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    saveFile(fileName);
}

// Called when the "Print" option is triggered by C-p or menu
void QNote::on_actionPrint_triggered()
{
//     QPrinter printer;

//     QPrintDialog dialog(&printer, this);
//     dialog.setWindowTitle(tr("Print Document"));
//     if (dialog.exec() != QDialog::Accepted) {
//         return;
//     }
}

// Called when the "Exit" option is triggered by C-q or menu
void QNote::on_actionExit_triggered()
{
    // TODO need to check if there are any unsaved buffers
    qApp->quit();
}

// Triggered when the mainTextEdit region has its text changed
// TODO figure out how frequently this method is called
void QNote::on_mainTextEdit_textChanged()
{
    // Save the current buffer
    // Notepad::on_actionSave_triggered();
}

// Called when the listView is clicked
void QNote::on_listView_clicked(const QModelIndex &index)
{
    if (!working_file_name.isEmpty())
        saveFile(working_file_name); // TODO should only save when changes have been made
    QString fileName = parent_dir.absoluteFilePath(fileModel->data(index).toString());
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if(fileName.contains(".o", Qt::CaseInsensitive)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open this file format yet"));
            return;
        }
        QFileInfo fileInfo(file);
        QString simpleFileName = fileInfo.fileName();
        working_file_name = file.fileName();

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        ui->mainTextEdit->setText(in.readAll());
        file.close();

        QTextStream titleIn(&simpleFileName);
        ui->titleEdit->setText(titleIn.readAll());
        updateDate();
    }
}

// Called as a simple about section
void QNote::on_actionAbout_triggered()
{
    QString abouttext = tr("<h1>Notetaking</h1>");

    abouttext.append(tr("<p><b>Easy - Intuitive -Anywhere</b></p>"));
    abouttext.append(tr("<p><a href=\"http://qt-project.org/\">QT: </a>"));
    abouttext.append(tr("editor of Notetaking</p>"));
    abouttext.append(tr("<p>Project page: <a href=\"https://github.com/pybae/notetaking\">source</a></p>"));
    abouttext.append(tr("<p>Awesome Devs: <br/><br/> Paul Bae<br/>"));
    abouttext.append(tr("Tianyu Cheng<br/>"));
    abouttext.append(tr("Kim Yu Ng<br/>"));
    abouttext.append(tr("Pikachu</p>"));
    abouttext.append(tr("<p>This software is released under the "
                        "<a href=\"http://opensource.org/licenses/MIT\"" \
                        ">MIT License</a></p>"));
    QMessageBox::about(this, tr("About us"), abouttext);
}

// renames the current file when enter is pressed in the title
void QNote::on_titleEdit_returnPressed()
{
    if(!working_file_name.isEmpty()) {
        // rename the file
        QFile file(working_file_name);
        QFileInfo fileInfo(file);
        QString path = parent_dir.absolutePath() + QDir::separator();

        QString newFileName = path + ui->titleEdit->displayText();
        QFile newFile(newFileName);
        QFileInfo newFileInfo(newFile);

        QModelIndex newIndex = fileModel->indexOf(newFileInfo.fileName());
        if (newIndex.isValid()) { // new file name already exists
            QMessageBox::critical(this, tr("Error"), tr("File already exists"));
            return;
        }
        file.rename(newFileName);

        QModelIndex index = fileModel->indexOf(fileInfo.fileName());
        fileModel->setData(index, newFileInfo.fileName(), Qt::EditRole);
        working_file_name = newFileName;

        updateDate();
    }
}

// Simple function to update the date
void QNote::updateDate()
{
    QFile file(working_file_name);
    QFileInfo fileInfo(file);
    QDateTime dateTime = fileInfo.lastModified();
    ui->dateLabel->setText(dateTime.toString("dddd, MMM d, h:mm A"));
    return;
}

// Function to update the listView selection
void QNote::updateListViewSelection(QString fileName)
{
    QFile file(fileName);
    QFileInfo fileInfo(file);
    QDir fileDir = fileInfo.absoluteDir();
    if (parent_dir == fileDir) {
        QModelIndex index = fileModel->indexOf(fileInfo.fileName());
        assert (index.isValid());
        ui->listView->setCurrentIndex(index);
    }
    else
        qDebug("Not in working directory, can't show in list view yet (not implemented)!");
}
