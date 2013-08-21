#include "qgraphicslayerscene.h"
#include <QTransform>

QGraphicsLayerScene::QGraphicsLayerScene(QObject *parent) :
    QGraphicsScene(parent)
{
}

void QGraphicsLayerScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QPointF pt = event->scenePos();
    QTransform trans;
    QGraphicsItem* item = this->itemAt(pt, trans);
    if(item && item->parentItem() == 0)
    {
        emit currentItemChanged(item);
    }
    QGraphicsScene::mousePressEvent(event);
}

void QGraphicsLayerScene::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem* item = this->mouseGrabberItem();
    emit currentItemPropertyChanged(item);
    QGraphicsScene::mouseMoveEvent(event);
}
