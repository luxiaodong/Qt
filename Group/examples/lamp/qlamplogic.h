#ifndef QLAMPLOGIC_H
#define QLAMPLOGIC_H

#include <QList>
#include "../../model/qbinarymodel.h"

enum {
    DRAW_NONE = 0,
    DRAW_O = 1,
    DRAW_X = 2
};

enum {
    FLIP_NONE = 0,
    FLIP_CROSS = 1,
    FLIP_SQUARE = 2
};

class QGild
{
public:
    int m_drawType;
    int m_flipType;
};

class QLampLogic
{
public:
    QLampLogic();
    int horzGildNumber() const {return m_horzGildNumber;}
    int vertGildNumber() const {return m_vertGildNumber;}
    void setGildNumber(const int horz, const int vert);
    void setGild(const int draw, const int flip);
    int drawType(const int x, const int y) const;
    void mainFlip(const int x, const int y);

    void searchSlove();
private:
    void flip(const int x, const int y);
    void flipCross(const int x, const int y);
    void flipSquare(const int x, const int y);
private:
    QList<QGild> m_gildType;
    int m_horzGildNumber;
    int m_vertGildNumber;
};

#endif
