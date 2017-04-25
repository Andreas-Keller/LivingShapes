#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "glwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void loop();

private slots:
    void update();

    void on_testButton_clicked();

    void on_zoomOutButton_clicked();

    void on_zoomInButton_clicked();

private:
    Ui::MainWindow *ui;
    GLWidget* _glWidget;
};

#endif // MAINWINDOW_H
