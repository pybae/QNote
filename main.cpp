#include "notepad.h"
#include <QApplication>
#include <QListView>
#include <fileviewmodel.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QListView fileView;

    FileViewModel fileModel(0);
    fileView.setModel(&fileModel);
    fileView.show();

    Notepad w;
    w.show();

    return a.exec();
}
