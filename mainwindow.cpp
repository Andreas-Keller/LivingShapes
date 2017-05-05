#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QTimer>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showMaximized();
    _glWidget = ui->glWidget;

    //force the glwidget to update every 1/60 second:
    QTimer* timer = new QTimer{ this };
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start();
    timer->setInterval(16);
}

MainWindow::~MainWindow()
{
    if (_glWidget) delete _glWidget;
    if (ui) delete ui;
}

void MainWindow::on_testButton_clicked()
{
    this->close();
}

void MainWindow::update() {
    _glWidget->update();
}
