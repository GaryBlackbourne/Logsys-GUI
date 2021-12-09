#include "spiwidget.h"
#include "ui_spiwidget.h"

<<<<<<< HEAD
spiwidget::spiwidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::spiwidget)
=======
SPIWidget::SPIWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SPIWidget)
>>>>>>> fix
{
    ui->setupUi(this);
}

<<<<<<< HEAD
spiwidget::~spiwidget()
=======
SPIWidget::~SPIWidget()
>>>>>>> fix
{
    delete ui;
}
