#include "qrunemasterdata.h"
#include "QDebug"

QRuneMasterData::QRuneMasterData()
{
}

void QRuneMasterData::findBestSwap()
{
    int totalScore = 0;
    for(int i = 0; i < RUNE_WIDTH; ++i)
    {
        for(int j = 0; j < RUNE_HEIGHT; ++j)
        {
            for(int k = 1; k <= 4; ++k)
            {
                this->copyRuneSingleDeep();
                if(this->canSwapRune(i,j,k) == true)
                {
                    makeSwap(i,j,k);
                    m_continueStep = 1;
                    m_spriteScore = 0;
                    int tempScore = 0;

                    while(1)
                    {
                        if(this->isExistRemoveRune() == false)
                        {
                            tempScore += scoreAtSingles();
                            break;
                        }

                        tempScore += scoreAtSingles() + m_spriteScore;
                        this->updateRuneData();
                        m_continueStep++;
                    }

                    if(tempScore > totalScore)
                    {
                        totalScore = tempScore;
                        m_swapX = i;
                        m_swapY = j;
                        m_swapZ = k;

                        qDebug()<<m_swapX<<m_swapY<<m_swapZ<<totalScore;
                    }
                }
            }
        }
    }
}

int QRuneMasterData::scoreAtSingles()
{
    int score = 0;
    for(int i = 0; i < RUNE_WIDTH; ++i)
    {
        for(int j = 0; j < RUNE_HEIGHT; ++j)
        {
            for(int k = 1; k <= 4; ++k)
            {
                int count = checkContinueRuneNumber(i,j,k,false);
                if(count >= 4)
                {
                    score += 100*m_continueStep;
                    qDebug()<<i<<j<<k<<count<<m_continueStep;
                }

                score += count;
            }
        }
    }
    return score;
}

bool QRuneMasterData::isExistRemoveRune()
{
    bool exist = false;
    for(int i = 0; i < RUNE_WIDTH; ++i)
    {
        for(int j = 0; j < RUNE_HEIGHT; ++j)
        {
            for(int k = 1; k <= 4; ++k)
            {
                int temp = checkContinueRuneNumber(i,j,k,false);
                if(temp >= 4)
                {
                    checkContinueRuneNumber(i,j,k,true);
                    exist = true;
                }
            }
        }
    }
    return exist;
}

void QRuneMasterData::updateRuneData()
{
    for(int i = 0; i < RUNE_WIDTH; ++i)
    {
        for(int j = 0; j < RUNE_HEIGHT; ++j)
        {
            QRuneSingle single = m_runeSingleCopys[i][j];
            if(single.m_isToRemove == true)
            {
                for(int k = j + 1; k < RUNE_HEIGHT; ++k)
                {
                    m_runeSingleCopys[i][k - 1] = m_runeSingleCopys[i][k];
                }
                QRuneSingle single2;
                m_runeSingleCopys[i][RUNE_HEIGHT - 1] = single2;
            }
        }
    }
}

int QRuneMasterData::checkContinueRuneNumber(int x, int y, int direction, bool addRemoveTag)
{
    QRuneSingle single = m_runeSingleCopys[x][y];
    if(single.m_imageIndex == IMAGE_UNKNOW)
    {
        return 0;
    }

    int number = 0;

    if(direction == DIRECTION_UP)
    {
        number = this->checkContinueRuneNumberOfDirectionUp(x,y,addRemoveTag);
    }
    else if(direction == DIRECTION_UP_RIGHT)
    {
        number = this->checkContinueRuneNumberOfDirectionUpRight(x,y,addRemoveTag);
    }
    else if(direction == DIRECTION_RIGHT)
    {
        number = this->checkContinueRuneNumberOfDirectionRight(x,y,addRemoveTag);
    }
    else if(direction == DIRECTION_RIGHT_DOWN)
    {
        number = this->checkContinueRuneNumberOfDirectionRightDown(x,y,addRemoveTag);
    }

    return number;
}

int QRuneMasterData::checkContinueRuneNumberOfDirectionUp(int x, int y, bool addRemoveTag)
{
    QRuneSingle single = m_runeSingleCopys[x][y];
    int imageIndex = single.m_imageIndex;

    int count = 1;
    int i = x;
    int j = y + 1;

    if(addRemoveTag == true)
    {
        single.m_isToRemove = true;
    }

    while(j < RUNE_HEIGHT)
    {
        int tempIndex = m_runeSingleCopys[i][j].m_imageIndex;

        if(tempIndex == imageIndex)
        {
            if(addRemoveTag == true)
            {
                m_runeSingleCopys[i][j].m_isToRemove = true;

                if(m_runeSingleCopys[i][j].m_spriteNumber > 0)
                {
                    m_spriteScore += 1000;
                }
            }

            count++;
            j++;
        }
        else
        {
            break;
        }
    }

    return count;
}

int QRuneMasterData::checkContinueRuneNumberOfDirectionUpRight(int x, int y, bool addRemoveTag)
{
    QRuneSingle single = m_runeSingleCopys[x][y];
    int imageIndex = single.m_imageIndex;

    int count = 1;
    int i = x + 1;
    int j = y + 1;

    if(addRemoveTag == true)
    {
        single.m_isToRemove = true;
    }

    while(j < RUNE_HEIGHT && i < RUNE_WIDTH)
    {
        int tempIndex = m_runeSingleCopys[i][j].m_imageIndex;

        if(tempIndex == imageIndex)
        {
            if(addRemoveTag == true)
            {
                m_runeSingleCopys[i][j].m_isToRemove = true;
                if(m_runeSingleCopys[i][j].m_spriteNumber > 0)
                {
                    m_spriteScore += 1000;
                }
            }
            count++;
            j++;
            i++;
        }
        else
        {
            break;
        }
    }

    return count;
}

int QRuneMasterData::checkContinueRuneNumberOfDirectionRight(int x, int y, bool addRemoveTag)
{
    QRuneSingle single = m_runeSingleCopys[x][y];
    int imageIndex = single.m_imageIndex;

    int count = 1;
    int i = x + 1;
    int j = y;

    if(addRemoveTag == true)
    {
        single.m_isToRemove = true;
    }

    while(i < RUNE_WIDTH)
    {
        int tempIndex = m_runeSingleCopys[i][j].m_imageIndex;

        if(tempIndex == imageIndex)
        {
            if(addRemoveTag == true)
            {
                m_runeSingleCopys[i][j].m_isToRemove = true;
                if(m_runeSingleCopys[i][j].m_spriteNumber > 0)
                {
                    m_spriteScore += 1000;
                }
            }
            count++;
            i++;
        }
        else
        {
            break;
        }
    }

    return count;
}

int QRuneMasterData::checkContinueRuneNumberOfDirectionRightDown(int x, int y, bool addRemoveTag)
{
    QRuneSingle single = m_runeSingleCopys[x][y];
    int imageIndex = single.m_imageIndex;

    int count = 1;
    int i = x + 1;
    int j = y - 1;

    if(addRemoveTag == true)
    {
        single.m_isToRemove = true;
    }

    while(i < RUNE_WIDTH && j >= 0)
    {
        int tempIndex = m_runeSingleCopys[i][j].m_imageIndex;

        if(tempIndex == imageIndex)
        {
            if(addRemoveTag == true)
            {
                m_runeSingleCopys[i][j].m_isToRemove = true;
                if(m_runeSingleCopys[i][j].m_spriteNumber > 0)
                {
                    m_spriteScore += 1000;
                }
            }
            count++;
            i++;
            y--;
        }
        else
        {
            break;
        }
    }

    return count;
}

bool QRuneMasterData::canSwapRune(int x, int y, int direction)
{
    int i = x;
    int j = y;

    if(direction == DIRECTION_UP)
    {
        j++;
    }
    else if(direction == DIRECTION_UP_RIGHT)
    {
        j++;
        i++;
    }
    else if(direction == DIRECTION_RIGHT)
    {
        i++;
    }
    else if(direction == DIRECTION_RIGHT_DOWN)
    {
        i++;
        j--;
    }

    if( i < 0 || i >= RUNE_WIDTH)
    {
        return false;
    }

    if( j < 0 || j >= RUNE_HEIGHT)
    {
        return false;
    }

    QRuneSingle rune1 = m_runeSingleCopys[x][y];
    QRuneSingle rune2 = m_runeSingleCopys[i][j];

    return this->canSwapRune(rune1,rune2);
}

bool QRuneMasterData::canSwapRune(QRuneSingle rune1, QRuneSingle rune2)
{
    if(rune1.m_isIce == true ||
       rune2.m_isIce == true)
    {
        return false;
    }

    if(rune1.m_imageIndex == rune2.m_imageIndex)
    {
        return false;
    }

    return true;
}

void QRuneMasterData::copyRuneSingleDeep()
{
    for(int i = 0; i < RUNE_WIDTH; ++i)
    {
        for(int j = 0; j < RUNE_HEIGHT; ++j)
        {
            m_runeSingleCopys[i][j] = m_runeSingles[i][j];
        }
    }
}

void QRuneMasterData::makeSwap(int x1, int y1, int x2, int y2)
{
    QRuneSingle rune1 = m_runeSingleCopys[x1][y1];
    QRuneSingle rune2 = m_runeSingleCopys[x2][y2];

    m_runeSingleCopys[x1][y1] = rune2;
    m_runeSingleCopys[x2][y2] = rune1;
}

void QRuneMasterData::makeSwap(int x, int y, int direction)
{
    int i = x;
    int j = y;

    if(direction == DIRECTION_UP)
    {
        j++;
    }
    else if(direction == DIRECTION_UP_RIGHT)
    {
        j++;
        i++;
    }
    else if(direction == DIRECTION_RIGHT)
    {
        i++;
    }
    else if(direction == DIRECTION_RIGHT_DOWN)
    {
        i++;
        j--;
    }

    return makeSwap(x,y,i,j);
}


void QRuneMasterData::unMakeSwap(int x, int y, int direction)
{
    return makeSwap(x,y,direction);
}

