#include "i2cwidget.h"
#include "ui_i2cwidget.h"

<<<<<<< HEAD
i2cwidget::i2cwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::i2cwidget)
=======
I2CWidget::I2CWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::I2CWidget)
>>>>>>> fix
{
    ui->setupUi(this);
}

<<<<<<< HEAD
i2cwidget::~i2cwidget()
=======
I2CWidget::~I2CWidget()
>>>>>>> fix
{
    delete ui;
}
