//
//  main.cpp
//  laboratory2
//
//  Created by Илья Атмажитов on 18.10.2024.
//

#include "../include/MainWindow.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    MainWindow window;
    window.show();
    return app.exec();
}
