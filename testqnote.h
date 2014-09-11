#ifndef TESTQNOTE_H
#define TESTQNOTE_H

#include "qnote.h"

class TestQNote : public QObject
{
    Q_OBJECT

private slots:
    // Initializes the variables for the tests
    void initTestCase();

    // Cleans (recycles) variables used in the tests
    void cleanupTestCase();
};

#endif // TESTQNOTE_H
