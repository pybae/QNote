#include "QNote.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QNote w;
    w.show();

    return a.exec();
}
