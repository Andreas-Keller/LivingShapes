#include "controlwindow.h"
#include "ui_controlwindow.h"


controlWindow::controlWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::controlWindow)
{
    ui->setupUi(this);
    ui->debugBox->resize(this->size());
    ui->debugBox->move(0, 0);
}

controlWindow::~controlWindow()
{
    delete ui;
}

void controlWindow::onResize(int width, int height)
{

    int x = ((width / 5) > 100)? width / 5 : 100;
    int y = height;

    resize(x, y);

    //move the widget on the right side of its parent:
    move(QPoint{ width - x, height - y } );

    //resize the text-box to cover the full widget:
    ui->debugBox->resize(x, y);
}

void controlWindow::addText(const std::string &msg) {
    ui->debugBox->append(QString::fromStdString(msg));
}

void controlWindow::addText(const std::vector<std::__cxx11::string> &msg)
{
    for (auto& S : msg) {
        addText(S);
    }
}
