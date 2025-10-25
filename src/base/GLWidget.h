//
// Created by veren on 15.10.2025.
//

#ifndef JULIASET_GLWIDGET_H
#define JULIASET_GLWIDGET_H

#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLShaderProgram>
#include <QOpenGLBuffer>
#include <QMatrix4x4>
#include <QVector2D>
#include <QElapsedTimer>
#include <QWheelEvent>
#include <QMouseEvent>
#include <QSurfaceFormat>
#include <QtMath>
#include "Engine.h"

#define ld long double

class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions {
Q_OBJECT
public:
    explicit GLWidget(QWidget *parent = nullptr);

    ~GLWidget() override;

    void setIterations(int value);

    QVector2D getJuliaConstant();

    void setJuliaConstant(double real, double imag);

signals:

    void fpsChanged(const QString &qString);

protected:
    void initializeGL() override;

    void resizeGL(int w, int h) override;

    void paintGL() override;

    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

public slots:

    void resetView();

private:
    Engine engine;

    QOpenGLShaderProgram *shaderProgram{};
    QOpenGLBuffer vbo;
    QOpenGLVertexArrayObject vao;

    QPoint lastMousePos;
    bool isDragging;

    QElapsedTimer frameTime;
    int frameCount;
    ld fps;

    void updateFPS();
};

#endif //JULIASET_GLWIDGET_H
