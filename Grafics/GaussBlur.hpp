/*	class to blur a texture with a two pass gaussian blur
 *  (ie., first we blur horizontal, then we blur vertical)  */

#pragma once

#include <QOpenGLFunctions>
#include <QOpenGLFramebufferObject>
#include <QOpenGLShaderProgram>

#include "Shapes/concreteshapes.h"

class GaussBlur {
public:
    GaussBlur(int width, int height);
	~GaussBlur();

    GLuint blur(int textureID, int iterations = 5);

    /* first downsample with Framebuffer Blitting, then blur: */
    GLuint blurDS(QOpenGLFramebufferObject* source, int iterations = 5);

    //to do: resize-function !!

private:
    //pointer to the openGL-Functions:
    QOpenGLFunctions*           _gl;

    //3 Framebuffers: [0, 1]: ping-pong-framebuffer; [2]: For downsampling
    QOpenGLFramebufferObject*	_fbo[3];

    //and our shader programm:
    QOpenGLShaderProgram*       _shader;

	int		_w;
	int		_h;

    Rectangle*                  _screenQuad;
};
