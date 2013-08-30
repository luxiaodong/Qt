#ifndef QRUNEMASTERWIDGET_H
#define QRUNEMASTERWIDGET_H

#include <QWidget>

#include "qrunemasterdata.h"

class QRuneMasterWidget : public QWidget
{
    Q_OBJECT
public:
    explicit QRuneMasterWidget(QWidget *parent = 0);

signals:

public slots:

public:
    QRuneMasterData m_runeMasterData;
};

#endif // QRUNEMASTERWIDGET_H
