#ifndef USARTWIDGET_H
#define USARTWIDGET_H

#include <QWidget>

namespace Ui {
class UsartWidget;
}

class UsartWidget : public QWidget
{
    Q_OBJECT

public:
    explicit UsartWidget(QWidget *parent = nullptr);
    ~UsartWidget();

private:
    Ui::UsartWidget *ui;
};

#endif // USARTWIDGET_H
