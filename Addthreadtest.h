#ifndef ADDTHREADTEST_H
#define ADDTHREADTEST_H

#include <QObject>
#include <QThread>
#include <QDebug>

class AddThreadTest : public QObject
{
    Q_OBJECT
public:
    AddThreadTest() : mData(){};

public slots:
    void process();

private:
    QString mData;
};

class AddThreadTestInterFace : public QObject
{
    Q_OBJECT
public:
    AddThreadTestInterFace() : mAddthreadtest(){
        mAddthreadtest.moveToThread(&mThread);
        connect(this, &AddThreadTestInterFace::acceptMessage, &mAddthreadtest, &AddThreadTest::process);
        mThread.start();
    };
    ~AddThreadTestInterFace(){
        mThread.exit();
    }
signals:
    void acceptMessage();

private:
    QThread mThread;
    AddThreadTest mAddthreadtest;
};

#endif // ADDTHREADTEST_H
