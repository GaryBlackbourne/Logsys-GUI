#ifndef CONFWIDGET_H
#define CONFWIDGET_H

#include <QWidget>

extern "C"{
    #include <libusb-1.0/libusb.h>
}

namespace Ui {
class ConfWidget;
}

class ConfWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfWidget(QWidget *parent = nullptr, libusb_device_handle* = nullptr);
    ~ConfWidget();

private slots:
    void on_pushbtnQuerry_clicked();

    void on_pusbtnCFG_clicked();

private:
    Ui::ConfWidget *ui;
    libusb_device_handle *logsys_device;
};

#endif // CONFWIDGET_H
