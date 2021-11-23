#ifndef CONFWIDGET_H
#define CONFWIDGET_H

#include <QWidget>

namespace Ui {
class ConfWidget;
}

class ConfWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ConfWidget(QWidget *parent = nullptr);
    ~ConfWidget();

private:
    Ui::ConfWidget *ui;
};

#endif // CONFWIDGET_H
