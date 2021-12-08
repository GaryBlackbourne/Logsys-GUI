#ifndef I2CWIDGET_H
#define I2CWIDGET_H

#include <QWidget>

namespace Ui {
class i2cwidget;
}

class i2cwidget : public QWidget
{
    Q_OBJECT

public:
    explicit i2cwidget(QWidget *parent = nullptr);
    ~i2cwidget();

private:
    Ui::i2cwidget *ui;
};

#endif // I2CWIDGET_H
