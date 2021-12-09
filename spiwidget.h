#ifndef SPIWIDGET_H
#define SPIWIDGET_H

#include <QWidget>

namespace Ui {
<<<<<<< HEAD
class spiwidget;
}

class spiwidget : public QWidget
=======
class SPIWidget;
}

class SPIWidget : public QWidget
>>>>>>> fix
{
    Q_OBJECT

public:
<<<<<<< HEAD
    explicit spiwidget(QWidget *parent = nullptr);
    ~spiwidget();

private:
    Ui::spiwidget *ui;
=======
    explicit SPIWidget(QWidget *parent = nullptr);
    ~SPIWidget();

private:
    Ui::SPIWidget *ui;
>>>>>>> fix
};

#endif // SPIWIDGET_H
