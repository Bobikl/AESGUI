#ifndef ADDTHREAD_H
#define ADDTHREAD_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <calculator.h>

class addThread : public QObject
{
    Q_OBJECT
public:
    addThread();

public slots:
    void process(int x);
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

class addThreadInterFace : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString data WRITE setData)
    Q_PROPERTY(QString password WRITE setPassword)
public:
    addThreadInterFace(){
        mAddthread.moveToThread(&mThread);
        connect(this, &addThreadInterFace::sendFilePath, &mAddthread, &addThread::getData);
        connect(this, &addThreadInterFace::sendPassword, &mAddthread, &addThread::getPassword);
        connect(this, &addThreadInterFace::acceptMessage, &mAddthread, &addThread::process);
        mThread.start();
    }

    void startThread(){

    }
    void setData(QString data){
        mData = data;
        emit sendFilePath(mData);
    }
    void setPassword(QString password){
        emit sendPassword(password);
    }

    ~addThreadInterFace(){
        mThread.exit();
    }
signals:
    void acceptMessage(int x);
    void sendFilePath(QString filePath);
    void sendPassword(QString password);

private:
    QString mData;
    QThread mThread;
    addThread mAddthread;
};
#endif // ADDTHREAD_H
