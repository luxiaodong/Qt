#include "qparseccbfile.h"

#include <QTextStream>
#include <QFile>
#include <QDebug>

QParseCCBFile::QParseCCBFile()
{
}

bool QParseCCBFile::parseCCBFile(QString filePath)
{
    if( this->readCCBFile(filePath) == true)
    {
        return this->parseCCBFile();
    }

    return false;
}

bool QParseCCBFile::readCCBFile(QString filePath)
{
    QFile file(filePath);
    if( file.open(QIODevice::ReadOnly) == false)
    {
        return false;
    }

    QTextStream stream(&file);
    while(stream.atEnd() == false)
    {
        QString line = stream.readLine().trimmed();
        m_lines.append(line);
    }

    file.close();
    return true;
}

bool QParseCCBFile::parseCCBFile()
{
    int nodeGraphIndex = this->indexOf("<key>nodeGraph</key>");
    if(nodeGraphIndex == -1)
    {
        return false;
    }

    int dictEndIndex = this->indexOfPair("dict", nodeGraphIndex + 1);
    //qDebug()<<nodeGraphIndex<<dictEndIndex;
    m_root = this->parseCocos2dClass(nodeGraphIndex + 1, dictEndIndex);
    this->print(m_root);
    return true;
}

QCocos2dCCNode* QParseCCBFile::parseCocos2dClass(int startLineIndex, int endLineIndex)
{
    int index = this->indexOf("<key>baseClass</key>", startLineIndex);
    QString baseClass = m_lines.at(index + 1);
    QCocos2dCCNode* node = this->createCocos2dClass(this->dataOfLine(baseClass));
    qDebug()<<index<<baseClass;
    Q_ASSERT(node != 0);

    index = this->indexOf("<key>children</key>", index);
    QString children = m_lines.at(index + 1);
    if(children == "<array>")
    {
        node->m_chlidren.clear();
        int arrayBeginIndex = index + 1;
        int arrayEndIndex = this->indexOfPair("array", arrayBeginIndex);
        int dictBeginIndex = arrayBeginIndex + 1;

        while(1)
        {
            int dictEndIndex = this->indexOfPair("dict", dictBeginIndex);
            QCocos2dCCNode* subNode = this->parseCocos2dClass(dictBeginIndex,dictEndIndex);
            node->m_chlidren.append(subNode);
            subNode->m_parent = node;
            dictBeginIndex = dictEndIndex + 1;
            Q_ASSERT(dictBeginIndex <= arrayEndIndex);
            if(dictBeginIndex == arrayEndIndex)
            {
                break;
            }
        }

        index = arrayEndIndex;
    }
    else if(children == "</array>")
    {
        node->m_chlidren.clear();
    }

    index = this->indexOf("<key>properties</key>", index);
    QString properties = m_lines.at(index + 1);
    if(properties == "<array>")
    {
        int arrayBeginIndex = index + 1;
        int arrayEndIndex = this->indexOfPair("array", arrayBeginIndex);
        int dictBeginIndex = arrayBeginIndex + 1;

        while(1)
        {
            int dictEndIndex = this->indexOfPair("dict", dictBeginIndex);
            this->parseCocos2dProperties(node, dictBeginIndex, dictEndIndex);
            dictBeginIndex = dictEndIndex + 1;
            //qDebug()<<dictBeginIndex<<dictEndIndex;
            Q_ASSERT(dictBeginIndex <= arrayEndIndex);
            if(dictBeginIndex == arrayEndIndex)
            {
                break;
            }
        }

        index = arrayEndIndex;
    }
    else if(properties == "</array>")
    {}

    Q_ASSERT(index < endLineIndex);
    return node;
}

void QParseCCBFile::parseCocos2dProperties(QCocos2dCCNode* node, int startLineIndex, int endLineIndex)
{
    int index = this->indexOf("<key>name</key>", startLineIndex);
    QString name = this->dataOfLine( m_lines.at(index + 1) );

    if(name == "position")
    {
        QCocos2dPropertiesPoint pt;
        pt.m_x = this->dataOfLine( m_lines.at(index + 6) ).toFloat();
        pt.m_y = this->dataOfLine( m_lines.at(index + 7) ).toFloat();
        pt.m_unknow = this->dataOfLine( m_lines.at(index + 8) ).toInt();
        node->m_point = pt;
    }
    else if(name == "contentSize")
    {
        QCocos2dPropertiesContentSize size;
        size.m_width = this->dataOfLine( m_lines.at(index + 6) ).toFloat();
        size.m_height = this->dataOfLine( m_lines.at(index + 7) ).toFloat();
        size.m_unknow = this->dataOfLine( m_lines.at(index + 8) ).toInt();
        node->m_size = size;
    }
    else if(name == "anchorPoint")
    {
        QCocos2dPropertiesAnchorPoint anchorPoint;
        anchorPoint.m_xPercent = this->dataOfLine( m_lines.at(index + 6) ).toFloat();
        anchorPoint.m_yPercent = this->dataOfLine( m_lines.at(index + 7) ).toFloat();
        node->m_anchorPoint = anchorPoint;
    }
    else if(name == "scale")
    {
        QCocos2dPropertiesScale scale;
        scale.m_xScale = this->dataOfLine( m_lines.at(index + 6) ).toFloat();
        scale.m_yScale = this->dataOfLine( m_lines.at(index + 7) ).toFloat();
        QString line = m_lines.at(index + 8);
        if(line.contains("true") == true)
        {
            scale.m_isLock = true;
        }
        else //if(line.contains("false") == true)
        {
            scale.m_isLock = false;
        }
        scale.m_unknow = this->dataOfLine( m_lines.at(index + 9) ).toInt();
        node->m_scale = scale;
    }
    else if(name == "ignoreAnchorPointForPosition")
    {
        QCocos2dPropertiesIgnoreAnchorPointForPosition ignore;
        QString line = m_lines.at(index + 5);
        if(line.contains("true") == true)
        {
            ignore.m_isIgnore = true;
        }
        else //if(line.contains("false") == true)
        {
            ignore.m_isIgnore = false;
        }
        node->m_ignoreAnchorPoint = ignore;
    }
    else if(name == "isTouchEnabled")
    {
        QCocos2dPropertiesIsTouchEnabled touch;
        QString line = m_lines.at(index + 7);
        if(line.contains("true") == true)
        {
            touch.m_isEnable = true;
        }
        else //if(line.contains("false") == true)
        {
            touch.m_isEnable = false;
        }
        QCocos2dCCLayer* temp = (QCocos2dCCLayer*)(node);
        temp->m_isTouch = touch;
    }
    else if(name == "isAccelerometerEnabled")
    {
        QCocos2dPropertiesIsAccelerometerEnabled accelerometer;
        QString line = m_lines.at(index + 7);
        if(line.contains("true") == true)
        {
            accelerometer.m_isEnable = true;
        }
        else //if(line.contains("false") == true)
        {
            accelerometer.m_isEnable = false;
        }
        QCocos2dCCLayer* temp = (QCocos2dCCLayer*)(node);
        temp->m_isAccelerometer = accelerometer;
    }
    else if(name == "isMouseEnabled")
    {
        QCocos2dPropertiesIsMouseEnabled mouse;
        QString line = m_lines.at(index + 7);
        if(line.contains("true") == true)
        {
            mouse.m_isEnable = true;
        }
        else //if(line.contains("false") == true)
        {
            mouse.m_isEnable = false;
        }
        QCocos2dCCLayer* temp = (QCocos2dCCLayer*)(node);
        temp->m_isMouse = mouse;
    }
    else if(name == "isKeyboardEnabled")
    {
        QCocos2dPropertiesIsKeyboardEnabled keyboard;
        QString line = m_lines.at(index + 7);
        if(line.contains("true") == true)
        {
            keyboard.m_isEnable = true;
        }
        else //if(line.contains("false") == true)
        {
            keyboard.m_isEnable = false;
        }
        QCocos2dCCLayer* temp = (QCocos2dCCLayer*)(node);
        temp->m_isKeyboard = keyboard;
    }
    else if(name == "startColor")
    {
        QCocos2dPropertiesColor3Int color;
        color.m_red = this->dataOfLine( m_lines.at(index + 6) ).toInt();
        color.m_green = this->dataOfLine( m_lines.at(index + 7) ).toInt();
        color.m_blue = this->dataOfLine( m_lines.at(index + 8) ).toInt();
        QCocos2dCCLayerGradient* temp = (QCocos2dCCLayerGradient*)(node);
        temp->m_startColor = color;
    }
    else if(name == "endColor")
    {
        QCocos2dPropertiesColor3Int color;
        color.m_red = this->dataOfLine( m_lines.at(index + 6) ).toInt();
        color.m_green = this->dataOfLine( m_lines.at(index + 7) ).toInt();
        color.m_blue = this->dataOfLine( m_lines.at(index + 8) ).toInt();
        QCocos2dCCLayerGradient* temp = (QCocos2dCCLayerGradient*)(node);
        temp->m_endColor = color;
    }
    else if(name == "normalSpriteFrame")
    {
        QCocos2dPropertiesSpriteFrame frame;
        frame.m_framePath = this->dataOfLine( m_lines.at(index + 7) );
        QCocos2dCCMenuItemImage* temp = (QCocos2dCCMenuItemImage*)(node);
        temp->m_normal = frame;
    }
    else if(name == "selectedSpriteFrame")
    {
        QCocos2dPropertiesSpriteFrame frame;
        frame.m_framePath = this->dataOfLine( m_lines.at(index + 7) );
        QCocos2dCCMenuItemImage* temp = (QCocos2dCCMenuItemImage*)(node);
        temp->m_select = frame;
    }
    else if(name == "disabledSpriteFrame")
    {
        QCocos2dPropertiesSpriteFrame frame;
        frame.m_framePath = this->dataOfLine( m_lines.at(index + 7) );
        QCocos2dCCMenuItemImage* temp = (QCocos2dCCMenuItemImage*)(node);
        temp->m_disable = frame;
    }
    else if(name == "displayFrame")
    {
        QCocos2dPropertiesSpriteFrame frame;
        frame.m_framePath = this->dataOfLine( m_lines.at(index + 7) );
        QCocos2dCCSprite* temp = (QCocos2dCCSprite*)(node);
        temp->m_display = frame;
    }
    else if(name == "fontName")
    {
        QCocos2dPropertiesFontName fontName;
        fontName.m_fontName = this->dataOfLine( m_lines.at(index + 5) );
        QCocos2dCCLabelTTF* temp = (QCocos2dCCLabelTTF*)(node);
        temp->m_fontName = fontName;
    }
    else if(name == "fontSize")
    {
        QCocos2dPropertiesFontSize fontSize;
        fontSize.m_fontSize = this->dataOfLine( m_lines.at(index + 6) ).toFloat();
        fontSize.m_unknow = this->dataOfLine( m_lines.at(index + 7) ).toInt();
        QCocos2dCCLabelTTF* temp = (QCocos2dCCLabelTTF*)(node);
        temp->m_fontSize = fontSize;
    }
    else if(name == "dimensions")
    {
        QCocos2dPropertiesLabelDimensions dimensions;
        dimensions.m_width = this->dataOfLine( m_lines.at(index + 6) ).toFloat();
        dimensions.m_height = this->dataOfLine( m_lines.at(index + 7) ).toFloat();
        dimensions.m_unknow = this->dataOfLine( m_lines.at(index + 8) ).toInt();
        QCocos2dCCLabelTTF* temp = (QCocos2dCCLabelTTF*)(node);
        temp->m_dimensions = dimensions;
    }
    else if(name == "horizontalAlignment")
    {
        QCocos2dPropertiesLabelAlignment alignment;
        alignment.m_type = this->dataOfLine( m_lines.at(index + 5) ).toInt();
        QCocos2dCCLabelTTF* temp = (QCocos2dCCLabelTTF*)(node);
        temp->m_horzAlignment = alignment;
    }
    else if(name == "verticalAlignment")
    {
        QCocos2dPropertiesLabelAlignment alignment;
        alignment.m_type = this->dataOfLine( m_lines.at(index + 5) ).toInt();
        QCocos2dCCLabelTTF* temp = (QCocos2dCCLabelTTF*)(node);
        temp->m_vertAlignment = alignment;
    }
    else if(name == "string") //可能是其他的情况
    {
        QCocos2dPropertiesLabelText text;
        text.m_text = this->dataOfLine( m_lines.at(index + 5) );
        QCocos2dCCLabelTTF* temp = (QCocos2dCCLabelTTF*)(node);
        temp->m_text = text;
    }

}

int QParseCCBFile::indexOf(QString key, int offset)
{
    int size = m_lines.size();
    for(int i = offset; i < size; ++i)
    {
        QString line = m_lines.at(i);
        if(line == key)
        {
            return i;
        }
    }

    return -1;
}

int QParseCCBFile::indexOfPair(QString key, int startLineIndex)
{
    //qDebug()<<"indexOfPair"<<key<<startLineIndex;
    int endLineIndex = startLineIndex;
    int count = 0;
    int size = m_lines.size();
    while(endLineIndex < size)
    {
        QString line = m_lines.at(endLineIndex);

        if(line.contains(QString("<%1>").arg(key)) == true)
        {
            count++;
        }

        if(line.contains(QString("</%1>").arg(key)) == true)
        {
            count--;
        }

        if(count == 0)
        {
            break;
        }

        endLineIndex++;
    }

    if(endLineIndex == size)
    {
        return -1;
    }

    return endLineIndex;
}

QString QParseCCBFile::dataOfLine(QString line)
{
    int index1 = line.indexOf(">");
    int index2 = line.indexOf("<", index1);
    return line.mid(index1 + 1, index2 - index1 - 1);
}

QCocos2dCCNode* QParseCCBFile::createCocos2dClass(QString key)
{
    QCocos2dCCNode* node = 0;
    if(key == "CCLayer")
    {
        node = new QCocos2dCCLayer();
    }
    else if(key == "CCLayerGradient")
    {
        node = new QCocos2dCCLayerGradient();
    }
    else if(key == "CCSprite")
    {
        node = new QCocos2dCCSprite();
    }
    else if(key == "CCMenu")
    {
        node = new QCocos2dCCMenu();
    }
    else if(key == "CCMenuItemImage")
    {
        node = new QCocos2dCCMenuItemImage();
    }
    else if(key == "CCLabelTTF")
    {
        node = new QCocos2dCCLabelTTF();
    }
    else if(key == "CCNode")
    {
        node = new QCocos2dCCNode();
    }
    else if(key == "CCLabelBMFont")
    {
        node = new QCocos2dCCLabelBMFont();
    }

    return node;
}

void QParseCCBFile::print(QCocos2dCCNode* node)
{
    node->print();
    qDebug()<<node->m_chlidren.size();
    foreach(QCocos2dCCNode* node, node->m_chlidren)
    {
        this->print(node);
    }
}
