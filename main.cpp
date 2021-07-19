#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <calculator.h>
#include <addthreadtest.h>
int main(int argc, char *argv[])
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    app.setOrganizationName("File Path");
    app.setOrganizationDomain("filepath.com");
    app.setApplicationName("AES Application");

    const QUrl url(QStringLiteral("qrc:/main.qml"));

    qDebug() << "main thread id is: " << QThread::currentThreadId();
    qmlRegisterType<Calculator>("AesCalculator", 1, 0, "AES");
    qmlRegisterType<addThreadTestInterFace>("AddThread", 1, 0, "ThreadTest");

    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
