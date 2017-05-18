#include "GaussBlur.hpp"


GaussBlur::GaussBlur(int w, int h)
    :	_gl     { nullptr },
        _shader { nullptr },
        _w      { w },
        _h      { h },
        _screenQuad { nullptr }

{
    //get openGL-Functions:
    _gl = QOpenGLContext::currentContext()->functions();

    //setup the three framebuffers:
    for (size_t i = 0; i < 3; i++) {
        _fbo[i] = new QOpenGLFramebufferObject { _w, _h };
        Q_ASSERT(_fbo[i]->isValid());
    }

    //setup the shader:
    _shader = new QOpenGLShaderProgram();
    _shader->addShaderFromSourceFile(QOpenGLShader::Vertex, "Shaders/gaussianBlur.vert.glsl");
    _shader->addShaderFromSourceFile(QOpenGLShader::Fragment, "Shaders/gaussianBlur.frag.glsl");
    _shader->link();

    //setup a viewport filling rectangle:
    _screenQuad = new Rectangle{ _shader, QVector2D{ 2.f, 2.f} };
}


GaussBlur::~GaussBlur() {
    //to do: clean up
}


GLuint GaussBlur::blur(int textureID, int iterations) {

    _gl->glViewport(0, 0, _w, _h); //need to reset to origin when finished!

    bool horizontal     { true };
    bool firstIteration { true };


    for (int i = 0; i < iterations * 2; i++) {
        _fbo[horizontal]->bind();
        _shader->bind();
        _gl->glUniform1i(_shader->uniformLocation("horizontal"), horizontal);

        //get the right texture:
        int textureToBind = firstIteration? textureID : _fbo[!horizontal]->texture();

        _gl->glActiveTexture(GL_TEXTURE0);
        _gl->glBindTexture(GL_TEXTURE_2D, textureToBind);
        _gl->glUniform1i(_shader->uniformLocation("image"), 0);

        _screenQuad->draw(_shader);

        horizontal = !horizontal; //ping-pong
        firstIteration = false;
    }
    _fbo[!horizontal]->release();

    return _fbo[!horizontal]->texture();
}


GLuint GaussBlur::blurDS(QOpenGLFramebufferObject* source, int iterations)
{
    QOpenGLFramebufferObject::blitFramebuffer(
        _fbo[2],
        QRect(0, 0, _fbo[2]->width(), _fbo[2]->height()),
        source,
        QRect(0, 0, source->width(), source->height()),
        GL_COLOR_BUFFER_BIT,
        GL_LINEAR,
        0, 0,
        QOpenGLFramebufferObject::FramebufferRestorePolicy::DontRestoreFramebufferBinding);

    return blur(_fbo[2]->texture(), iterations);
     /* we downsample to the size of the GaussBlur Framebuffers:
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
	glReadBuffer(GL_COLOR_ATTACHMENT0 + colorAttachment);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, _fbo[2]);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
	glBlitFramebuffer(0, 0, originSize.x, originSize.y, 0, 0, _w, _h, GL_COLOR_BUFFER_BIT, GL_LINEAR);

    return blur(_tbo[2], iterations); */
}
