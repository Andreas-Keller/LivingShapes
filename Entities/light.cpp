#include "light.h"

Light::Light(const std::string& texturePath, const QVector3D& pos, float radius)
    :   GameEntity  { ShapeMaker::instance()->get(ShapeType::circle) },
        _gl         { nullptr },
        _tex        { nullptr },
        _r          { radius }
{
    QImage img{ QString::fromStdString("Textures/" + texturePath + ".png") };

    //make sure the image was loaded:
    if (img.isNull()) qDebug() << "Warning: Could not load texture from file " << "Textures/" << texturePath.c_str() << ".png";

    _tex = new QOpenGLTexture(img);
    _tex->create();
    _gl = QOpenGLContext::currentContext()->functions();
    _transform.scale(_r);
    _transform.setPos(pos);

    Q_ASSERT(_tex->isCreated());
}


Light::~Light() {
    if (_tex) {
        delete _tex;
        _tex = nullptr;
    }
}

void Light::draw(QOpenGLShaderProgram* shader) {
    _shape->setMatrix(_transform.matrix());

    _gl->glActiveTexture(GL_TEXTURE0);
    _gl->glBindTexture(GL_TEXTURE_2D, _tex->textureId());

    _shape->draw(shader);
}
