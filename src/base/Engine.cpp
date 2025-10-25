//
// Created by veren on 25.10.2025.
//

#include "Engine.h"

Engine::Engine()
        : center(0.0, 0.0),
          zoom(1.0),
          iterations(100),
          threshold(2.0),
          juliaConstant(-0.7, 0.27015) {
    updateViewMatrix();
}

void Engine::setIterations(int value) {
    this->iterations = value;
}

void Engine::setJuliaConstant(double real, double imag) {
    juliaConstant = QPointF(real, imag);
}

void Engine::resetView() {
    center = QPointF(0.0, 0.0);
    zoom = 1.0;
    updateViewMatrix();
}

void Engine::updateViewMatrix() {
    viewMatrix.setToIdentity();
    viewMatrix.scale((float) zoom, (float) zoom);
    viewMatrix.translate((float) center.x(), (float) center.y());
}

void Engine::move(QPoint delta, int width, int height) {
    ld factor = 3 * zoom;

    center.setX(center.x() - ((ld) delta.x()) * factor / zoom / ((ld) width));
    center.setY(center.y() + ((ld) delta.y()) * factor / zoom / ((ld) height));

    updateViewMatrix();
}

void Engine::zoomFunc(const QPointF &mousePos, int width, int height, int angleDelta) {
    QPointF ndcMousePos(
            (2.0 * mousePos.x() / width) - 1.0,
            1.0 - (2.0 * mousePos.y() / height)
    );

    ld ratio = (ld) width / height;
    ld fx, fy;
    if (ratio > 1.0) {
        fx = (ld) ndcMousePos.x() * (ld) ratio;
        fy = (ld) ndcMousePos.y();
    } else {
        fx = (ld) ndcMousePos.x();
        fy = (ld) ndcMousePos.y() / (ld) ratio;
    }

    ld factor = 1.3;
    ld change = (angleDelta < 0 ? factor : (1.0L / factor));
    zoom *= change;

    ld oldX = center.x();
    ld oldY = center.y();
    center.setX((oldX + fx) / change - fx);
    center.setY((oldY + fy) / change - fy);

    updateViewMatrix();
}