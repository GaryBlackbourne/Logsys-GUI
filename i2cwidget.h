#ifndef I2CWIDGET_H
#define I2CWIDGET_H

#include <QWidget>

namespace Ui {
<<<<<<< HEAD
class i2cwidget;
}

class i2cwidget : public QWidget
=======
class I2CWidget;
}

class I2CWidget : public QWidget
>>>>>>> fix
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit i2cwidget(QWidget *parent = nullptr);
    ~i2cwidget();

private:
    Ui::i2cwidget *ui;
=======
    explicit I2CWidget(QWidget *parent = nullptr);
    ~I2CWidget();

private:
    Ui::I2CWidget *ui;
>>>>>>> fix
};

#endif // I2CWIDGET_H
