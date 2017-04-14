#include "concreteshapes.h"


Rectangle::Rectangle(QOpenGLShaderProgram* shader,
        const QVector2D &widthAndHeight,
        const QColor &color
        )
    : Shape{ shader },
      _widthAndHeight{ widthAndHeight }

{
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    initVertices(vertices, indices, color);
    initBuffers(vertices, indices);

    //we may need the edge points:
    _vertices = vertices;

    //and we certainly need the number of indices:
    _numVertices = indices.size();
}

Rectangle::~Rectangle()
{
    _vao.destroy();
    _ebo.destroy();
    _vbo.destroy();
}

void Rectangle::initVertices(std::vector<Vertex>& vertices,
                             std::vector<int>& indices,
                             const QColor& color)
{
    //calculate the edge points:
    float offsetX = _widthAndHeight.x() * 0.5;
    float offsetY = _widthAndHeight.y() * 0.5;
    float offsetZ = 0.0;

    vertices.resize(4);
    vertices[0].pos = QVector3D{ -offsetX, -offsetY, offsetZ }; //bottom left
    vertices[1].pos = QVector3D{ offsetX, -offsetY, offsetZ };  //bottom right
    vertices[2].pos = QVector3D{ offsetX, offsetY, offsetZ };   //top right
    vertices[3].pos = QVector3D{ -offsetX, offsetY, offsetZ };  //top left

    //assign color (we may support gradients in the future):
    vertices[0].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };
    vertices[1].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };
    vertices[2].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };
    vertices[3].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };

    //assign the indices:
    indices.resize(6);
    indices[0] = 0; indices[1] = 1; indices[2] = 3; //first triangle
    indices[3] = 1; indices[4] = 2; indices[5] = 3; //second triangle
}
