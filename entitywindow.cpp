#include "entitywindow.h"
#include "ui_entitywindow.h"

#include "Entities/gameentity.h"
#include "Entities/movingentity.h"

EntityWindow::EntityWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::EntityWindow),
    _ent{ nullptr }
{
    ui->setupUi(this);
    ui->forceBox->setMinimum(0.1f);
    ui->forceBox->setMaximum(100.f);
    ui->massBox->setMinimum(0.1f);
    ui->massBox->setMaximum(200.f);
}

EntityWindow::~EntityWindow()
{
    delete ui;
}

void EntityWindow::sendShapeInfo(GameEntity *entity) {

    if (entity->type() == EntType::moving) {

        MovingEntity* movEnt = static_cast<MovingEntity*>(entity);
        _ent = movEnt;

        //set the checkboxes to their appropriate state:
        ui->ArriveBox->setChecked(movEnt->steering()->isArriveOn());
        ui->WanderBox->setChecked(movEnt->steering()->isWanderOn());
        ui->ObstacleBox->setChecked(movEnt->steering()->isObstacleAvoidanceOn());

        //set proper values for the spinboxes:
        ui->forceBox->setValue(_ent->maxForce());
        ui->massBox->setValue(_ent->mass());
    }
}

void EntityWindow::on_WanderBox_stateChanged(int arg1) {

    if (!_ent) return;
    if (arg1 == Qt::Unchecked) _ent->steering()->wanderOff();
    if (arg1 == Qt::Checked) _ent->steering()->wanderOn();
}


void EntityWindow::on_ArriveBox_stateChanged(int arg1) {

    if (!_ent) return;
    if (arg1 == Qt::Checked) _ent->steering()->arriveOn();
    if (arg1 == Qt::Unchecked) _ent->steering()->arriveOff();
}

void EntityWindow::on_ObstacleBox_stateChanged(int arg1) {

    if (!_ent) return;
    if (arg1 == Qt::Checked) _ent->steering()->obstacleAvoidanceOn();
    if (arg1 == Qt::Unchecked) _ent->steering()->obstacleAvoidanceOff();

}

void EntityWindow::on_massBox_valueChanged(double arg1) {
    if (_ent) _ent->setMass(arg1);
}

void EntityWindow::on_forceBox_valueChanged(double arg1) {
   if (_ent) _ent->setMaxForce(arg1);
}
