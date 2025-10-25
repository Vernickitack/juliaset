#include "GLWidget.h"
#include <fstream>

const GLfloat vertices[] = {
        -1.0f, -1.0f,
        1.0f, -1.0f,
        -1.0f, 1.0f,
        1.0f, 1.0f
};

GLWidget::GLWidget(QWidget *parent)
        : QOpenGLWidget(parent),
          isDragging(false),
          frameCount(0),
          fps(0.0) {
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
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/vertex.glsl");
    shaderProgram->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/fragment.glsl");
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
    shaderProgram->setUniformValue("iViewMatrix", engine.getViewMatrix());
    shaderProgram->setUniformValue("iIterations", engine.getIterations());
    shaderProgram->setUniformValue("iThreshold", (float) engine.getThreshold());
    QPointF c = engine.getJuliaConstant();
    shaderProgram->setUniformValue("iJuliaConstant", QVector2D((float) c.x(), (float) c.y()));

    startTimer(20);
}

void GLWidget::setJuliaConstant(double real, double imag) {
    engine.setJuliaConstant(real, imag);
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
    shaderProgram->setUniformValue("iViewMatrix", engine.getViewMatrix()); // TODO:?
    vao.bind();
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    vao.release();

    frameCount++;
    updateFPS();
}

void GLWidget::wheelEvent(QWheelEvent *event) {
    engine.zoomFunc(event->position(), width(), height(), event->angleDelta().y());
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
        engine.move(delta, width(), height());
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
    engine.setIterations(value);
    if (shaderProgram) {
        shaderProgram->bind();
        shaderProgram->setUniformValue("iIterations", value);
        update();
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
    engine.resetView();
    update();
}

QVector2D GLWidget::getJuliaConstant() {
    QPointF c = engine.getJuliaConstant();
    return {(float) c.x(), (float) c.y()};
}