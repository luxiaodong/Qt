#include "qcocos2ddata.h"
#include <QDebug>

QCocos2dData::QCocos2dData()
{
}

void QCocos2dCCNode::print()
{
    qDebug()<<"CCNode";
}

void QCocos2dCCSpriteBatchNode::print()
{
    qDebug()<<"CCSpriteBatchNode";
}

void QCocos2dCCLayer::print()
{
    qDebug()<<"CCLayer";
}

void QCocos2dCCLayerGradient::print()
{
    qDebug()<<"CCLayerGradient";
}

void QCocos2dCCMenuItemImage::print()
{
    qDebug()<<"CCMenuItemImage";
}

void QCocos2dCCMenu::print()
{
    qDebug()<<"CCMenu";
}

void QCocos2dCCSprite::print()
{
    qDebug()<<"CCSprite";
}

void QCocos2dCCLabelTTF::print()
{
    qDebug()<<"CCLabelTTF";
}

void QCocos2dCCLabelBMFont::print()
{
    qDebug()<<"CCLabelBMFont";
}
