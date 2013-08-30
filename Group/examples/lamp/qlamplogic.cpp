#include "qlamplogic.h"


QLampLogic::QLampLogic()
{
    m_gildType.clear();
    this->m_horzGildNumber = 0;
    this->m_vertGildNumber = 0;
}

void QLampLogic::setGildNumber(const int horz, const int vert)
{
    this->m_horzGildNumber = horz;
    this->m_vertGildNumber = vert;
}

void QLampLogic::setGild(const int draw, const int flip)
{
    QGild gild;
    gild.m_drawType = draw;
    gild.m_flipType = flip;
    m_gildType.append(gild);
}

int QLampLogic::drawType(const int x, const int y) const
{
    int index = this->m_horzGildNumber*x + y;

    if(index >= this->m_gildType.size())
    {
        return (int)DRAW_NONE;
    }

    return this->m_gildType.at(index).m_drawType;
}

void QLampLogic::mainFlip(const int x,const int y)
{
    int index = x*m_horzGildNumber + y;

    if(index >= this->m_gildType.size())
    {
        return ;
    }

    int filpType = this->m_gildType.at(index).m_flipType;

    if(filpType == (int)FLIP_CROSS)
    {
        this->flipCross(x,y);
    }
    else if(filpType == (int)FLIP_SQUARE)
    {
        this->flipSquare(x,y);
    }

}

void QLampLogic::flipCross(const int x, const int y)
{
    this->flip(x,y);
    this->flip(x-1,y);
    this->flip(x,y-1);
    this->flip(x+1,y);
    this->flip(x,y+1);
}

void QLampLogic::flipSquare(const int x, const int y)
{
    this->flip(x,y);
    this->flip(x-1,y);
    this->flip(x,y-1);
    this->flip(x+1,y);
    this->flip(x,y+1);
    this->flip(x-1,y-1);
    this->flip(x+1,y-1);
    this->flip(x+1,y+1);
    this->flip(x-1,y+1);
}

void QLampLogic::flip(const int x, const int y)
{
    if(x < 0 || x >= m_horzGildNumber)
    {
        return ;
    }

    if(y < 0 || y >= m_vertGildNumber)
    {
        return ;
    }

    int draw = this->drawType(x,y);
    int index = x*m_horzGildNumber + y;

    if(draw == (int)DRAW_NONE)
    {
        return ;
    }
    else if(draw == (int)DRAW_O)
    {
        QGild gild;
        gild.m_drawType = (int)DRAW_X;
        gild.m_flipType = m_gildType.at(index).m_flipType;
        m_gildType.replace(index, gild);
    }
    else if(draw == (int)DRAW_X)
    {
        QGild gild;
        gild.m_drawType = (int)DRAW_O;
        gild.m_flipType = m_gildType.at(index).m_flipType;
        m_gildType.replace(index, gild);
    }
}

void QLampLogic::searchSlove()
{
    QList<int> base;

    for(int i = 0; i < m_horzGildNumber; ++i)
    {
        for(int j = 0; j < m_vertGildNumber; ++j)
        {
            int draw = this->m_gildType.at(i*m_horzGildNumber + j).m_drawType;
            base.append(draw);
        }
    }

    QList<QBitArray> rule;

    for(int i = 0; i < m_horzGildNumber; ++i)
    {
        for(int j = 0; j < m_vertGildNumber; ++j)
        {
            int flip = this->m_gildType.at(i*m_horzGildNumber + j).m_flipType;

        }
    }
}

