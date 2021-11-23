#include "bitbangwidget.h"
#include "ui_bitbangwidget.h"

BitbangWidget::BitbangWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::BitbangWidget)
{
    ui->setupUi(this);
}

BitbangWidget::~BitbangWidget()
{
    delete ui;
}
