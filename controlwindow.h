#ifndef CONTROLWINDOW_H
#define CONTROLWINDOW_H

#include <QWidget>

namespace Ui {
class controlWindow;
}

class controlWindow : public QWidget
{
    Q_OBJECT

public:
    explicit controlWindow(QWidget *parent = 0);
    ~controlWindow();

    void onResize(int width, int height);

    void addText(const std::string& msg);
    void addText(const std::vector<std::string>& msg);

private:
    Ui::controlWindow *ui;
};

#endif // CONTROLWINDOW_H
