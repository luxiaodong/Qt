#include "qplayer.h"

QPlayer::QPlayer()
    :m_attackEquip(0),
     m_defenseEquip(0),
     m_attackHorse(0),
     m_defenseHorse(0),
     m_judgeShandian(0),
     m_judgeLebusishu(0)
{
    this->m_role = role_none;
    m_handCards.clear();
}

QPlayer::QPlayer(Role role)
    :m_attackEquip(0),
     m_defenseEquip(0),
     m_attackHorse(0),
     m_defenseHorse(0),
     m_judgeShandian(0),
     m_judgeLebusishu(0)
{
    this->m_role = role;
    m_handCards.clear();
}

bool QPlayer::hasHandCard(QCard* card)
{
    if( m_handCards.indexOf(card) == -1)
    {
        return false;
    }

    return true;
}

bool QPlayer::hasCard(QCard* card)
{
    bool r = hasHandCard(card);
    r = r || (card == m_attackEquip);
    r = r || (card == m_defenseEquip);
    r = r || (card == m_attackHorse);
    r = r || (card == m_defenseHorse);
    r = r || (card == m_judgeShandian);
    r = r || (card == m_judgeLebusishu);

    return r;
}

void QPlayer::dropCard(QCard* card)
{
    if(card)
    {
        return;
    }

    if(card == m_attackEquip)
    {
        m_attackEquip = 0;
        return ;
    }
    else if(card == m_defenseEquip)
    {
        m_defenseEquip = 0;
        return ;
    }
    else if(card == m_attackHorse)
    {
        m_attackHorse = 0;
        return ;
    }
    else if(card == m_defenseHorse)
    {
        m_defenseHorse = 0;
        return ;
    }
    else if(card == m_judgeShandian)
    {
        m_judgeShandian = 0;
        return ;
    }
    else if(card == m_judgeLebusishu)
    {
        m_judgeLebusishu = 0;
        return ;
    }

    dropHandCard(card);
}

void QPlayer::dropHandCard(QCard* card)
{
    for(int i = 0; i < m_handCards.size(); ++i)
    {
        if(card == m_handCards[i])
        {
            m_handCards.removeAt(i);
            return ;
        }
    }
}

