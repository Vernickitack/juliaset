#include "GLWidget.h"
#include <fstream>
#include <iostream>

const GLfloat vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

GLWidget::GLWidget(QWidget *parent)
        : QOpenGLWidget(parent),
          center(0.0, 0.0),
          zoom(1.0),
          isDragging(false),
          iterations(100),
          threshold(2.0),
          frameCount(0),
          fps(0.0),
          juliaConstant(-0.7, 0.27015) {
    setMouseTracking(true);
    frameTime.start();
}

GLWidget::~GLWidget() {
    makeCurrent();
    vbo.destroy();
    delete shaderProgram;
    doneCurrent();
}

void GLWidget::initializeGL() {
    initializeOpenGLFunctions();
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    shaderProgram = new QOpenGLShaderProgram(this);

    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, "shaders/vertex.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, "shaders/fragment.glsl");
    shaderProgram->link();
    shaderProgram->bind();

    vao.create();
    vao.bind();

    vbo.create();
    vbo.bind();
    vbo.allocate(vertices, sizeof(vertices));

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (void *) nullptr);
    glEnableVertexAttribArray(0);

    vao.release();

    shaderProgram->setUniformValue("iResolution", QVector2D(((float) width()), ((float) height())));
    updateViewMatrix();
    shaderProgram->setUniformValue("iIterations", iterations);
    shaderProgram->setUniformValue("iThreshold", (float) threshold);
    shaderProgram->setUniformValue("iJuliaConstant", QVector2D((float) juliaConstant.x(), (float) juliaConstant.y()));

    startTimer(20);
}

void GLWidget::setJuliaConstant(double real, double imag) {
    juliaConstant = QPointF(real, imag);
    if (shaderProgram) {
        shaderProgram->bind();
        shaderProgram->setUniformValue("iJuliaConstant", QVector2D((float) real, (float) imag));
        update();
    }
}

void GLWidget::resizeGL(int w, int h) {
    glViewport(0, 0, w, h);
    if (shaderProgram) {
        shaderProgram->bind();
        shaderProgram->setUniformValue("iResolution", QVector2D((float) w, (float) h));
    }
}

void GLWidget::paintGL() {
    glClear(GL_COLOR_BUFFER_BIT);

    shaderProgram->bind();
    vao.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    vao.release();

    frameCount++;
    updateFPS();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    QPointF mousePos = event->position();

    QPointF ndcMousePos(
            (2.0 * mousePos.x() / width()) - 1.0,
            1.0 - (2.0 * mousePos.y() / height())
    );

    ld ratio = (ld) width() / height();
    ld fx, fy;
    if (ratio > 1.0) {
        fx = (ld) ndcMousePos.x() * (ld) ratio;
        fy = (ld) ndcMousePos.y();
    } else {
        fx = (ld) ndcMousePos.x();
        fy = (ld) ndcMousePos.y() / (ld) ratio;
    }

    ld factor = 1.3;
    ld change = (event->angleDelta().y() < 0 ? factor : (1.0L / factor));
    ld oldZoom = zoom;
    zoom *= change;

    ld oldX = center.x();
    ld oldY = center.y();
    center.setX((oldX + fx) / change - fx);
    center.setY((oldY + fy) / change - fy);

    updateViewMatrix();
    update();
    event->accept();
}


void GLWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = true;
        lastMousePos = event->pos();
        setCursor(Qt::ClosedHandCursor);
    }
    event->accept();
}

void GLWidget::mouseMoveEvent(QMouseEvent *event) {
    if (isDragging) {
        QPoint delta = event->pos() - lastMousePos;
        lastMousePos = event->pos();

        ld factor = 3 * zoom;

        center.setX(center.x() - ((ld) delta.x()) * factor / zoom / ((ld) width()));
        center.setY(center.y() + ((ld) delta.y()) * factor / zoom / ((ld) height()));

        updateViewMatrix();
        update();
    }
    event->accept();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        isDragging = false;
        setCursor(Qt::ArrowCursor);
    }
    event->accept();
}

void GLWidget::setIterations(int value) {
    this->iterations = value;
    if (shaderProgram) {
        shaderProgram->bind();
        shaderProgram->setUniformValue("iIterations", value);
        update();
    }
}

void GLWidget::updateViewMatrix() {
    viewMatrix.setToIdentity();
    viewMatrix.scale((float) zoom, (float) zoom);
    viewMatrix.translate((float) center.x(), (float) center.y());

    if (shaderProgram) {
        shaderProgram->bind();
        shaderProgram->setUniformValue("iViewMatrix", viewMatrix);
    }
}

void GLWidget::updateFPS() {
    if (frameTime.elapsed() >= 1000) {
        fps = ((ld) frameCount) * 1000.0 / frameTime.elapsed();
        emit fpsChanged(QString("FPS: %1").arg((double) fps, 0, 'f', 1));
        frameCount = 0;
        frameTime.restart();
    }
}

void GLWidget::resetView() {
    center = QPointF(0.0, 0.0);
    zoom = 1.0;
    updateViewMatrix();
    update();
}

QVector2D GLWidget::getJuliaConstant() {
    return {(float) juliaConstant.x(), (float) juliaConstant.y()};
}
