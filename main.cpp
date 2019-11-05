#include "mainwindow.h"

#include <QApplication>
#include <QtWebEngine>
int main(int argc, char *argv[])
{
    //QApplication a(argc, argv);

    //QtWebEngine::initialize();

    //MainWindow w;
    //w.show();

    //QQmlApplicationEngine engine;
    //engine.load(QUrl(""))
    //return a.exec();



    QCoreApplication::setOrganizationName("QtExamples");
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QApplication a(argc, argv);

    QUrl url;
    if (argc > 1)
        url = QUrl::fromUserInput(argv[1]);
    else
        url = QUrl("http://www.google.com/ncr");
    MainWindow *browser = new MainWindow(url);
    browser->resize(1024, 768);
    browser->show();

    return a.exec();
}
