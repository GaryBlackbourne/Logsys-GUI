#ifndef SPIWIDGET_H
#define SPIWIDGET_H

#include <QWidget>

namespace Ui {
class SPIWidget;
}

class SPIWidget : public QWidget
{
    Q_OBJECT

public:
    explicit SPIWidget(QWidget *parent = nullptr);
    ~SPIWidget();

private:
    Ui::SPIWidget *ui;
};

#endif // SPIWIDGET_H
