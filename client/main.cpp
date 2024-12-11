//#include "CCMainWindow.h"
#include "UserLogin.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    /*CCMainWindow w;
    w.show();*/

    a.setQuitOnLastWindowClosed(false);          // 设置最后一个窗口关闭时不进行退出

    UserLogin* userLogin = new UserLogin;
    userLogin->show();
    return a.exec();
}
