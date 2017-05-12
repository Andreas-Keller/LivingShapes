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

#include "Entities/gameentity.h"
#include "Entities/light.h"
#include "camera.h"


class GLWidget : public QOpenGLWidget {

public:
    /*  @param parent: The parent window in which the canvas is drawn */
    GLWidget(QWidget* parent, QColor clearColor = QColor(0, 0, 0));

    /* Handle mouse events: */
    virtual void mousePressEvent(QMouseEvent *event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent *event) override;
    virtual void wheelEvent(QWheelEvent *event) override;

    //getters for the ui:
    Camera& camera() { return _cam; }

protected:
    /* inherited from QOpenGLWidget: */
    virtual void    initializeGL() override;
    virtual void    paintGL() override;
    virtual void    resizeGL(int width, int height) override;

private:
    void initShaders();
    void drawLights();
    void drawScene();
    void drawFinal();
    void setCameraUniforms(QOpenGLShaderProgram* shader);

    QOpenGLFunctions*   _gl;    //pointer to openGL-Functions
    QColor              _color; //background color of the widget

    Camera              _cam;

    QPoint              _mousePos;

    //for drawing the ligths off-screen:
    QOpenGLFramebufferObject* _fbLight;
    QOpenGLFramebufferObject* _fbScene;

    Rectangle*                _screenQuad; //a quad filling the screen, needed to draw the framebuffers into


    //TESTCODE:
    QOpenGLShaderProgram*    _shader;      //shader for the final on-screen-drawing
    QOpenGLShaderProgram*    _lightShader; //shader for drawing the lights
    QOpenGLShaderProgram*    _sceneShader; //shader for drawing the scene objects
    std::vector<GameEntity*> _entities;
    std::vector<Light*>      _lights;
};

#endif // GLWIDGET_H
