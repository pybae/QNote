#ifndef NOTEPAD_H
#define NOTEPAD_H

#include <QMainWindow>
#include <QDir>

namespace Ui {
class Notepad;
}

class Notepad : public QMainWindow
{
    Q_OBJECT

public:
    explicit Notepad(QWidget *parent = 0);
    ~Notepad();

private:
    QString working_file_name;
    QDir working_dir;
    QStringList file_list;

private slots:
    void on_actionOpen_triggered();

    void on_actionSaveAs_triggered();

    void on_mainTextEdit_textChanged();

    void on_actionNew_triggered();

    void on_actionSave_triggered();

    void on_actionPrint_triggered();

    void on_actionExit_triggered();

private:
    Ui::Notepad *ui;
};

#endif // NOTEPAD_H
