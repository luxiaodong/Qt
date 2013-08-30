#ifndef QRUNEMASTERDATA_H
#define QRUNEMASTERDATA_H

#define IMAGE_UNKNOW 0

#define RUNE_WIDTH 5
#define RUNE_HEIGHT 7
#define RUNE_DEEP 3

#define DIRECTION_UP 1
#define DIRECTION_UP_RIGHT 2
#define DIRECTION_RIGHT 3
#define DIRECTION_RIGHT_DOWN 4

class QRuneSingle
{
public:
    QRuneSingle(){m_imageIndex = 0; m_spriteNumber = 0; m_isIce = false; m_isToRemove = false;}
    int     m_imageIndex;   //1 - 5
    int     m_spriteNumber; //0 - 4
    bool    m_isIce;
    bool    m_isToRemove;
};

class QRuneMasterData
{
public:
    QRuneMasterData();
    int checkContinueRuneNumber(int x, int y, int direction, bool addRemoveTag = false);
    int checkContinueRuneNumberOfDirectionUp(int x, int y, bool addRemoveTag = false);
    int checkContinueRuneNumberOfDirectionUpRight(int x, int y, bool addRemoveTag = false);
    int checkContinueRuneNumberOfDirectionRight(int x, int y, bool addRemoveTag = false);
    int checkContinueRuneNumberOfDirectionRightDown(int x, int y, bool addRemoveTag = false);

    bool canSwapRune(int x, int y, int direction);
    bool canSwapRune(QRuneSingle rune1, QRuneSingle rune2);

    int scoreAtSingles();
    bool isExistRemoveRune();
    void updateRuneData();

    void findBestSwap();
    void copyRuneSingleDeep();
    void makeSwap(int x, int y, int direction);
    void unMakeSwap(int x, int y, int direction);
    void makeSwap(int x1, int y1, int x2, int y2);
    void unMakeSwap(int x1, int y1, int x2, int y2){return makeSwap(x1,y1,x2,y2);}

public:
    QRuneSingle m_runeSingles[RUNE_WIDTH][RUNE_HEIGHT];
    QRuneSingle m_runeSingleCopys[RUNE_WIDTH][RUNE_HEIGHT];
    int m_deep;
    int m_continueStep;
    int m_spriteScore;

    int m_swapX;
    int m_swapY;
    int m_swapZ;
};

#endif // QRUNEMASTERDATA_H
