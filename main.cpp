#include "mainwindow.h"

#include <QApplication>

//extern "C" {
//    #include <libusb-1.0/libusb.h>
//    #include "logsys/common.h"
//    #include "logsys/usb.h"
//    #include "logsys/control.h"
//    #include "logsys/status.h"
//    #include "logsys/jconf.h"
//}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
