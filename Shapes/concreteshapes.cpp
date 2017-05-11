#include "concreteshapes.h"

#include <math.h>

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



/* ---------------------------------------------------------------------------------------------------- */


TriangleEqualSided::TriangleEqualSided(
        QOpenGLShaderProgram *shader,
        const QVector2D &widthAndHeight,
        const QColor &color
        )
    : Shape{ shader },
       _widthAndHeight{ widthAndHeight }
{
    /* same as above: */
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    initVertices(vertices, indices, color);
    initBuffers(vertices, indices);

    _vertices = vertices;

    _numVertices = indices.size();
}


TriangleEqualSided::~TriangleEqualSided()
{
    _vao.destroy();
    _ebo.destroy();
    _vbo.destroy();
}

void TriangleEqualSided::initVertices(std::vector<Vertex> &vertices, std::vector<int> &indices, const QColor &color)
{
    float w = _widthAndHeight.x();
    float h = _widthAndHeight.y();
    vertices.resize(3);

    //the triangle is constructed so that its center is the center of mass:
    vertices[0].pos = QVector3D{ -w / 2.0f, -h / 3.0f, 0.0 };
    vertices[1].pos = QVector3D{ w / 2.0f, -h / 3.0f, 0.0 };
    vertices[2].pos = QVector3D{ 0, h * 2.0f / 3.0f, 0.0 };

    //color
    vertices[0].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };
    vertices[1].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };
    vertices[2].rgb = QVector3D{ float(color.redF()), float(color.greenF()), float(color.blueF()) };

    //indices (kind of superfluos here...)
    indices.resize(3);
    indices[0] = 0; indices[1] = 1; indices[2] = 2;
}


Circle::Circle(QOpenGLShaderProgram *shader, float radius, int segments, const QColor &color)
    : Shape { shader },
      _r    { radius },
      _segments{ segments }
{
    /* same as above: */
    std::vector<Vertex> vertices;
    std::vector<int> indices;

    initVertices(vertices, indices, color);
    initBuffers(vertices, indices);

    _vertices = vertices;

    _numVertices = indices.size();
}

Circle::~Circle()
{
    _vao.destroy();
    _ebo.destroy();
    _vbo.destroy();
}

void Circle::initVertices(std::vector<Vertex> &vertices, std::vector<int> &indices, const QColor &color)
{
    //initial vertices:
    vertices.resize(3);
    vertices[0].pos = QVector3D{ 0.f, 0.f, 0.f };
    vertices[1].pos = QVector3D{ 1.f, 0.f, 0.f };
    vertices[2].pos = QVector3D{ 0.f, 1.f, 0.f };


    /*  we now have the following:

        A (v2)                                          v3      (v1+v3)
        |                                               +------+
        |                       segmentBreakup()        | / v2 |
        |                       ---------------->       |/     |
        +------- B (v1)                                 +------v1
        M (v0)                                          v0


        We now add the vectors MA and MB and normalize it. Since the circles radius is always one, this
        normalized vector will always be on the circle. We do this so often as we have segments.
    */

    for (size_t i = 0; i < _segments; i++) {
        segmentBreakup(vertices);
    }

    //mirror on y-axis
    int index = vertices.size();
    for (size_t i = 0; i < index; i++) {
        Vertex mirrored = vertices[i];
        mirrored.pos.setX( -vertices[i].pos.x());
        vertices.push_back(mirrored);
    }

    //mirror on x-Axis:
    index = vertices.size();
    for (size_t i = 0; i < index; i++) {
        Vertex mirrored = vertices[i];
        mirrored.pos.setY( -vertices[i].pos.y());
        vertices.push_back(mirrored);
    }

    //add the indices:
    for (size_t i = 1; i < vertices.size()-1; i++) {
        indices.push_back(0); //every triangle starts from the middle point
        indices.push_back(i);
        indices.push_back(i + 1);
    }

    //colors:
    float r = color.redF();
    float g = color.greenF();
    float b = color.blueF();
    for (auto& vertex : vertices) {
        vertex.rgb = QVector3D{ r, g, b };
    }

    //add uv-coordinates:
    for (auto& V : vertices) {
        V.uv.setX(0.5f * (V.pos.x() + 1.f)); //bring from range [-1/1] to [0/1]
        V.uv.setY(0.5f * (V.pos.y() + 1.f));
    }
}


void Circle::segmentBreakup(std::vector<Vertex>& vertices) {

    std::vector<Vertex> temp = vertices;
    vertices.clear();
    vertices.resize(0);
    vertices.push_back(temp[0]);

    for (size_t i = 1; i < temp.size()-1; i++) {
        Vertex middle;
        middle.pos = (temp[i].pos + temp[i + 1].pos);
        middle.pos.normalize();
        vertices.push_back(temp[i]);
        vertices.push_back(middle);
    }

    vertices.push_back(temp.back());
}
