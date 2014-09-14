#include "qnote.h"
#include "src_global.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QNote w;
    w.setup();
    w.show();

    return a.exec();
}
