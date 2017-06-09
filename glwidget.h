/*  Class GLWidget
    Task: Acts as a canvas for the openGL-Rendering
    Inherits from QOpenGLWidget
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>

#include <vector>

#include "Grafics/renderer.h"

#include "Scene.h"
#include "camera.h"
#include "controlwindow.h"
#include "entitywindow.h"
#include "objpicker.h"

//convenience macro used in the destructor:
#define DEL(x) { if(x) { delete x; x = nullptr; }; }

class GLWidget : public QOpenGLWidget {

public:
    /*  @param parent: The parent window in which the canvas is drawn */
    GLWidget(QWidget* parent);
    ~GLWidget();

    /* Handle mouse events: */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

    //getters for the ui:
    Camera& camera() { return _cam; }

protected:
    /* inherited from QOpenGLWidget: */
    virtual void    initializeGL() override;
    virtual void    paintGL() override;
    virtual void    resizeGL(int width, int height) override;

    //init the debug and the entity window:
    void initDebugWin();
    void initEntityWin();

    //sends shapes data to the debug win if we picked an object:
    void sendShapeInfo(GameEntity *entity);

private:
    Camera              _cam;
	Renderer* 			_renderer;

    QPoint              _mousePos;

    //window for debugging (and later maybe for user-options)
    controlWindow*            _debugWin;

    //window that pops up when we click on an entity:
    EntityWindow*             _entityWin;

    //For object picking with the mouse:
    ObjPicker                 _picker;

	//the scene keeps track of all shapes:
    Scene                    _scene;

    //MORE TESTS: CREATE NEW ENTITYS ON MOUSE CLICK:
    std::vector<QVector3D>   _newEntPos;
};

#endif // GLWIDGET_H
