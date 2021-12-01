#include "confwidget.h"
#include "ui_confwidget.h"

#include <QFile>
#include <QFileInfo>
#include <QFileDialog>

extern "C"{
    #include <libusb-1.0/libusb.h>
    #include "logsys/common.h"
    #include "logsys/jconf.h"
}

ConfWidget::ConfWidget(QWidget *parent, libusb_device_handle* logsys_device_param) :
    QWidget(parent),
    ui(new Ui::ConfWidget)
{
    ui->setupUi(this);
    ui->conf_log->setReadOnly(true);

    lastWD = "";

    this->logsys_device = logsys_device_param;
}

ConfWidget::~ConfWidget()
{
    delete ui;
}

void ConfWidget::on_pushbtnQuerry_clicked()
{
    // begin
    bool rdy;
    int res = logsys_jtag_begin(logsys_device, MODE_ECHO, &rdy);
    if(res < 0){
        QString error(libusb_error_name(res));
        ui->conf_log->insertPlainText("JTAG begin is failed!\n" + error);
    }else{
        ui->conf_log->insertPlainText("JTAG begin!\n");
    }

    // querry
    uint32_t devs[16];
    int found_devs;
    res = logsys_jtag_scan(logsys_device, devs, 16, &found_devs);
    if (res < 0) {
        QString error(libusb_error_name(res));
        ui->conf_log->insertPlainText("Scan failed!\n" + error);
    }else{
        ui->conf_log->insertPlainText(QString::number(found_devs) + " device(s) found:\n");
    }

    // end
    res = logsys_jtag_end(logsys_device);
    if(res < 0)
    {
        ui->conf_log->insertPlainText("Failed to end JTAG communication\n"); // todo: error handling
    }

    // set selector options
    QString device;
    for (int i = 0; i < found_devs; i++) {
        device = QString::number(devs[i], 16);
        ui->device_selector->addItem(device, devs[i]); // devs[i] a talált azonosító száma
        ui->conf_log->insertPlainText(" " + QString::number(i) + ": " + device + "\n");
    }
}

void ConfWidget::on_pusbtnCFG_clicked()
{

    QFileInfo *finfo;
    FILE *prog_file;
    QString filename = QFileDialog::getOpenFileName(this, "Open the file", lastWD, "Programming files (*.svf *.xsvf *.bit)");

    if(filename != nullptr){

        ui->conf_log->insertPlainText("File selected for configuration: " + filename + "\n");

        finfo = new QFileInfo(filename);
        QString extension(finfo->suffix());
        lastWD = finfo->dir().path();
//        (filename.toUtf8().constData());

        if(extension == "bit"){

            prog_file = (FILE*)logsys_conv_bit2svf(filename.toUtf8().constData());
            if(prog_file == nullptr){
                ui->conf_log->insertPlainText("Error: Bit conversion faliure. Do you have Lattice installed?\n");
            }else{
                ui->conf_log->insertPlainText("SVF conversion happened, starting download\n");
                logsys_jtag_dl_svf(logsys_device, prog_file);
                fclose(prog_file);
            }
            ui->conf_log->insertPlainText("Download complete!\n");

        }else if(extension == "svf"){

            prog_file = fopen(filename.toUtf8().constData(), "r");
            logsys_jtag_dl_svf(logsys_device, prog_file);
            fclose(prog_file);
            ui->conf_log->insertPlainText("Download complete!\n");

        }else if(extension == "xsvf"){

            prog_file = fopen(filename.toUtf8().constData(), "r");
            logsys_jtag_dl_xsvf(logsys_device, prog_file);
            fclose(prog_file);
            ui->conf_log->insertPlainText("Download complete!\n");

        }else{
            ui->conf_log->insertPlainText("Error: Only the following file extensions allowed: .bit .svf .xsvf\n");
        }
    }
}
