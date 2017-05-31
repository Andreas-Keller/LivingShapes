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

#include "Scene.h"
#include "Grafics/GaussBlur.hpp"
#include "camera.h"
#include "controlwindow.h"


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

    //init the debug window:
    void initDebugWin();

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

    controlWindow*            _debugWin;

    //TESTCODE:
    QOpenGLShaderProgram*    _shader;      //shader for the final on-screen-drawing
    QOpenGLShaderProgram*    _lightShader; //shader for drawing the lights
    QOpenGLShaderProgram*    _sceneShader; //shader for drawing the scene objects
    Scene                    _scene;

    //MORE TESTS: CREATE NEW ENTITYS ON MOUSE CLICK:
    std::vector<QVector3D>   _newEntPos;

    //gauss Blur:
    GaussBlur*               _gauss;

    //how big the lightmap is compared to the viewport (decrease for better performance)
    float                    _lightmapScale;
};

#endif // GLWIDGET_H
