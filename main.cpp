/*启动界面的程序入口*/

#include "mainwindow.h"
#include <QApplication>


#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
    return 0;
}
