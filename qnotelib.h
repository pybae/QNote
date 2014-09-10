#ifndef QNOTELIB_H
#define QNOTELIB_H

#include <QDir>

class QNoteLib : QObject
{
private:
    QNoteLib(){};
public:
    static QDir readInDefaultDirectory(QWidget *parent);

};

#endif // QNOTELIB_H
