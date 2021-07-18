#include "addthreadtest.h"

void addThreadTest::process()
{
    qDebug() << "test Thread id is: " << QThread::currentThreadId();
    calculator.encryptChoose(filePath, password);
}

