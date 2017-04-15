#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showMaximized();
    _glWidget = ui->glWidget;

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
void MainWindow::on_zoomOutButton_clicked()
{
    _glWidget->camera().setZoom(_glWidget->camera().zoom() + 100.0);
}

void MainWindow::on_zoomInButton_clicked()
{
    _glWidget->camera().setZoom(_glWidget->camera().zoom() - 100.0);
}
