#include "tetrixnature.h"
#include <QKeyEvent>
#include <QApplication>

#include <QTextStream>
#include <stdio.h>

TetrixNature::TetrixNature()
{
    m_moveTime = 0;
    m_transformerTime = 0;
}

void TetrixNature::copyBoard(TetrixShape* pboard,
                             TetrixPiece currentPiece,
                             TetrixPiece nextPiece)
{
    for(int j = 0; j < boardHeight; j++)
        for(int i = 0; i < boardWidth; i++)
        {
            int k = j*boardWidth + i;
            board[k] = pboard[k];
        }

    m_currentPiece = currentPiece;
    m_nextPiece = nextPiece;
    m_moveTime = 0;
    m_transformerTime = 0;
}


int TetrixNature::estimate()
{
    int temp[boardWidth*boardHeight];

    for(int j = 0; j < boardHeight; ++j)
    {
        for(int i = 0; i < boardWidth; ++i)
        {
            if(shapeAt(i,j) != NoShape)
            {
                temp[j*boardWidth + i] = 1;
            }
            else
            {
                temp[j*boardWidth + i] = 0;
            }
        }
    }

    for(int j = 0; j < boardHeight; ++j)
    {
        bool isFullLine = true;
        for(int i = 0; i < boardWidth; ++i)
        {
            if( temp[j*boardWidth + i] == 0)
            {
                isFullLine = false;
                break;
            }
        }

        if (isFullLine)
        {
            for(int k = j + 1; k < boardHeight; ++k)
            {
                for(int i = 0; i < boardWidth; ++i)
                {
                    temp[(k-1)*boardWidth + i] = temp[k*boardWidth + i];
                }
            }

            --j;
        }
    }

    int height[boardWidth] = {0};  
    for(int i = 0; i < boardWidth; ++i)
    {
        for(int j = boardHeight - 1; j >= 0 ; --j)
        {
            if(temp[j*boardWidth + i] == 1)
            {
                height[i] = j + 1;
                break;
            }
        }
    }

    int heightMax = 0;
    for(int i = 0; i < boardWidth; ++i)
    {
        if (height[i] > heightMax)
        {
            heightMax = height[i];
        }
    }

    int sigamaTall = 0;
    int sigamaDelt = 0;
    int sigamaHole = 0;
    for(int i = 0; i < boardWidth; ++i)
    {
        sigamaTall += height[i]*height[i];
    }

    for(int i = 0; i < boardWidth - 1; ++i)
    {
        int delt = height[i+1] - height[i];
        sigamaDelt += delt*delt;
    }

    for(int i = 0; i < boardWidth - 1; ++i)
    {
        int hole = 0;
        for(int j = 0; j < height[i]; ++j)
        {
            if(temp[j*boardWidth + i] == 0)
            {
                hole++;
            }
        }
        sigamaHole += hole;
    }

    int sorce = sigamaTall + sigamaDelt + sigamaHole*sigamaHole;
    return sorce;
#if 0
    system("cls");
    QTextStream cin(stdin, QIODevice::ReadOnly);
    QTextStream cout(stdout, QIODevice::WriteOnly);

    for(int j = 0; j < boardHeight; ++j)
    {
        for(int i = 0; i < boardWidth; ++i)
        {
            if (shapeAt(i,boardHeight - 1 -j) == NoShape)
            {
                cout<<"0  ";
            }
            else
            {
                cout<<"1  ";
            }
        }
        cout<<endl;
    }

    cout<<sorce<<endl;
    cout.flush();
    system("pause");
#endif
    return  sorce;
}


int TetrixNature::analysisBoard(int level)
{
    TetrixPiece piece;
    if( level == 0)
    {
        return this->estimate();
    }
    else if(level == 1)
    {
        piece = m_nextPiece;
    }
    else if(level == 2)
    {
        piece = m_currentPiece;
    }

    int transformerTime = 0;

    switch(piece.shape())
    {
        case SquareShape:
            transformerTime = 1;
            break;

        case ZShape:
        case SShape:
        case LineShape:
            transformerTime = 2;
            break;

        case TShape:
        case LShape:
        case MirroredLShape:
            transformerTime = 4;
            break;

        default:
            qDebug("It's must a bug<NoShape>");
            return 0;
    }

    int minSorce = 1<<29;

    for(int i = 0; i < transformerTime; i++)
    {
 //ÒÆµ½×î×ó±ß
        const int oriX = boardWidth / 2 + 1;
        const int oriY = boardHeight - 2 + piece.minY();
        int curX = oriX;

        while( tryMove(piece,curX - 1,oriY) )
        {
            curX = curX - 1;
        }

        while(1)
        {
            int curY = oriY;
            while( tryMove(piece, curX, curY - 1) )
            {
                curY--;
            }

            makeMove(piece, curX, curY);
            int sorce = this->analysisBoard(level - 1);
            unMakeMove(piece, curX, curY);

            if( sorce < minSorce)
            {
                minSorce = sorce;

                if (level == 2)
                {
                    m_transformerTime = i;
                    m_moveTime = curX - oriX;
                }
            }

            if( !tryMove(piece, curX + 1, oriY) )
            {
                break;
            }

            curX++;
        }

        piece = piece.rotatedLeft();
    }

    int number = 0;
    for(int j = 0; j < boardHeight; ++j)
    {
        if( isFullLine(j) )
        {
            number++;
        }
    }


//    return this->estimate() + minSorce;
    return minSorce - number;

}

void TetrixNature::sendMessage(QWidget* widget)
{
    for(int i = 0; i < m_transformerTime; ++i)
    {
        QKeyEvent event(QEvent::KeyPress, Qt::Key_Up, Qt::NoModifier);
        QApplication::sendEvent(widget, &event);
    }

    if(m_moveTime < 0)
    {
        for(int i = 0; i < (-m_moveTime); ++i)
        {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_Left, Qt::NoModifier);
            QApplication::sendEvent(widget, &event);
        }
    }

    if(m_moveTime > 0)
    {
        for(int i = 0; i < m_moveTime; ++i)
        {
            QKeyEvent event(QEvent::KeyPress, Qt::Key_Right, Qt::NoModifier);
            QApplication::sendEvent(widget, &event);
        }
    }

    QKeyEvent event(QEvent::KeyPress, Qt::Key_Space, Qt::NoModifier);
    QApplication::sendEvent(widget, &event);
}

bool TetrixNature::tryMove(const TetrixPiece &newPiece, int newX, int newY)
{
    for (int i = 0; i < 4; ++i)
    {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);
        if (x < 0 || x >= boardWidth || y < 0 || y >= boardHeight)
            return false;
        if (shapeAt(x, y) != NoShape)
            return false;
    }

    return true;
}

void TetrixNature::makeMove(const TetrixPiece &newPiece, int newX, int newY)
{
    for(int i = 0; i < 4; ++i)
    {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);

        if (shapeAt(x, y) != NoShape)
        {
            qDebug("It.s must be a bug.<makeMove>");
            qDebug("x = %d,y = %d",x,y);
        }

        shapeAt(x, y) = SShape;
    }
}

void TetrixNature::unMakeMove(const TetrixPiece &newPiece, int newX, int newY)
{
    for(int i = 0; i < 4; ++i)
    {
        int x = newX + newPiece.x(i);
        int y = newY - newPiece.y(i);

        if (shapeAt(x, y) == NoShape)
        {
            qDebug("It.s must be a bug.<unMakeMove>");
            qDebug("x = %d,y = %d",x,y);
        }

        shapeAt(x, y) = NoShape;
    }
}

int TetrixNature::getHeight(int index)
{
    int height = 0;

    for(int j = boardHeight - 1; j >= 0 ; --j)
    {
        if(shapeAt(index,j) != NoShape)
        {
            height = j + 1;
            break;
        }
    }

    return height;
}

bool TetrixNature::isFullLine(int height)
{
    for(int i = 0; i < boardWidth; ++i)
    {
        if(shapeAt(i,height) == NoShape)
        {
            return false;
        }
    }

    return true;
}

int TetrixNature::getHoleNumber()
{
    int hole = 0;

    for(int i = 0; i < boardWidth; ++i)
    {
        int height = 0;

        for(int j = boardHeight - 1; j >= 0; --j)
        {
            if(shapeAt(i,j) != NoShape)
            {
                if ( !this->isFullLine(j) )
                {
                    height = j + 1;
                    break;
                }
            }
        }

        for(int j = 0; j < height; ++j)
        {
            if(shapeAt(i,j) == NoShape)
            {
                ++hole;
            }
        }
    }

    return hole;
}

