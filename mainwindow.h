#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
//#include <QtWebEngineWidgets/QWebEngineView>
//#include <QtWebEngine>
//#include <QtWebEngineWidgets>
#include <QtWidgets>
/*
QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE
*/

QT_BEGIN_NAMESPACE
class QWebEngineView;
class QLineEdit;
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    //MainWindow(QWidget *parent = nullptr);
    //~MainWindow();
    MainWindow(const QUrl& url);

protected slots:

    void adjustLocation();
    void changeLocation();
    void adjustTitle();
    void setProgress(int p);
    void finishLoading(bool);

    void viewSource();

    void highlightAllLinks();
    void rotateImages(bool invert);
    void getValue();

private:
    //Ui::MainWindow *ui;
    QString jQuery, value;
    QWebEngineView *view;
    QLineEdit *locationEdit;
    QAction *rotateAction;
    int progress;
};
#endif // MAINWINDOW_H
