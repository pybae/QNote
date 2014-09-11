#include "testqnote.h"
#include "qnote.h"

void TestQNote::initTestCase()
{
    QNote w;
    w.setup();
    w.show();

    return;
}

void TestQNote::cleanupTestCase()
{
    return;
}
