#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confwidget.h"
#include "usartwidget.h"
#include "bitbangwidget.h"

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
    ui->outputConsole->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    on_actionQuit_triggered();
    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
//    logsys_usb_close(logsys_device);
//    libusb_exit(NULL);
    qApp->exit(0);
}

void MainWindow::closeEvent(QCloseEvent *event){
//    on_actionQuit_triggered();
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
//    QString msg(qgetenv("XILINX"));
//    MainWindow::ui->outputConsole->insertPlainText(msg + "\n");
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

void MainWindow::on_pushbtn_CFG_clicked(bool checked)
{
    int idx = -1;
    if(checked){
        idx = MainWindow::ui->tab_container->addTab(new ConfWidget(nullptr, MainWindow::logsys_device), "Configuration");
        MainWindow::ui->tab_container->setCurrentIndex(idx);
        checked = false;
    }else{
        for(int i = 0; i < MainWindow::ui->tab_container->count(); i ++){
            if(ui->tab_container->tabText(i) == "Configuration"){
                idx = i;
            }
        }
        if(idx > -1){
            MainWindow::ui->tab_container->removeTab(idx);
        }
        checked = true;
    }
}

void MainWindow::on_pushbtn_COM_clicked(bool checked)
{
    int idx = -1;

    if(checked){

        MainWindow::ui->comm_selector->setDisabled(true);

        QString panel = MainWindow::ui->comm_selector->currentItem()->text();
        QWidget* itemToLoad;

        if(panel == "USART"){
            itemToLoad = new UsartWidget;
        }else if(panel == "Bitbang I/O"){
            itemToLoad = new BitbangWidget;
        }else{
            qApp->exit(1);
        }

        idx = MainWindow::ui->tab_container->addTab(itemToLoad, panel);
        MainWindow::ui->tab_container->setCurrentIndex(idx);
        checked = false;

    }else{

        for(int i = 0; i < MainWindow::ui->tab_container->count(); i ++){
            if(ui->tab_container->tabText(i) == "USART"){
                idx = i;
            }
            if(ui->tab_container->tabText(i) == "Bitbang I/O"){
                idx = i;
            }
        }

        if(idx > -1){
            MainWindow::ui->tab_container->removeTab(idx);
        }
        checked = true;
        MainWindow::ui->comm_selector->setDisabled(false);
    }
}

void MainWindow::on_comm_selector_itemSelectionChanged()
{
    MainWindow::ui->pushbtn_COM->setDisabled(false);
}

void MainWindow::on_pushbtn_RST_clicked(bool checked)
{
    bool success;
    if(checked){
        checked = false;
        logsys_set_reset(logsys_device, true, &success);
        if(success){
            MainWindow::ui->outputConsole->insertPlainText("RST signal ON!\n");
        }else{
            MainWindow::ui->outputConsole->insertPlainText("Could not drive RST! (already in use?)\n");
        }
    }else{
        checked = true;
        logsys_set_reset(logsys_device, false, &success);
        if(success){
            MainWindow::ui->outputConsole->insertPlainText("RST signal OFF!\n");
        }else{
            MainWindow::ui->outputConsole->insertPlainText("Could not drive RST! (already in use?)\n");
        }
    }

}

libusb_device_handle* MainWindow::getLogsysDevice() const{
    return logsys_device;
}
