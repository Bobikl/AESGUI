#include "addthread.h"

addThread::addThread()
{

}

void addThread::process(int x)
{
//    qDebug() << "add Thread id is: " << QThread::currentThreadId();
    if (x == 0){
        calculator.encryptChooseFile(filePath, password);
    } else {
        calculator.decryptChooseFile(filePath, password);
    }
}

