#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QMainWindow::showMaximized();
    _glWidget = ui->glWidget;
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
