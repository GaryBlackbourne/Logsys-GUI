#include "usartwidget.h"
#include "ui_usartwidget.h"

UsartWidget::UsartWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsartWidget)
{
    ui->setupUi(this);
}

UsartWidget::~UsartWidget()
{
    delete ui;
}
