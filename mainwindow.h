#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
extern "C"{
#include <libusb-1.0/libusb.h>
}

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

protected:
     void closeEvent(QCloseEvent *event);

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_pushbtn_INIT_clicked();

    void on_actionQuit_triggered();

    void on_pushbtn_PWR_clicked(bool checked);

    void on_pushbtn_CFG_clicked(bool checked);

    void on_pushbtn_COM_clicked(bool checked);

    void on_comm_selector_itemSelectionChanged();

    void on_pushbtn_RST_clicked(bool checked);

    libusb_device_handle* getLogsysDevice() const;

private:
    Ui::MainWindow *ui;
    libusb_device_handle *logsys_device;
};
#endif // MAINWINDOW_H
