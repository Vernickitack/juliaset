//
// Created by veren on 25.10.2025.
//

#ifndef JULIASET_ENGINE_H
#define JULIASET_ENGINE_H

#include <QPointF>
#include <QMatrix4x4>
#include <QVector2D>
#include <QtMath>

#define ld long double

class Engine {
public:
    Engine();

    int getIterations() const { return iterations; }

    ld getThreshold() const { return threshold; }

    QPointF getJuliaConstant() const { return juliaConstant; }

    QMatrix4x4 getViewMatrix() const { return viewMatrix; }

    void setIterations(int value);

    void setJuliaConstant(double real, double imag);

    void resetView();

    void move(QPoint delta, int width, int height);

    void zoomFunc(const QPointF &mousePos, int width, int height, int angleDelta);

private:
    QMatrix4x4 viewMatrix;
    QPointF center;
    ld zoom;

    int iterations;
    ld threshold;
    QPointF juliaConstant;

    void updateViewMatrix();
};

#endif //JULIASET_ENGINE_H