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


Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    // TODO, not sure where to organize instantiating methods
    // Temporarily changing to a directory
    working_dir = QDir("/Users/pybae/Documents");
    QStringList files = working_dir.entryList();

    ui->setupUi(this);

    FileViewModel *fileModel = new FileViewModel(files, 0);
    ui->listView->setModel(fileModel);
    ui->listView->show();
}

Notepad::~Notepad()
{
    delete ui;
}

// A helper method to save a file, given a fileName in the current working directory
void Notepad::saveFile(QString fileName)
{
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
            return;
        } else {
            QTextStream stream(&file);
            stream << ui->mainTextEdit->toPlainText() << "\n";
            stream.flush();
            file.close();
        }
    }
    else {
        printf("File does not exist\n");
    }
}

// Called when the "New" option is triggered by C-n or menu
void Notepad::on_actionNew_triggered()
{
    QString newFileName = QFileDialog::getSaveFileName(this, tr("New File"), working_dir.absolutePath(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    saveFile(newFileName);
    working_file_name = newFileName;
}

// Called when the "Open" option is triggered by C-o or menu
void Notepad::on_actionOpen_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), working_dir.absolutePath(),
            tr("All Files (*.*);;Text Files (*.txt);;RTF Files(*.rtf);;C++ Files (*.cpp *.h)"));

    if (!fileName.isEmpty()) {
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
