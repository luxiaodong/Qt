#ifndef QCOCOS2DDATA_H
#define QCOCOS2DDATA_H

#include <QString>
#include <QStringList>
#include <QList>

class QCocos2dPropertiesPoint
{
public:
    float m_x;
    float m_y;
    float m_unknow;
};

class QCocos2dPropertiesContentSize
{
public:
    float m_width;
    float m_height;
    int m_unknow;
};

class QCocos2dPropertiesAnchorPoint
{
public:
    float m_xPercent;
    float m_yPercent;
};

class QCocos2dPropertiesScale
{
public:
    float m_xScale;
    float m_yScale;
    bool  m_isLock;
    int m_unknow;
};

class QCocos2dPropertiesRotation
{
public:
    float m_degree;
};

class QCocos2dPropertiesIgnoreAnchorPointForPosition
{
public:
    bool m_isIgnore;
};

class QCocos2dPropertiesIsTouchEnabled
{
public:
    bool m_isEnable;
};

class QCocos2dPropertiesIsAccelerometerEnabled
{
public:
    bool m_isEnable;
};

class QCocos2dPropertiesIsMouseEnabled
{
public:
    bool m_isEnable;
};

class QCocos2dPropertiesIsKeyboardEnabled
{
public:
    bool m_isEnable;
};

class QCocos2dPropertiesColor3Int
{
public:
    int m_red;
    int m_green;
    int m_blue;
};

class QCocos2dPropertiesVector
{
public:
    int m_x;
    int m_y;
};

class QCocos2dPropertiesSpriteFrame
{
public:
    QString m_framePath;
};

class QCocos2dPropertiesBlock
{
public:
    QString m_unknow1;
    int m_unknow2;
};

class QCocos2dPropertiesMemberVar
{
public:
    QString m_name;
    int m_type;
};

class QCocos2dPropertiesFontName
{
public:
    QString m_fontName;
};

class QCocos2dPropertiesFontSize
{
public:
    float m_fontSize;
    int m_unknow;
};

class QCocos2dPropertiesLabelDimensions
{
public:
    float m_width;
    float m_height;
    int m_unknow;
};

class QCocos2dPropertiesLabelAlignment
{
public:
    int m_type;
};

class QCocos2dPropertiesLabelText
{
public:
    QString m_text;
};

class QCocos2dData
{
public:
    QCocos2dData();
};

class QCocos2dCCNode
{
public:
    virtual void print();
public:
    QList<QCocos2dCCNode*> m_chlidren;
    QCocos2dCCNode* m_parent;

public:
    QCocos2dPropertiesPoint m_point;
    QCocos2dPropertiesAnchorPoint m_anchorPoint;
    QCocos2dPropertiesContentSize m_size;
    QCocos2dPropertiesScale m_scale;
    QCocos2dPropertiesRotation m_rotation;
    QCocos2dPropertiesIgnoreAnchorPointForPosition m_ignoreAnchorPoint;
};

class QCocos2dCCSpriteBatchNode : public QCocos2dCCNode
{
public:
    virtual void print();
};

class QCocos2dCCLayer : public QCocos2dCCNode
{
public:
    virtual void print();
public:
    QCocos2dPropertiesIsTouchEnabled m_isTouch;
    QCocos2dPropertiesIsAccelerometerEnabled m_isAccelerometer;
    QCocos2dPropertiesIsMouseEnabled m_isMouse;
    QCocos2dPropertiesIsKeyboardEnabled m_isKeyboard;
};

class QCocos2dCCLayerGradient : public QCocos2dCCLayer
{
public:
    virtual void print();
public:
    QCocos2dPropertiesColor3Int m_startColor;
    QCocos2dPropertiesColor3Int m_endColor;
};

class QCocos2dCCMenuItemImage : public QCocos2dCCNode
{
public:
    virtual void print();
public:
    QCocos2dPropertiesSpriteFrame m_normal;
    QCocos2dPropertiesSpriteFrame m_select;
    QCocos2dPropertiesSpriteFrame m_disable;
};

class QCocos2dCCMenu : public QCocos2dCCLayer
{
public:
    virtual void print();
};

class QCocos2dCCSprite : public QCocos2dCCNode
{
public:
    virtual void print();
public:
    QCocos2dPropertiesSpriteFrame m_display;
};

class QCocos2dCCLabelTTF : public QCocos2dCCSprite
{
public:
    virtual void print();
public:
    QCocos2dPropertiesFontName m_fontName;
    QCocos2dPropertiesFontSize m_fontSize;
    QCocos2dPropertiesLabelAlignment m_horzAlignment;
    QCocos2dPropertiesLabelAlignment m_vertAlignment;
    QCocos2dPropertiesLabelDimensions m_dimensions;
    QCocos2dPropertiesLabelText m_text;
};

class QCocos2dCCLabelBMFont : public QCocos2dCCSpriteBatchNode
{
public:
    virtual void print();
public:
    QCocos2dPropertiesLabelAlignment m_horzAlignment;
    int m_maxWidth;
    QCocos2dPropertiesLabelText m_text;
};

#endif // QCOCOS2DDATA_H

