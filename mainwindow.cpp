#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>
#include <QtWebEngineWidgets>
#include <QMessageBox>
/*
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}
*/
MainWindow::MainWindow(const QUrl& url)
{
    setAttribute(Qt::WA_DeleteOnClose, true);
    progress = 0;
    QFile file;

    //Getting JQuery File Content
    file.setFileName(":/jquery/jquery.min.js");
    file.open(QIODevice::ReadOnly);
    jQuery = file.readAll();
    jQuery.append("\nvar qt = {'jQuery': jQuery.noConflict(true)};");
    file.close();

    view = new QWebEngineView(this);
    view->load(url);

    //Functionality for Web Browser
    connect(view, &QWebEngineView::loadFinished, this, &MainWindow::adjustLocation);
    connect(view, &QWebEngineView::titleChanged, this, &MainWindow::adjustTitle);
    connect(view, &QWebEngineView::loadProgress, this, &MainWindow::setProgress);
    connect(view, &QWebEngineView::loadFinished, this, &MainWindow::finishLoading);

    locationEdit = new QLineEdit(this);

    locationEdit->setSizePolicy(QSizePolicy::Expanding, locationEdit->sizePolicy().verticalPolicy());

    connect(locationEdit, &QLineEdit::returnPressed, this, &MainWindow::changeLocation);

    QToolBar *toolbar = addToolBar(tr("Navigation"));

    toolbar->addAction(view->pageAction(QWebEnginePage::Back));
    toolbar->addAction(view->pageAction(QWebEnginePage::Forward));
    toolbar->addAction(view->pageAction(QWebEnginePage::Reload));
    toolbar->addAction(view->pageAction(QWebEnginePage::Stop));
    toolbar->addWidget(locationEdit);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));
    QAction *viewSourceAction = new QAction(tr("Page Source"), this);
    QAction *viewGetAction = new QAction(tr("Get input value from Google.com"), this);

    connect(viewSourceAction, &QAction::triggered, this, &MainWindow::viewSource);
    connect(viewGetAction, &QAction::triggered, this, &MainWindow::getValue);

    viewMenu->addAction(viewSourceAction);
    viewMenu->addAction(viewGetAction);


    QMenu *effectMenu = menuBar()->addMenu(tr("&Effect"));

    effectMenu->addAction(tr("Highlight all links"), this, &MainWindow::highlightAllLinks);

    rotateAction = new QAction(this);
    rotateAction->setIcon(style()->standardIcon(QStyle::SP_FileDialogDetailedView));
    rotateAction->setCheckable(true);
    rotateAction->setText(tr("Turn images upside down"));

    connect(rotateAction, &QAction::toggled, this, &MainWindow::rotateImages);

    effectMenu->addAction(rotateAction);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    setCentralWidget(view);
}

void MainWindow::getValue()
{

    //QString code = QStringLiteral("qt.jQuery('input').each(function () { return qt.jQuery(this).val() })");
    //QString code = QStringLiteral("var summary = []; qt.jQuery('input[type=text]').each(function () { summary.push(qt.jQuery(this).val()) }); summary;");
    QString code = QStringLiteral("qt.jQuery('input[name=q]').val()");
    //view->page()->runJavaScript(code, [this](const QVariant &v) { v.toStringList().at(0);  });
    view->page()->runJavaScript(code, [this](const QVariant &v) { qDebug()<<v.toString(); });
    //view->page()->runJavaScript("function getelement(){return $('#elementid').val();} getelement();",[this](const QVariant &v) { qDebug()<<v.toString();});

    //QMessageBox::information(this, "Sample", value);
}

void MainWindow::viewSource()
{
    QTextEdit *textEdit = new QTextEdit(nullptr);
    textEdit->setAttribute(Qt::WA_DeleteOnClose);
    textEdit->adjustSize();
    textEdit->move(this->geometry().center() - textEdit->rect().center());
    textEdit->show();

    view->page()->toHtml([textEdit](const QString &html){
        textEdit->setPlainText(html);
    });
}

void MainWindow::adjustLocation()
{
    locationEdit->setText(view->url().toString());
}

void MainWindow::changeLocation()
{
    QUrl url = QUrl::fromUserInput(locationEdit->text());
    view->load(url);
    view->setFocus();
}

void MainWindow::adjustTitle()
{
    if (progress == 0 || progress >= 100)
        setWindowTitle(view->title());
    else
        setWindowTitle(QStringLiteral("%1(%2%)").arg(view->title()).arg(progress));
}

void MainWindow::setProgress(int p)
{
    progress = p;
    adjustTitle();
}

void MainWindow::finishLoading(bool)
{
    progress = 100;
    adjustTitle();
    view->page()->runJavaScript(jQuery);

    rotateImages(rotateAction->isChecked());
}

void MainWindow::highlightAllLinks()
{
    QString code = QStringLiteral("qt.jQuery('a').each(function () { qt.jQuery(this).css('background-color', 'yellow') })");
    view->page()->runJavaScript(code);
}

void MainWindow::rotateImages(bool invert)
{
    QString code;

    if (invert)
            code = QStringLiteral("qt.jQuery('img').each( function () { qt.jQuery(this).css('transition', 'transform 2s'); qt.jQuery(this).css('transform', 'rotate(180deg)') } )");
        else
            code = QStringLiteral("qt.jQuery('img').each( function () { qt.jQuery(this).css('transition', 'transform 2s'); qt.jQuery(this).css('transform', 'rotate(0deg)') } )");


    view->page()->runJavaScript(code);
}


