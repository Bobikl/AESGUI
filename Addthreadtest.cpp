#include "Addthreadtest.h"


void AddThreadTest::process()
{
    qDebug() <<  "test id: " << QThread::currentThreadId();
}
