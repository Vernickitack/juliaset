//
// Created by veren on 15.10.2025.
//

#include <QApplication>
#include "Window.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    Window window;
    window.show();
    return QApplication::exec();
}