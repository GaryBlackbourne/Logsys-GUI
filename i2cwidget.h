#ifndef I2CWIDGET_H
#define I2CWIDGET_H

#include <QWidget>

namespace Ui {
class I2CWidget;
}

class I2CWidget : public QWidget
{
    Q_OBJECT

public:
    explicit I2CWidget(QWidget *parent = nullptr);
    ~I2CWidget();

private:
    Ui::I2CWidget *ui;
};

#endif // I2CWIDGET_H
