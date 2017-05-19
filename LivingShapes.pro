#-------------------------------------------------
#
# Project created by QtCreator 2017-04-10T15:14:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LivingShapes
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    Shapes/shape.cpp \
    Shapes/concreteshapes.cpp \
    camera.cpp \
    Entities/gameentity.cpp \
    Entities/movingentity.cpp \
    Entities/aabb.cpp \
    Shapes/shapemaker.cpp \
    Entities/light.cpp \
    Grafics/GaussBlur.cpp \
    Scene.cpp

HEADERS  += mainwindow.h \
    glwidget.h \
    Shapes/shape.h \
    Shapes/concreteshapes.h \
    camera.h \
    Entities/gameentity.h \
    Entities/transform.h \
    Entities/movingentity.h \
    Entities/aabb.h \
    Shapes/shapemaker.h \
    Entities/light.h \
    Grafics/GaussBlur.hpp \
    Scene.h

FORMS    += mainwindow.ui

DISTFILES += \
    Shaders/simple.vert.glsl \
    Shaders/simple.frag.glsl \
    Shaders/lights.vert.glsl \
    Shaders/lights.frag.glsl \
    Shaders/scene.frag.glsl \
    Shaders/scene.vert.glsl \
    Shaders/final.vert.glsl \
    Shaders/final.frag.glsl \
    Shaders/gaussianBlur.frag.glsl \
    Shaders/gaussianBlur.vert.glsl
