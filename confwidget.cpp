#include "confwidget.h"
#include "ui_confwidget.h"

ConfWidget::ConfWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfWidget)
{
    ui->setupUi(this);
}

ConfWidget::~ConfWidget()
{
    delete ui;
}
