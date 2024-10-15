#include <QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mainWindow;
    mainWindow.show();

    return app.exec();
}


// #include "homepage.h"
// #include <QApplication>
// // 主函数
// int main(int argc, char *argv[])
// {
//     QApplication app(argc, argv);

//     HomePage homePage;
//     homePage.show();

//     return app.exec();
// }
