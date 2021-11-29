#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confwidget.h"
#include "usartwidget.h"
#include "bitbangwidget.h"
#include "backthread.h"

#include <unistd.h>

#include <QDebug>
#include <QCloseEvent>
#include <QString>

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

    ui->freq_input->setValidator(new QIntValidator(1, 1000, this));

    ui->pushbtn_CLK->setDisabled(true);
    ui->pushbtn_CFG->setDisabled(true);
    ui->pushbtn_RST->setDisabled(true);
    ui->pushbtn_PULSE->setDisabled(true);

    ui->comm_selector->setDisabled(true);

    // initialize libusb:
    if (libusb_init(NULL) != 0) {
        qDebug() << "USB initialization failed!\n";
        qApp->exit(1); // todo: debug üzenetek.
    }else{
        qDebug() << "USB system initialized!\n";
        backLoop = new backThread;
    }
}

MainWindow::~MainWindow()
{
    backLoop->run_loop = false;
    backLoop->wait();
    // end libusb
    qDebug() << "Main window destructing \n";

    delete backLoop;

    //libusb end:
    qDebug() << "USB end\n";
    libusb_exit(NULL);

    delete ui;
}

void MainWindow::on_actionQuit_triggered()
{
    qDebug() << "Got quit signal\n";
    qApp->exit(0);
}

void MainWindow::closeEvent(QCloseEvent *event){
    qDebug() << "close event happened";
    event->accept();
}

void MainWindow::on_pushbtn_PWR_clicked(bool checked)
{
    logsys_set_vcc(backLoop->logsys_device, checked);
    if(checked){

        MainWindow::ui->pushbtn_PWR->setText("5V Power ON");
        checked = false;
        MainWindow::ui->outputConsole->insertPlainText("Vcc is set to 5V\n");

        ui->pushbtn_CFG->setDisabled(false);
        ui->pushbtn_RST->setDisabled(false);
        ui->pushbtn_PULSE->setDisabled(false);
        ui->comm_selector->setDisabled(false);

    }else{

        MainWindow::ui->pushbtn_PWR->setText("5V Power OFF");
        checked = true;
        MainWindow::ui->outputConsole->insertPlainText("Vcc is set to 0V\n");

        ui->pushbtn_CFG->setDisabled(true);
        ui->pushbtn_RST->setDisabled(true);
        ui->pushbtn_PULSE->setDisabled(true);
        ui->comm_selector->setDisabled(true);

    }
}

void MainWindow::on_pushbtn_CFG_clicked(bool checked)
{
    int idx = -1;
    if(checked){
        idx = MainWindow::ui->tab_container->addTab(new ConfWidget(nullptr, backLoop->logsys_device), "Configuration");
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
        logsys_set_reset(backLoop->logsys_device, true, &success);
        if(success){
            MainWindow::ui->outputConsole->insertPlainText("RST signal ON!\n");
        }else{
            MainWindow::ui->outputConsole->insertPlainText("Could not drive RST! (already in use?)\n");
        }
    }else{
        checked = true;
        logsys_set_reset(backLoop->logsys_device, false, &success);
        if(success){
            MainWindow::ui->outputConsole->insertPlainText("RST signal OFF!\n");
        }else{
            MainWindow::ui->outputConsole->insertPlainText("Could not drive RST! (already in use?)\n");
        }
    }
}

void MainWindow::on_pushbtn_CLK_clicked(bool checked)
{
    bool success;
    if(checked){
        ui->pushbtn_PULSE->setDisabled(true);
        int freq = ui->freq_input->text().toInt();

        if(freq < 1){
            MainWindow::ui->outputConsole->insertPlainText("Invalid CLK value: " + QString::number(freq) + " Hz!\n");
            return;
        }

        checked = false;
        logsys_clk_start(backLoop->logsys_device, freq, &success);
        if(success){
            MainWindow::ui->outputConsole->insertPlainText("CLK ON!\n");
            MainWindow::ui->outputConsole->insertPlainText("Frequency: " + QString::number(freq) + " Hz\n");
        }else{
            MainWindow::ui->outputConsole->insertPlainText("Could not drive CLK!\n");
        }
    }else{
        checked = true;
        logsys_clk_stop(backLoop->logsys_device, &success);
        if(success){
            MainWindow::ui->outputConsole->insertPlainText("CLK OFF!\n");
        }else{
            MainWindow::ui->outputConsole->insertPlainText("CLK already OFF!\n");
        }
        ui->pushbtn_PULSE->setDisabled(false);
    }
}

void MainWindow::on_pushbtn_PULSE_clicked()
{
    bool success;
    logsys_clk_start(backLoop->logsys_device, 1, &success);
    if(success){
        MainWindow::ui->outputConsole->insertPlainText("CLK pulse was given!\n");
    }else{
        MainWindow::ui->outputConsole->insertPlainText("Could not drive CLK!\n");
    }
    usleep(100);
    logsys_clk_stop(backLoop->logsys_device, &success);
    if(!success){
        MainWindow::ui->outputConsole->insertPlainText("CLK already OFF!\n");
    }
}

void MainWindow::on_freq_input_textChanged(const QString &arg1)
{
    if(arg1 != ""){
        ui->pushbtn_CLK->setDisabled(false);
    }else{
        ui->pushbtn_CLK->setDisabled(true);
    }
}
