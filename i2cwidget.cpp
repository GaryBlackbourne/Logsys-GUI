#include "i2cwidget.h"
#include "ui_i2cwidget.h"

I2CWidget::I2CWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::I2CWidget)
{
    ui->setupUi(this);
}

I2CWidget::~I2CWidget()
{
    delete ui;
}
