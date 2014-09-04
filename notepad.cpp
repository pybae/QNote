#include "notepad.h"
#include "ui_notepad.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <iostream>

Notepad::Notepad(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Notepad)
{
    // TODO, not sure where to organize instantiating methods
    // Temporarily changing to a directory
    working_dir = QDir("/Users/pybae/Documents");
    file_list = working_dir.entryList();

    QStringListIterator it(file_list);
    while (it.hasNext()) {
        std::cout << it.next().toStdString() << std::endl;
    }
    ui->setupUi(this);
}

Notepad::~Notepad()
{
    delete ui;
}

// Called when the "New" option is triggered by C-n or menu
void Notepad::on_actionNew_triggered()
{
    // TODO
}

// Called when the "Open" option is triggered by C-o or menu
void Notepad::on_actionOpen_triggered()
{
    printf("What is the working dir: %s\n", working_dir.absolutePath().toStdString().c_str());
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), working_dir.absolutePath(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));

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

    printf("what is working_file: %s\n", working_file_name.toStdString().c_str());
}

// Called when the "Save" option is triggered by C-s or menu
void Notepad::on_actionSave_triggered()
{
    // TODO
    // can refactor both the save methods
    if (!working_file_name.isEmpty()) {
        QFile file(working_file_name);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
            return;
        } else {
            QTextStream stream(&file);
            stream << ui->mainTextEdit->toPlainText();
            stream.flush();
            file.close();
        }
    }
    else {
        printf("File not intiailized yet\n");
    }
}

// Called when the "Save As" option is triggered by C-S (Ctrl shift s) or menu
void Notepad::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), QString(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    // can refactor both the save methods
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if (!file.open(QIODevice::WriteOnly)) {
            // error message
            return;
        } else {
            QTextStream stream(&file);
            stream << ui->mainTextEdit->toPlainText();
            stream.flush();
            file.close();
        }
    }

}

// Called when the "Print" option is triggered by C-p or menu
void Notepad::on_actionPrint_triggered()
{
    // TODO
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
    Notepad::on_actionSave_triggered();
}
