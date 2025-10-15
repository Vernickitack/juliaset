//
// Created by veren on 15.10.2025.
//

#ifndef JULIASET_WINDOW_H
#define JULIASET_WINDOW_H

#include <QMainWindow>
#include <QSlider>
#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGroupBox>
#include <QPushButton>

class GLWidget;

class Window : public QMainWindow {
Q_OBJECT

public:
    explicit Window(QWidget *parent = nullptr);

    ~Window() override;

private slots:

    void updateIterationsLabel(int value);

private:
    GLWidget *glWidget;
    QSlider *iterationsSlider;
    QLabel *iterationsLabel;
    QLabel *fpsLabel;

    QSlider *juliaRealSlider;
    QSlider *juliaImagSlider;
    QLabel *juliaRealLabel;
    QLabel *juliaImagLabel;

    QPushButton *resetButton;
};


#endif //JULIASET_WINDOW_H
