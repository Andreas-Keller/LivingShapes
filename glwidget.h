/*  Class GLWidget
    Task: Acts as a canvas for the openGL-Rendering
    Inherits from QOpenGLWidget
*/

#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QOpenGLFunctions>
#include <QOpenGLWidget>
#include <QOpenGLShaderProgram>

#include <vector>

#include "Entities/gameentity.h"
#include "camera.h"


class GLWidget : public QOpenGLWidget
{
public:
    /*  @param parent: The parent window in which the canvas is drawn */
    GLWidget(QWidget* parent, QColor clearColor = QColor(0, 0, 0));

    //getters for the ui:
    Camera& camera() { return _cam; }

protected:
    /* inherited from QOpenGLWidget: */
    virtual void    initializeGL() override;
    virtual void    paintGL() override;
    virtual void    resizeGL(int width, int height) override;

private:
    QOpenGLFunctions*   _gl;    //pointer to openGL-Functions
    QColor              _color; //background color of the widget

    Camera              _cam;

    //TESTCODE:
    QOpenGLShaderProgram*    _shader;
    std::vector<GameEntity*> _entities;
};

#endif // GLWIDGET_H
