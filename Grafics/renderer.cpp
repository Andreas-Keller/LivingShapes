#include "renderer.h"

#include "Scene.h"
#include "camera.h"
#include "Shapes/concreteshapes.h"
#include "Grafics/GaussBlur.hpp"
#include "Shapes/shapemaker.h"

Renderer::Renderer(Scene *scene, int width, int height, const QColor &clearColor)
    :   _scene  { scene },
		_screenQuad { nullptr },
		_gauss 	{ nullptr },
		_lightmapScale { 0.5f },
        _dim    { QPoint{ width, height } },
        _color  { clearColor },
        _gl     { QOpenGLContext::currentContext()->functions() },
        _fbo    { nullptr }

{
    Q_ASSERT(scene && _gl);

    initGL();
    initShaders();
    initFramebuffers();

    //give the shape shader to our Shapemaker:
    ShapeMaker::instance()->setShader(_shader[SCENE]);

    //initialize screen Quad:
    _screenQuad = new Rectangle{ _shader[SCENE], QVector2D{ 2.f, 2.f } };

    //we leave the gaussian blur uninitialized since we do not need it (yet)
}


Renderer::~Renderer() {

}

void Renderer::draw(Camera& cam) {
    drawLights(cam);
    drawScene(cam);
    drawFinal(cam);
}

void Renderer::resize(int width, int height) {
    _dim.setX(width);
    _dim.setY(height);

    _gl->glViewport(0, 0, width, height);

    //we need to resize the framebuffers:
    if (_fbo[0]) delete _fbo[0];
    if (_fbo[1]) delete _fbo[1];
    initFramebuffers();
}

void Renderer::drawScene(Camera& cam) {

    _fbo[SCENE]->bind();

    _gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    /*
    //draw lights again with 1/3 of size so we can see them in empty areas:
    _lightShader->bind();
    for (auto& L : _lights) {
        L->transform()->scale(2.f);
        L->draw(_lightShader);
        L->transform()->scale(4.f);
    }
    _lightShader->release();*/

    //bind shader and set camera matrices:
    _shader[SCENE]->bind();
    setCameraUniforms(cam, _shader[SCENE]);

    //draw our gameEntities:
    _scene->drawShapes(_shader[SCENE]);

    _shader[SCENE]->release();
    _fbo[SCENE]->release();
}

void Renderer::drawLights(Camera& cam) {
    _fbo[LIGHTS]->bind();
    _gl->glViewport(0, 0, _fbo[LIGHTS]->width(), _fbo[LIGHTS]->height());
    _gl->glEnable(GL_BLEND);
    _gl->glBlendFunc(GL_ONE, GL_ONE); //additive blending for the lights
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    _shader[LIGHTS]->bind();

    setCameraUniforms(cam, _shader[LIGHTS]);

    //the actual draw-call:
    _scene->drawLights(_shader[LIGHTS]);

    _shader[LIGHTS]->release();
    _fbo[LIGHTS]->release();
    _gl->glViewport(0, 0, _dim.x(), _dim.y());
}

void Renderer::drawFinal(Camera& cam) {
    _gl->glDisable(GL_BLEND);
    _gl->glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //test: gauss blur
    //int text = _gauss->blurDS(_fbScene, 1);
    //_gl->glBindFramebuffer(GL_FRAMEBUFFER, 0); //why necessary?
    //_gl->glViewport(0, 0, this->width(), this->height());

    _shader[FINAL]->bind();

    //bind the textures from our two framebuffers:
    int sceneTex = _shader[FINAL]->uniformLocation("sceneTex");
    int lightTex = _shader[FINAL]->uniformLocation("lightTex");

    _gl->glActiveTexture(GL_TEXTURE0);
    _gl->glBindTexture(GL_TEXTURE_2D, _fbo[SCENE]->texture());
    _gl->glUniform1i(sceneTex, 0);

    _gl->glActiveTexture(GL_TEXTURE1);
    _gl->glBindTexture(GL_TEXTURE_2D, _fbo[LIGHTS]->texture());
    _gl->glUniform1i(lightTex, 1);

    _screenQuad->draw(_shader[FINAL]);

    _shader[FINAL]->release();
}

void Renderer::initGL() {
    _gl->glClearColor(_color.redF(), _color.greenF(), _color.blueF(), _color.alphaF());
    _gl->glViewport(0, 0, _dim.x(), _dim.y());
    _gl->glEnable(GL_BLEND);
    _gl->glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void Renderer::initShaders() {
    _shader[FINAL] = new QOpenGLShaderProgram();
    _shader[FINAL]->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/final.vert.glsl");
    _shader[FINAL]->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/final.frag.glsl");
    _shader[FINAL]->link();

    //Create the light shader:
    _shader[LIGHTS] = new QOpenGLShaderProgram();
    _shader[LIGHTS]->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/lights.vert.glsl");
    _shader[LIGHTS]->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/lights.frag.glsl");
    _shader[LIGHTS]->link();

    //Create the scene shader:
    _shader[SCENE] = new QOpenGLShaderProgram();
    _shader[SCENE]->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/scene.vert.glsl");
    _shader[SCENE]->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/scene.frag.glsl");
    _shader[SCENE]->link();
}

void Renderer::initFramebuffers() {
    _fbo[LIGHTS] = new QOpenGLFramebufferObject{
            int(_dim.x() * _lightmapScale),
            int(_dim.y() * _lightmapScale) };
    _fbo[SCENE] = new QOpenGLFramebufferObject{
            _dim.x(), _dim.y() };

    Q_ASSERT(_fbo[LIGHTS]->isValid());
    Q_ASSERT(_fbo[SCENE]->isValid());
}

void Renderer::setCameraUniforms(Camera& cam, QOpenGLShaderProgram* shader) {
    _gl->glUniformMatrix4fv(shader->uniformLocation("O"), 1, GL_FALSE, cam.orthoMatrix().data());
    _gl->glUniformMatrix4fv(shader->uniformLocation("V"), 1, GL_FALSE, cam.viewMatrix().data());
}
