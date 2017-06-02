/*  this class is resposible for the openGL-Rendering . It needs a pointer to
    our scene and gets called from glWidget::paintGL
*/

#ifndef RENDERER_H
#define RENDERER_H

#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QOpenGLFramebufferObject>
#include <QColor>

//forward declarations:
class Scene;
class Camera;
class Rectangle;
class GaussBlur;

class Renderer {
public:
    Renderer(Scene* scene,
             int width, int height,
             const QColor& clearColor = QColor{ 0, 0, 0, 1 });
    ~Renderer();

    //we give the camera on the render call so we could easily switch it:
    virtual void draw(Camera& cam);

    virtual void resize(int width, int height);

protected:
    //private enum for indexing the shader and fbo arrays:
    enum Shaders { SCENE, LIGHTS, FINAL };

    //private init functions:
    void initGL();
    void initShaders();
    void initFramebuffers();

    //private drawing shaders:
    void drawScene(Camera& cam);
    void drawLights(Camera& cam);
    void drawFinal(Camera& cam);

    //helper functions:
    void setCameraUniforms(Camera &cam, QOpenGLShaderProgram* shader);

    Scene*       _scene;

    Rectangle*  _screenQuad; //screen-filling quad in which we draw the final image
    GaussBlur*  _gauss;

    float       _lightmapScale;

    QPoint      _dim;       //width and height of the window we draw into
    QColor      _color;     //clear-screen-color

    QOpenGLFunctions*           _gl;
    QOpenGLShaderProgram*       _shader[3]; //0: scene; 1: lights; 2: final Image
    QOpenGLFramebufferObject*   _fbo[2];    //0: scene; 1: lights

};

#endif // RENDERER_H
