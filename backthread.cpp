#include "backthread.h"

#include <sys/time.h>
#include <QDebug>

extern "C"{
    #include "libusb-1.0/libusb.h"
    #include "logsys/usb.h"
}

libusb_device_handle* backThread::logsys_device = nullptr;

int backThread::logsys_hp_callback(libusb_context *ctx, libusb_device *dev, libusb_hotplug_event evt_type, void *just_null){
    if(evt_type==LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED){
        qDebug() << "Logsys connected\n";
        logsys_device = logsys_usb_open(nullptr, nullptr);
        if (logsys_device == nullptr) {
            qDebug() << "USB open error\n";
        } else {
            qDebug() << "Logsys device found!\n";
        }
    }else{ //LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT
        qDebug() << "Logsys disconnected\n";
    }
    return 0;
}

backThread::backThread(){

    logsys_device = logsys_usb_open(nullptr, nullptr);
    if (logsys_device == nullptr) {
        qDebug() << "USB open error\n";
    } else {
        qDebug() << "Logsys device found!\n";
    }

    if(!logsys_hotplug_enable(NULL, (libusb_hotplug_event)(LIBUSB_HOTPLUG_EVENT_DEVICE_ARRIVED|LIBUSB_HOTPLUG_EVENT_DEVICE_LEFT), logsys_hp_callback, NULL, &cb_handle)){
        qDebug() << "Hotplug listen error\n";
    }

    run_loop = true;
    this->start();
}

backThread::~backThread(){
    qDebug() << "backLoop destructing\n";
    libusb_hotplug_deregister_callback(NULL, cb_handle);
    if(logsys_device != nullptr){
        logsys_usb_close(logsys_device);
    }
}

void backThread::run(){
    qDebug() << "backloop cycle start\n";
    struct timeval tv;
    tv.tv_sec = 1;
    tv.tv_usec = 0;
    while(run_loop){
        libusb_handle_events_timeout_completed(NULL, &tv,  NULL); // timeoutosra cserélve, hogy ki tudjon lépni a thread
        qDebug() << "bl cycle passed!\n";
    }
    qDebug() << "backloop cycle end\n";
}
