#include "qdesk.h"

QDesk::QDesk(QList<QTcpSocket*> sockets)
{
    m_players.clear();
    int playerNumber = sockets.size();
    for(int i = 0; i < playerNumber; ++i)
    {
        QPlayer* player = new QPlayer();
        player->setSocket(sockets.at(i));
        m_players.append(player);
    }

    m_backCards.clear();
    m_frontCards.clear();

    for(int i = 0; i < m_cards.size(); ++i)
    {
        m_frontCards.append(i);
    }

    loadData();
}

bool QDesk::loadData()
{
    bool flag1 = this->readPlayCardData();
    bool flag2 = this->readHeroData();
    bool flag3 = this->readSkillData();
    return (flag1 && flag2 && flag3);
}

bool QDesk::readPlayCardData()
{
    QString fileName = QString("PlayCard.dat");
    fileName = QDir::currentPath() + "/" + fileName;
    QFile file(fileName);
    if( !file.open(QIODevice::ReadOnly) )
    {
        return false;
    }

    QDataStream bin(&file);

    quint8 byte1;
    quint8 byte2;
    quint8 byte3;
    quint8 byte4;

    bin>>byte1>>byte2>>byte3>>byte4;
    bin>>byte1>>byte2>>byte3>>byte4;
    int count = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
    bin>>byte1>>byte2>>byte3>>byte4;
    int length = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
    bin>>byte1>>byte2>>byte3>>byte4;
    bin>>byte1>>byte2>>byte3>>byte4;

    int j = 0;
    while(j < count)
    {
        QString str = QString();
        int array[11] = {0,0,0,0,0,0,0,0,0,0,0};
        int i = 0;
        for (i = 0; i < length; ++i)
        {
            bin>>byte1;
            bin>>byte2;
            bin>>byte3;
            bin>>byte4;

            array[i] = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
        }
//qDebug("%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d",array[0],array[1],array[2],array[3],array[4],array[5],array[6],array[7],array[8]);
        QCard* card = new QCard(array[0],array[1],array[2],array[3],array[4]);
        m_cards.append(card);

        ++j;
    }
    file.close();
    return true;
}

bool QDesk::readHeroData()
{
    QString fileName = QString("Character.dat");

    fileName = QDir::currentPath() + "/" + fileName;
    QFile file(fileName);
    if( !file.open(QIODevice::ReadOnly) )
    {
        return false;
    }

    QDataStream bin(&file);

    quint8 byte1;
    quint8 byte2;
    quint8 byte3;
    quint8 byte4;

    bin>>byte1>>byte2>>byte3>>byte4;
    bin>>byte1>>byte2>>byte3>>byte4;
    int count = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
    bin>>byte1>>byte2>>byte3>>byte4;
    int length = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
    bin>>byte1>>byte2>>byte3>>byte4;
    bin>>byte1>>byte2>>byte3>>byte4;

    int j = 0;
    while(j < count)
    {
        QString str = QString();
        int array[11] = {0,0,0,0,0,0,0,0,0,0,0};
        int i = 0;
        for (i = 0; i < length; ++i)
        {
            bin>>byte1;
            bin>>byte2;
            bin>>byte3;
            bin>>byte4;

            array[i] = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
        }
//qDebug("%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d",array[0],array[1],array[2],array[3],array[4],array[5],array[6],array[7],array[8]);

        QHero* hero = new QHero(array[0],array[2],array[3],array[4],array[5],array[6],array[7]);
        m_heros.append(hero);
        ++j;
    }
    file.close();
    return true;
}

bool QDesk::readSkillData()
{
    QString fileName = QString("spell.dat");

    fileName = QDir::currentPath() + "/" + fileName;
    QFile file(fileName);
    if( !file.open(QIODevice::ReadOnly) )
    {
        return false;
    }

    QDataStream bin(&file);

    quint8 byte1;
    quint8 byte2;
    quint8 byte3;
    quint8 byte4;

    bin>>byte1>>byte2>>byte3>>byte4;
    bin>>byte1>>byte2>>byte3>>byte4;
    int count = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
    bin>>byte1>>byte2>>byte3>>byte4;
    int length = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
    bin>>byte1>>byte2>>byte3>>byte4;
    bin>>byte1>>byte2>>byte3>>byte4;

    int j = 0;
    while(j < count)
    {
        QString str = QString();
        int array[11] = {0,0,0,0,0,0,0,0,0,0,0};
        int i = 0;
        for (i = 0; i < length; ++i)
        {
            bin>>byte1;
            bin>>byte2;
            bin>>byte3;
            bin>>byte4;

            array[i] = (byte4<<24) + (byte3<<16) + (byte2<<8) + (byte1);
        }
//qDebug("%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d,%4d",array[0],array[1],array[2],array[3],array[4],array[5],array[6],array[7],array[8]);
        QSkill* skill = new QSkill(array[0],array[1],array[2]);
        m_skills.append(skill);
        ++j;
    }
    file.close();
    return true;
}

int QDesk::distanceBetweenTwoPlayer(QPlayer* one, QPlayer* another) const
{
    if((one == 0) || (another == 0))
    {
        return -1;
    }

    if(one->isDead() || another->isDead())
    {
        return -1;
    }

    int indexOne = m_players.indexOf(one);
    int indexAnother = m_players.indexOf(another);

    if((indexOne == -1) || (indexAnother == -1))
    {
        return -1;
    }

    if(one == another)
    {
        return 0;
    }

    int smallIndex = (indexOne < indexAnother) ? indexOne : indexAnother;
    int bigIndex = (indexOne > indexAnother) ? indexOne : indexAnother;

    int distance1 = 0;
    int distance2 = 0;

    distance1 = bigIndex - smallIndex;
    for(int i = smallIndex + 1; i < bigIndex; i++)
    {
        if(m_players.at(i)->isDead() == true)
        {
            distance1--;
        }
    }

    int playerCount = this->playerCount();
    distance2 = smallIndex - bigIndex + playerCount;
    for(int i = bigIndex + 1; i < smallIndex + playerCount; ++i)
    {
        int index = i%playerCount;
        if(m_players.at(index)->isDead() == true)
        {
            distance2--;
        }
    }

    return (distance1 < distance2) ? distance1 : distance2;
}

QPlayer* QDesk::playerAt(int index) const
{
    Q_ASSERT(index < m_players.count());
    return m_players[index];
}

QCard* QDesk::cardAt(int index)const
{
    Q_ASSERT(index < m_cards.count());
    return m_cards[index];
}

QHero* QDesk::heroAt(int index)const
{
    Q_ASSERT(index < m_heros.count());
    return m_heros[index];
}

QSkill* QDesk::skillAt(int index)const
{
    Q_ASSERT(index < m_skills.count());
    return m_skills[index];
}

QPlayer* QDesk::zhugong()const
{
    int playerNumber = m_players.count();

    for(int i = 0; i < playerNumber; ++i)
    {
        if(m_players.at(i)->isZhugong())
        {
            return m_players[i];
        }
    }

    Q_ASSERT(0);
    return 0;
}

void QDesk::shuffle()
{
//随机的将front给back
    int size = m_frontCards.size();
    while(size > 0)
    {
        int index = qrand()%size;
        int value = m_frontCards[index];
        m_frontCards.removeAt(index);
        m_backCards.append(value);
        size--;
    }
}

int QDesk::popCard()
{
    if( m_backCards.isEmpty() )
    {
        this->shuffle();
    }

    int index = m_backCards.at(0);
    m_backCards.removeAt(0);
    m_frontCards.append(index);
    return index;
}

int QDesk::lookCard(int index)
{
    if( m_backCards.count() <= index )
    {
        this->shuffle();
    }

    return m_backCards[index];
}

int QDesk::livePlayerCount()const
{
    int number = m_players.size();
    for(int i = 0; i < m_players.size(); ++i)
    {
        if(m_players.at(i)->isDead())
        {
            number--;
        }
    }

    return number;
}

int QDesk::guanxinNeedCardNumber()
{
    int needCard = livePlayerCount() < 5 ? livePlayerCount() : 5;
    if(m_backCards.count() < needCard)
    {
        shuffle();
    }

    return needCard;
}

void QDesk::sortCardAfterGuanxin(QList<int> cards)
{
    Q_ASSERT(cards.size() <= m_backCards.size());

    int size = cards.size();
    for(int i = 0; i < size; ++i)
    {
        m_backCards.replace(i, cards[i]);
    }
}

QPlayer* QDesk::hasGaipanSkill()const
{
    for(int i = 0; i < m_players.size(); ++i)
    {
        QPlayer* player = m_players[i];
        if(player->isDead() == false)
        {
            QHero* hero = player->m_hero;
            if(hero->hasGuicai() /* || hero->hasGuidao() */ )
            {
                return player;
            }
        }
    }

    return 0;
}

QPlayer* QDesk::nextPlayer(QPlayer* current)const
{
    int index = m_players.indexOf(current);
    Q_ASSERT(index != -1);
    int playerCount = this->playerCount();

    for(int i = index + 1; i < index + playerCount; ++i)
    {
        int j = i%playerCount;
        if(m_players.at(j)->isDead() == false)
        {
            return m_players[j];
        }
    }

    Q_ASSERT(0);
    return 0;
}

bool QDesk::hasHandCard(QPlayer* player, QCard* card)const
{
    Q_ASSERT(player && card);
    QList<QCard*> handCards = player->m_handCards;
    for(int i = 0; i < handCards.size(); ++i)
    {
        if(handCards[i] == card)
        {
            return true;
        }
    }

    return false;
}

void QDesk::sendCard(QPlayer* player, int number)
{
    Q_ASSERT(player && number > 0);
    for(int i = 0; i < number; i++)
    {
        int index = this->popCard();
        QCard* card = this->cardAt(index);
        player->m_handCards.append(card);
    }
}

//用于顺手牵羊 和 过河拆桥
QCard* QDesk::findWhichCardSelected(QPlayer* player,int index)const
{
    Q_ASSERT(player);

    if(index >= 0)
    {
        QCard* card = this->cardAt(index);
        if(player->hasHandCard(card))
        {
            return card;
        }
    }
    else if(index == -1)
    {
        return player->m_attackEquip;
    }
    else if(index == -2)
    {
        return player->m_defenseEquip;
    }
    else if(index == -3)
    {
        return player->m_attackHorse;
    }
    else if(index == -4)
    {
        return player->m_defenseHorse;
    }
    else if(index == -5)
    {
        return player->m_judgeShandian;
    }
    else if(index == -6)
    {
        return player->m_judgeLebusishu;
    }

    return 0;
}

void QDesk::dropCard(QPlayer *player, int index)
{
    QCard* card = m_cards.at(index);
    return dropCard(player,card);
}

void QDesk::dropCard(QPlayer* player, QCard* card)
{
    Q_ASSERT(player->hasCard(card));
    int index = m_cards.indexOf(card);
    Q_ASSERT(index != -1);
    m_frontCards.append(index);
}

void QDesk::dropHandCard(QPlayer* player, QCard* card)
{
    Q_ASSERT(player->hasHandCard(card));
    int index = m_cards.indexOf(card);
    m_frontCards.append(index);
}

void QDesk::createWugufengdengCards()
{
    int playerCount = this->livePlayerCount();

    if(playerCount > m_backCards.count())
    {
        this->shuffle();
    }

    m_selectCards.clear();
    for(int i = 0; i < playerCount; ++i)
    {
        int index = this->popCard();
        QCard* card = this->cardAt(index);
        m_selectCards.append(card);
    }
}

void QDesk::destoryWugufengdengResidualCards()
{
    int count = m_selectCards.count();
    for(int i = 0; i < count; ++i)
    {
        QCard* card = m_selectCards[i];
        int index = m_cards.indexOf(card);
        if(index != -1)
        {
            m_frontCards.append(index);
        }
    }
    m_selectCards.clear();
}

void QDesk::selectWugufengdengCard(QPlayer* player, QCard* card)
{
    // ?如果删除将引起 偏移量 错位
    if(card == 0)
    {
        player->m_handCards.append(m_selectCards.first());

    }
}

bool QDesk::isGameOverAfterLeave(QPlayer *player)const
{
    if(player->isZhugong())
    {
        return true;
    }

    if(player->isZhongchen())
    {
        return false;
    }

    int feizeiLifeNumber = 0;
    for(int i = 0; i < m_players.size(); ++i)
    {
       if(m_players[i]->isFanzei() &&
          m_players[i]->isDead() == false)
       {
           feizeiLifeNumber++;
       }
    }

    if(player->isNeijian())
    {
        if(feizeiLifeNumber == 0)
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    if(player->isFanzei())
    {
        if(feizeiLifeNumber > 0)
        {
            return false;
        }

        for(int i = 0; i < m_players.size(); ++i)
        {
            if(m_players[i]->isNeijian() &&
               m_players[i]->isDead() == false)
            {
                return false;
            }
        }

        return true;
    }

    return false;
}


