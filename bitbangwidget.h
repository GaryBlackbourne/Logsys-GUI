#ifndef BITBANGWIDGET_H
#define BITBANGWIDGET_H

#include <QWidget>

namespace Ui {
class BitbangWidget;
}

class BitbangWidget : public QWidget
{
    Q_OBJECT

public:
    explicit BitbangWidget(QWidget *parent = nullptr);
    ~BitbangWidget();

private:
    Ui::BitbangWidget *ui;
};

#endif // BITBANGWIDGET_H
