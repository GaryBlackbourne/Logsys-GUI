#ifndef BACKTHREAD_H
#define BACKTHREAD_H

extern "C"{
    #include "libusb-1.0/libusb.h"
}

#include <QThread>

class backThread : public QThread
{

    Q_OBJECT

public:
    libusb_hotplug_callback_handle cb_handle;
    static libusb_device_handle* logsys_device;
    bool run_loop;

    backThread();
    ~backThread();

    void run() override;

    static int logsys_hp_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event evt_type, void *just_null);

signals:

    void sig_logsys_removed();

};

#endif // BACKTHREAD_H
