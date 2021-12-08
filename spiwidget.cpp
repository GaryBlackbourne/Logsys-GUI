#include "spiwidget.h"
#include "ui_spiwidget.h"

spiwidget::spiwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spiwidget)
{
    ui->setupUi(this);
}

spiwidget::~spiwidget()
{
    delete ui;
}
