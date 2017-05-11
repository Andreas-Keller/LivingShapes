/*  Class for simple 2D-Lights.

    Works with a texture that defines the gradient of the lights intensitiy. The shader will read the texture and
    multiply an objects color with the r-value of the texture.

    Inherits from GameEntity. The lights shape is always a circle
*/

#ifndef LIGHT_H
#define LIGHT_H

#include <QOpenGLTexture>
#include <QOpenGLFunctions>
#include <string>

#include "gameentity.h"
#include "Shapes/shapemaker.h"


class Light : public GameEntity {
public:
    Light(const std::string& texturePath, float radius = 1.f);
    ~Light();

    virtual void draw(QOpenGLShaderProgram* shader) override;

private:
    QOpenGLFunctions* _gl;

    QOpenGLTexture* _tex;   //texture that describes the light
    float           _r;     //radius of the point light
};

#endif // LIGHT_H
