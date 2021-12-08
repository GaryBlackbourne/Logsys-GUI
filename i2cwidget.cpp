#include "i2cwidget.h"
#include "ui_i2cwidget.h"

i2cwidget::i2cwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::i2cwidget)
{
    ui->setupUi(this);
}

i2cwidget::~i2cwidget()
{
    delete ui;
}
