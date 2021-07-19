#include "addthreadtest.h"

addThreadTest::addThreadTest()
{

}

void addThreadTest::process()
{
    qDebug() << "add Thread id is: " << QThread::currentThreadId();
    calculator.encryptChoose(filePath, password);
}

