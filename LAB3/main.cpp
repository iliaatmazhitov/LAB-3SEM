#include <iostream>
#include <QApplication>
#include "MainWindow.h"
#include "backpack.h"
#include "MemoryManager.h"
#include "sequences/ArraySequence.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
