#ifndef ENTITYWINDOW_H
#define ENTITYWINDOW_H

#include <QWidget>

class GameEntity;
class MovingEntity;

namespace Ui {
class EntityWindow;
}

class EntityWindow : public QWidget
{
    Q_OBJECT

public:
    explicit EntityWindow(QWidget *parent = 0);
    ~EntityWindow();

    void sendShapeInfo(GameEntity* entity);

private slots:
    void on_WanderBox_stateChanged(int arg1);

    void on_ArriveBox_stateChanged(int arg1);

    void on_ObstacleBox_stateChanged(int arg1);


    void on_massBox_valueChanged(double arg1);

    void on_forceBox_valueChanged(double arg1);

private:
    Ui::EntityWindow *ui;

    MovingEntity* _ent;
};

#endif // ENTITYWINDOW_H
