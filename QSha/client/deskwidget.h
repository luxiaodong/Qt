#ifndef DESKWIDGET_H
#define DESKWIDGET_H

#include <QWidget>

namespace Ui {
    class DeskWidget;
}

class DeskWidget : public QWidget
{
    Q_OBJECT

public:
    explicit DeskWidget(QWidget *parent = 0);
    ~DeskWidget();

private:
    Ui::DeskWidget *ui;
};

#endif // DESKWIDGET_H
