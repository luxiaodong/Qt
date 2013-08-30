#ifndef QDESK_H
#define QDESK_H

#include <QDataStream>
#include <QString>
#include <QFile>
#include <QDir>
#include "../qplayer.h"

class QDesk
{
public:
    QDesk(QList<QTcpSocket*> sockets);

    bool loadData();
    bool readPlayCardData();
    bool readHeroData();
    bool readSkillData();

    void shuffle();
    int popCard();
    int lookCard(int index);
    int guanxinNeedCardNumber();
    void sortCardAfterGuanxin(QList<int> cards);
    void createWugufengdengCards();
    void selectWugufengdengCard(QPlayer* player, QCard* card = 0);
    void destoryWugufengdengResidualCards();

    QPlayer* hasGaipanSkill()const;
    QPlayer* nextPlayer(QPlayer* current)const;

    QCard* findWhichCardSelected(QPlayer* player,int index)const;

    bool hasHandCard(QPlayer* player, QCard* card)const; //判断特定的手牌
    void sendCard(QPlayer* player, int number);//发牌
    void dropCard(QPlayer* player, QCard* card);//回收牌，玩家打出的
    void dropCard(QPlayer *player, int index);
    void dropHandCard(QPlayer* player, QCard* card);

//inquiry
    int distanceBetweenTwoPlayer(QPlayer* one, QPlayer* another) const;
    int playerCount()const{return m_players.count();}
    int livePlayerCount()const;

    bool isGameOverAfterLeave(QPlayer *player)const;

    QPlayer* playerAt(int index)const;
    QCard* cardAt(int index)const;
    QHero* heroAt(int index)const;
    QSkill* skillAt(int index)const;

    QPlayer* zhugong()const;
public:
    QList<QCard*> m_selectCards;
private:
    QList<QPlayer*> m_players;
    QList<QCard*> m_cards;
    QList<QHero*> m_heros;
    QList<QSkill*> m_skills;

    QList<int> m_backCards;//摸牌堆
    QList<int> m_frontCards;//明牌堆
};

#endif // QDESK_H
