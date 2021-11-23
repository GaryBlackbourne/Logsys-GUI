#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCloseEvent>

extern "C" {
    #include <libusb-1.0/libusb.h>
    #include "logsys/common.h"
    #include "logsys/usb.h"
    #include "logsys/control.h"
    #include "logsys/status.h"
    #include "logsys/jconf.h"
}

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    on_actionQuit_triggered();
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    logsys_usb_close(logsys_device);
    libusb_exit(NULL);
    qApp->exit(0);
}

void MainWindow::closeEvent(QCloseEvent *event){
    on_actionQuit_triggered();
    event->accept();
}

void MainWindow::on_pushbtn_PWR_clicked(bool checked)
{
    logsys_set_vcc(logsys_device, checked);
    if(checked){
        MainWindow::ui->pushbtn_PWR->setText("5V Power ON");
        checked = false;
        MainWindow::ui->outputConsole->insertPlainText("Vcc is set to 5V\n");
    }else{
        MainWindow::ui->pushbtn_PWR->setText("5V Power OFF");
        checked = true;
        MainWindow::ui->outputConsole->insertPlainText("Vcc is set to 0V\n");
    }
}

void MainWindow::on_pushbtn_INIT_clicked()
{
    // initialize libusb:
    if (libusb_init(NULL) != 0) {
        MainWindow::ui->outputConsole->insertPlainText("USB initialization failed!\n");
        //qApp->exit(1);
    }else{
        MainWindow::ui->outputConsole->insertPlainText("USB system initialized!\n");
    }

    logsys_device = logsys_usb_open(NULL, NULL);
    if (logsys_device == NULL) {
        MainWindow::ui->outputConsole->insertPlainText("No Logsys device found!\n");
        //qApp->exit(2);
    }else{
        MainWindow::ui->outputConsole->insertPlainText("Logsys device found!\n");
    }
}
