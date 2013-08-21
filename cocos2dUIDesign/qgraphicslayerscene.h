#ifndef QGRAPHICSLAYERSCENE_H
#define QGRAPHICSLAYERSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>

class QGraphicsLayerScene : public QGraphicsScene
{
    Q_OBJECT
public:
    explicit QGraphicsLayerScene(QObject *parent = 0);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
signals:
    void currentItemChanged(QGraphicsItem*);
    void currentItemPropertyChanged(QGraphicsItem*);
public slots:

};

#endif // QGRAPHICSLAYERSCENE_H
