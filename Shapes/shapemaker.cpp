#include "shapemaker.h"

ShapeMaker::ShapeMaker()
    : _rectangle    { nullptr },
      _triangle     { nullptr },
      _circle       { nullptr },
      _shader       { nullptr }
{}

ShapeMaker *ShapeMaker::instance() {
    static ShapeMaker instance{ };
    return &instance;
}

ShapeMaker::~ShapeMaker() {

    Q_ASSERT(_shader);

    if (_rectangle) {
        delete _rectangle;
        _rectangle = nullptr;
    }
    if (_triangle) {
        delete _triangle;
        _triangle = nullptr;
    }
    if (_circle) {
        delete _circle;
        _circle = nullptr;
    }
}

/* One problem: Color is always the same now */
Shape* ShapeMaker::get(ShapeType type) {

    switch (type) {

    case ShapeType::rectangle:
        if (!_rectangle) {
            _rectangle = new Rectangle{ _shader, QVector2D{ 1.f, 1.f }, QColor{ 250, 0, 100 } };
        }
        return _rectangle;
        break;
    case ShapeType::triangle:
        if (!_triangle) {
            _triangle = new TriangleEqualSided{ _shader, QVector2D{ 1.f, 1.f }, QColor{ 100, 0, 250 } };
        }
        return _triangle;
        break;
    case ShapeType::circle:
        if (!_circle) {
            _circle = new Circle{ _shader, 1.f, 8, QColor{ 20, 250, 100 } };
        }
        return _circle;
        break;
    default:
        return nullptr;
    }
}
