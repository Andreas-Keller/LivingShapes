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

    loop();
}

MainWindow::~MainWindow()
{
    if (_glWidget) delete _glWidget;
    if (ui) delete ui;
}

void MainWindow::loop()
{

}

void MainWindow::on_testButton_clicked()
{
    this->close();
}

//Test buttons, this is better done with mouse wheel input
void MainWindow::update() {
    _glWidget->update();
}

void MainWindow::on_zoomOutButton_clicked()
{
    _glWidget->camera().setZoom(_glWidget->camera().zoom() + 100.0);
}

void MainWindow::on_zoomInButton_clicked()
{
    _glWidget->camera().setZoom(_glWidget->camera().zoom() - 100.0);
}
