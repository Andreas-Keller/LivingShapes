/*  Class Transform
    holds position, scale and rotation and let us extract a matrix containing these three elements

    all functions are inlined in the header file
*/


#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <math.h>

#include <QVector3D>
#include <QMatrix4x4>
#include <QQuaternion>

//purely for debugging:
#include "winmsg.h"

//Math helper function: get the translation part from a matrix:
inline QVector2D getTranslation(const QMatrix4x4& M) {
    return QVector2D{ M.column(3) };
}

//Put a translation into a matrix:
inline void putTranslation(QMatrix4x4& M, const QVector3D& value) {
    M.setColumn(3, QVector4D{ value.x(), value.y(), value.z(), 1.f });
}

//Math helper: Get the perpendicular vector (ONLY FOR 2D !!)
inline QVector3D getNormal(const QVector3D& vector) {
    return QVector3D{ -vector.y(), vector.x(), vector.z() };
}

//Get degrees from radians:
inline float toDeg(float angle) {
    return 180.f * angle / M_PI;
}

//Get radians from degrees:
inline float toRad(float angle) {
    return angle * M_PI / 180.f;
}

class Transform {
public:
    Transform() : _scale{ QVector3D(1.f, 1.f, 1.f) }, _isMvalid { false }
	{ _M.setToIdentity();  }
    ~Transform() {}

    /* setters: */
    void setPos (const QVector3D& pos)   { _pos = pos; putTranslation(_M, _pos);}
    void move   (const QVector3D& val)   { _pos += val; putTranslation(_M, _pos); }

    void scale  (float val)              { _scale = QVector3D{ val, val, val }; _isMvalid = false; }
    void scale  (const QVector3D& val)   { _scale = val; _isMvalid = false; }

    void setRotation(float angle, const QVector3D& axis) {
        _rot = QQuaternion::fromAxisAndAngle(axis, angle);
        _isMvalid = false;
        QVector3D ax;
        float an;
        _rot.getAxisAndAngle(&ax, &an);
    }
    void addRotation(float angle, const QVector3D& axis) {
        QQuaternion q = QQuaternion::fromAxisAndAngle(axis, angle);
        _rot = _rot * q;
        _rot.normalize();
        _isMvalid = false;
    }

    //rotate to look at a given direction (2D only) (BUGGED!!):
    void lookAt(const QVector2D& target, bool tracker = false) {
        //find out in which quadrant we are:
        int quadrant = 0;
        if (target.x() >= 0 && target.y() >= 0) quadrant = 1;
        else if (target.x() >= 0 && target.y() < 0) quadrant = 4;
        else if (target.x() < 0 && target.y() >= 0) quadrant = 2;
        else if (target.x() < 0 && target.y() < 0) quadrant = 3;

        //get the atangens of our target:
        float angle = toDeg(atan(target.x() / target.y()));

        //debugging:
        if (tracker) {
            WinMsg::add("<i>Internal data from Function <Transform::lookAt>:</i>");
            WinMsg::add("Direction Vector:\t " +
                        std::to_string(target.x()) + " / " +
                        std::to_string(target.y()));
            WinMsg::add("Direction angle:\t" + std::to_string(angle));
            WinMsg::add("Vectors Quadrant:\t" + std::to_string(quadrant));
        }

        //since atangens only delivers results for quadrants 1 and 4, we may need to adjust:
        //if (quadrant == 2) angle = 180.f + angle;
        if (quadrant == 1) angle *= -1;
        if (quadrant == 2) angle *= -1;
        if (quadrant == 3) angle = 180.f - angle;
        if (quadrant == 4) angle = 180.f - angle;

        if (angle < -180) angle += 360;
        if (angle > 180) angle -= 360;

        if (tracker) {
            WinMsg::add("Definitive angle: " + std::to_string(angle));
        }

        setRotationZ(angle);

        /*
        QVector3D direction = QVector3D{ target.x(), target.y(), 0.0f };
        _rot = QQuaternion::fromDirection(direction.normalized(), QVector3D{ 0.f, 0.f, 1.0f });
        _isMvalid = false; */
    }

    //convenience functions because we work in 2D and therefore will always rotate around the Z-Axis:
    void setRotationZ(float angle) {
        setRotation(angle, QVector3D{ 0.f, 0.f, 1.f });
        _isMvalid = false;
    }
    float rotationZ() {
        float pitch = 0.f; float yaw = 0.f; float roll = 0.f;
        _rot.getEulerAngles(&pitch, &yaw, &roll);
        return roll;
    }

    /* getters: */
    QVector3D pos() const       { return _pos; }
    QVector3D scale() const     { return _scale; }
    QQuaternion rotation() const { return _rot; }
    QVector4D rotationAngleAxis() const {
        float angle{ 0.f };
        float x = 0.f; float y = 0.f; float z = 0.f;
        _rot.getAxisAndAngle(&x, &y, &z, &angle);
        return QVector4D{ angle, x, y, z };
    }

    /*  this is the matrix we need to give our shapes before rendering them. */
    QMatrix4x4 matrix() {
        if (_isMvalid) return _M;
        _M.setToIdentity();
        _M.translate(_pos);
        _M.rotate(_rot);
        _M.scale(_scale);
        _isMvalid = true;
        return _M;
    }


private:
    QVector3D       _pos;
    QVector3D       _scale;
    QQuaternion     _rot;

	QMatrix4x4 		_M;

    bool            _isMvalid;
};

#endif // TRANSFORM_H
