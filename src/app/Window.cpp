//
// Created by veren on 15.10.2025.
//

#include "Window.h"
#include "base/GLWidget.h"

Window::Window(QWidget *parent) : QMainWindow(parent) {
    setWindowTitle("Super cool fractal");
    setMinimumSize(800, 600);

    auto *centralWidget = new QWidget(this);
    setCentralWidget(centralWidget);

    auto *mainLayout = new QHBoxLayout(centralWidget);
    glWidget = new GLWidget(this);
    mainLayout->addWidget(glWidget, 1);

    auto *controlGroup = new QGroupBox("Parameters");
    auto *controlLayout = new QVBoxLayout(controlGroup);

    auto *iterationsTitle = new QLabel("Iterations:");
    iterationsSlider = new QSlider(Qt::Horizontal);
    iterationsSlider->setRange(10, 1000);
    iterationsSlider->setValue(300);
    iterationsLabel = new QLabel("300");

    fpsLabel = new QLabel("FPS: 0");

    resetButton = new QPushButton("Reset View");

    auto *juliaRealTitle = new QLabel("Julia Real:");
    juliaRealSlider = new QSlider(Qt::Horizontal);
    juliaRealSlider->setRange(-100, 100);
    juliaRealSlider->setValue(-70);
    juliaRealLabel = new QLabel("-0.7");

    auto *juliaImagTitle = new QLabel("Julia Imag:");
    juliaImagSlider = new QSlider(Qt::Horizontal);
    juliaImagSlider->setRange(-100, 100);
    juliaImagSlider->setValue(27);
    juliaImagLabel = new QLabel("0.27");

    controlLayout->addWidget(iterationsTitle);
    controlLayout->addWidget(iterationsSlider);
    controlLayout->addWidget(iterationsLabel);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(fpsLabel);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(juliaRealTitle);
    controlLayout->addWidget(juliaRealSlider);
    controlLayout->addWidget(juliaRealLabel);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(juliaImagTitle);
    controlLayout->addWidget(juliaImagSlider);
    controlLayout->addWidget(juliaImagLabel);
    controlLayout->addSpacing(20);
    controlLayout->addWidget(resetButton);
    controlLayout->addStretch(1);

    mainLayout->addWidget(controlGroup);

    connect(iterationsSlider, &QSlider::valueChanged, glWidget, &GLWidget::setIterations);
    connect(iterationsSlider, &QSlider::valueChanged, this, &Window::updateIterationsLabel);

    connect(juliaRealSlider, &QSlider::valueChanged, this, [this](int value) {
        double realValue = (double)value / 100.0;
        juliaRealLabel->setText(QString::number(realValue, 'f', 2));
        glWidget->setJuliaConstant(realValue, glWidget->getJuliaConstant().y());
    });

    connect(juliaImagSlider, &QSlider::valueChanged, this, [this](int value) {
        double imagValue = (double)value / 100.0;
        juliaImagLabel->setText(QString::number(imagValue, 'f', 2));
        glWidget->setJuliaConstant(glWidget->getJuliaConstant().x(), imagValue);
    });

    connect(resetButton, &QPushButton::clicked, glWidget, &GLWidget::resetView);

    connect(glWidget, &GLWidget::fpsChanged, fpsLabel, &QLabel::setText);
}

Window::~Window() = default;

void Window::updateIterationsLabel(int value) {
    iterationsLabel->setText(QString::number(value));
}