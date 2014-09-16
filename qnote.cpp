#include <qnote.h>
#include <ui_qnote.h>

#include <fileviewmodel.h>
#include <cassert>

#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QListWidget>
#include <QtPrintSupport>
#include <QListView>
#include <QDebug>
#include <QLabel>
#include <QBuffer>

//#include "/usr/local/include/hunspell/hunspell.hxx"

QNote::QNote(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::QNote)
{
    ui->setupUi(this);
}

QNote::~QNote()
{
    delete ui;
}

void QNote::setup()
{
    readConfig();
    QStringList files = parentDir.entryList(QDir::AllEntries | QDir::NoDotAndDotDot);
    fileModel = new FileViewModel(files, 0);
    ui->listView->setModel(fileModel);
}

void QNote::readConfig()
{
    QFile configFile(QDir::homePath() + QDir::separator() + ".notetakinginfo");
    if (!configFile.exists()) {
        QMessageBox::warning(this, tr("No default directory found"), tr("Please choose a default directory"));

        QString working_dir_name = QFileDialog::getExistingDirectory(this, tr("Default directory"),
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        if (!working_dir_name.isEmpty())
            parentDir = QDir(working_dir_name);
        else
            readConfig();

        writeToFile(configFile, working_dir_name + "\n");
    }
    else {
        checkedOpenFile(configFile, QIODevice::ReadOnly);
        QTextStream in(&configFile);
        parentDir = QDir(in.readLine());
        configFile.close();
    }
}

bool QNote::saveFile(QFile& file)
{
    return writeToFile(file, ui->mainTextEdit->toPlainText());
}

void QNote::on_actionNew_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("New File"), parentDir.absolutePath(),
            tr("All Files (*);;Text Files (*.txt);;RTF Files(*.rtf);;C++ Files (*.cpp *.h)"));
    if (fileName.isEmpty())
        return;
    currentFileName = fileName;
    ui->mainTextEdit->clear();

    QFile file(currentFileName);
    saveFile(file);

    QFileInfo fileInfo(file);
    QString localFileName(fileInfo.fileName());

    fileModel->addFile(localFileName);
}

void QNote::on_actionOpen_triggered()
{
    // TODO: As soon as we implement a file sys watcher, look for unsaved changes,
    // and prompt the user here instead of saving all the time
    // saveFile(currentFileName);

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), parentDir.absolutePath(),
            tr("All Files (*);;Text Files (*.txt);;RTF Files(*.rtf);;C++ Files (*.cpp *.h)"));
    if (!fileName.isEmpty()) {
        if(fileName.contains(".o", Qt::CaseInsensitive)) {
            QMessageBox::critical(this, tr("Error"), tr("Can not open this file format yet"));
            return;
        }

        updateListViewSelection(fileName);
        QFile file(fileName);
        QFileInfo fileInfo(file);
        currentFileName = file.fileName();
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

void QNote::on_actionSave_triggered()
{
    QFile file(currentFileName);
    saveFile(file);
    updateDate();
}

void QNote::on_actionSaveAs_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"),  parentDir.absolutePath(),
            tr("Text Files (*.txt);;C++ Files (*.cpp *.h)"));
    QFile file(fileName);
    saveFile(file);
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
    QFile file(currentFileName);
    // qDebug() << ui->mainTextEdit->toPlainText();
    //saveFile(file);
}

void QNote::on_listView_clicked(const QModelIndex &index)
{
    // Same as the comment in the New-slot, should only save when changes   have been made
    //    saveFile(currentFileName);
    QString fileName = parentDir.absoluteFilePath(fileModel->data(index).toString());
    if (!fileName.isEmpty()) {
        QFile file(fileName);
        if(fileName.contains(".o", Qt::CaseInsensitive)) {
            QMessageBox::critical(this, tr("Error"), tr("Could not open this file format yet"));
            return;
        }
        QFileInfo fileInfo(file);
        QString simpleFileName = fileInfo.fileName();
        currentFileName = file.fileName();

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
    if(!currentFileName.isEmpty()) {
        // rename the file
        QFile file(currentFileName);
        QFileInfo fileInfo(file);
        QString path = parentDir.absolutePath() + QDir::separator();

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
        currentFileName = newFileName;

        updateDate();
    }
}

// Simple function to update the date
void QNote::updateDate()
{
    QFile file(currentFileName);
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

    if (parentDir == fileDir) {
        QModelIndex index = fileModel->indexOf(fileInfo.fileName());

        if (index.isValid())
            ui->listView->setCurrentIndex(index);
    }
    else
        qDebug("Not in working directory, can't show in list view yet (not implemented)!");
}

// Function to write the provided data to the file
bool QNote::writeToFile(QFile& file, QString data)
{
    if (!checkedOpenFile(file, QIODevice::WriteOnly)) return false;

    QTextStream stream(&file);
    stream << data;
    stream.flush();
    file.close();

    return true;
}

// Opens and returns whether the operation was successful
bool QNote::checkedOpenFile(QFile& file, QIODevice::OpenMode mode)
{
    if (!file.open(mode)) {
        QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
        return false;
    }
    return true;
}
