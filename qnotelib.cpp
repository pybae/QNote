#include "qnotelib.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QTextStream>


QDir QNoteLib::readInDefaultDirectory(QWidget *parent)
{
    QFile metadata(QDir::homePath() + QDir::separator() + ".notetakinginfo");
    QDir parent_dir;
    if (!metadata.exists()) {
        QMessageBox::warning(parent, tr("No default directory found"), tr("Please choose a default directory"));

        QFileDialog dlg(parent, tr("Default directory"));
        QString working_dir_name = dlg.getExistingDirectory(parent, tr("Default directory"),
                                                            QDir::homePath(),
                                                            QFileDialog::ShowDirsOnly
                                                            | QFileDialog::DontResolveSymlinks);
        if (!working_dir_name.isEmpty())
            parent_dir = QDir(working_dir_name);
        else {
            QMessageBox::critical(parent, tr("No default directory"), tr(""));
            exit(0);
        }
        if (!metadata.open(QIODevice::WriteOnly)) {
            QMessageBox::critical(parent, tr("Error"), tr("Could not write to file"));
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
            QMessageBox::critical(parent, tr("Error"), tr("Could not read file"));
            exit(0);
        }
        QTextStream in(&metadata);
        parent_dir = QDir(in.readLine());
        metadata.close();
    }
    return parent_dir;
}
