#ifndef ADDTHREADTEST_H
#define ADDTHREADTEST_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <calculator.h>

class addThreadTest : public QObject
{
    Q_OBJECT
public:
    addThreadTest() : mData(){};

public slots:
    void process();
    void getData(QString path){
        filePath = path;
    }
    void getPassword(QString pwd){
        password = pwd;
    }

private:
    QString mData;
    QString filePath;
    QString password;
    Calculator calculator;
};

class addThreadTestInterFace : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString data WRITE setData USER false)
    Q_PROPERTY(QString password WRITE setPassword USER false)
public:
    addThreadTestInterFace() : mAddthreadtest(){
        mAddthreadtest.moveToThread(&mThread);
        connect(this, &addThreadTestInterFace::acceptMessage, &mAddthreadtest, &addThreadTest::process);
        connect(this, &addThreadTestInterFace::sendFilePath, &mAddthreadtest, &addThreadTest::getData);
        connect(this, &addThreadTestInterFace::sendPassword, &mAddthreadtest, &addThreadTest::getPassword);
        mThread.start();
    };

    void setData(QString data){
        mData = data;
        emit sendFilePath(mData);
    }
    void setPassword(QString password){
        emit sendPassword(password);
    }

    ~addThreadTestInterFace(){
        mThread.exit();
    }
signals:
    void acceptMessage();
    void sendFilePath(QString filePath);
    void sendPassword(QString password);

private:
    QString mData;
    QThread mThread;
    addThreadTest mAddthreadtest;
};
#endif // ADDTHREADTEST_H
