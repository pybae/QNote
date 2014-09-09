#include "notepad.h"
#include "ui_notepad.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QtPrintSupport>
#include <QListView>
#include <fileviewmodel.h>
#include <QLabel>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    readInDefaultDirectory();
    QStringList files = working_dir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    ui->setupUi(this);
    fileModel = new FileViewModel(files, 0);
    ui->listView->setModel(fileModel);
    ui->listView->show();
}

// deconstructor
Notepad::~Notepad()
{
    delete ui;
}

// A helper method to read in the default directory
void Notepad::readInDefaultDirectory()
{
    QFile metadata(QDir::homePath() + "/.notetakinginfo");

    if (!metadata.exists()) {
        // TODO send a message box
        working_dir = QDir(QFileDialog::getExistingDirectory(this, tr("Default Directory"),
                                                        QDir::homePath(),
                                                        QFileDialog::ShowDirsOnly
                                                        | QFileDialog::DontResolveSymlinks));
        if (!metadata.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not write to file"));
            return;
        } else {
            QTextStream stream(&metadata);
            stream << working_dir.absolutePath() << "\n";
            stream.flush();
            metadata.close();
        }
    }
    else {
        if (!metadata.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not read file"));
            return;
        }
        QTextStream in(&metadata);
        working_dir = QDir(in.readLine());
        metadata.close();
    }
}

// A helper method to save a file, given a fileName in the current working directory
void Notepad::saveFile(QString fileName)
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
void Notepad::on_actionNew_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), working_dir.absolutePath(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    saveFile(fileName);
    updateDate();

    QFileInfo fileInfo(fileName);
    QString localFileName(fileInfo.fileName());

    fileModel->addFile(localFileName);
    working_file_name = fileName;
}

// Called when the "Open" option is triggered by C-o or menu
void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), working_dir.absolutePath(),
            tr("All Files (*);;Text Files (*.txt);;RTF Files(*.rtf);;C++ Files (*.cpp *.h)"));
    if (!fileName.isEmpty()) {
        if(fileName.contains(".o", Qt::CaseInsensitive)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open this file format yet"));
            return;
        }
        QFile file(fileName);
        working_file_name = file.fileName();

        if (!file.open(QIODevice::ReadOnly)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
            return;
        }
        QTextStream in(&file);
        ui->mainTextEdit->setText(in.readAll());
        file.close();
    }
}

// Called when the "Save" option is triggered by C-s or menu
void Notepad::on_actionSave_triggered()
{
    saveFile(working_file_name);
    updateDate();
}

// Called when the "Save As" option is triggered by C-S (Ctrl shift s) or menu
void Notepad::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    saveFile(fileName);
}

// Called when the "Print" option is triggered by C-p or menu
void Notepad::on_actionPrint_triggered()
{
//     QPrinter printer;

//     QPrintDialog dialog(&printer, this);
//     dialog.setWindowTitle(tr("Print Document"));
//     if (dialog.exec() != QDialog::Accepted) {
//         return;
//     }
}

// Called when the "Exit" option is triggered by C-q or menu
void Notepad::on_actionExit_triggered()
{
    // TODO need to check if there are any unsaved buffers
    qApp->quit();
}

// Triggered when the mainTextEdit region has its text changed
// TODO figure out how frequently this method is called
void Notepad::on_mainTextEdit_textChanged()
{
    // Save the current buffer
    // Notepad::on_actionSave_triggered();
}

// Called when the listView is clicked
void Notepad::on_listView_clicked(const QModelIndex &index)
{
    if (!working_file_name.isEmpty())
        saveFile(working_file_name); // TODO should only save when changes have been made
    QString fileName = working_dir.absoluteFilePath(fileModel->data(index).toString());
    if (!fileName.isEmpty()) {
        QFile file(fileName);
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
void Notepad::on_actionAbout_triggered()
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
void Notepad::on_titleEdit_returnPressed()
{
    if(!working_file_name.isEmpty()) {
        // rename the file
        QFile file(working_file_name);
        QFileInfo fileInfo(file);
        QString path = working_dir.absolutePath() + QDir::separator();

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
void Notepad::updateDate()
{
    QFile file(working_file_name);
    QFileInfo fileInfo(file);
    QDateTime dateTime = fileInfo.lastModified();
    ui->dateLabel->setText(dateTime.toString("dddd, MMM d, h:mm A"));
    return;
}
