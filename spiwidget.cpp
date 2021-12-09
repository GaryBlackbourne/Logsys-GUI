#include "spiwidget.h"
#include "ui_spiwidget.h"

SPIWidget::SPIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPIWidget)
{
    ui->setupUi(this);
}

SPIWidget::~SPIWidget()
{
    delete ui;
}
