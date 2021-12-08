#ifndef SPIWIDGET_H
#define SPIWIDGET_H

#include <QWidget>

namespace Ui {
class spiwidget;
}

class spiwidget : public QWidget
{
    Q_OBJECT

public:
    explicit spiwidget(QWidget *parent = nullptr);
    ~spiwidget();

private:
    Ui::spiwidget *ui;
};

#endif // SPIWIDGET_H
