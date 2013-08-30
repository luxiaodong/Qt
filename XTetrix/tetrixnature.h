#ifndef TETRIXNATURE_H
#define TETRIXNATURE_H

#include "tetrixpiece.h"
#include <QWidget>

class TetrixNature
{
public:
    TetrixNature();
    void    copyBoard(TetrixShape* pboard, TetrixPiece currentPiece, TetrixPiece nextPiece);
    int     analysisBoard(int level = 2);
    void    sendMessage(QWidget* widget);
private:
    int     estimate();
    int     getHeight(int index);
    int     getHoleNumber();
    bool    isFullLine(int height);
    bool    tryMove(const TetrixPiece &newPiece, int newX, int newY);
    void    makeMove(const TetrixPiece &newPiece, int newX, int newY);
    void    unMakeMove(const TetrixPiece &newPiece, int newX, int newY);
    TetrixShape &shapeAt(int x, int y) { return board[(y * boardWidth) + x]; }
private:
    enum { boardWidth = 10, boardHeight = 22 };
    TetrixShape board[boardWidth * boardHeight];

    TetrixPiece m_currentPiece;
    TetrixPiece m_nextPiece;

    int m_moveTime;
    int m_transformerTime;
};

#endif // TETRIXNATURE_H
