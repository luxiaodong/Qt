#ifndef QPLAYER_H
#define QPLAYER_H

#include <QList>
#include <QTcpSocket>

#include "qcard.h"
#include "qhero.h"
#include "qskill.h"

enum Role{
    role_none = 0,
    role_zhugong = 1,
    role_zhongchen = 2,
    role_fanzei = 3,
    role_neijian = 4,
};

class QPlayer
{
public:
    QPlayer();
    QPlayer(Role role);
    void setHero(QHero* hero){this->m_hero = hero;}
    void setSocket(QTcpSocket* socket){this->m_socket = socket;}
    void setRole(Role role){m_role = role;}

    void dropCard(QCard* card);
    void dropHandCard(QCard* card);

//check function
    bool isDead()const{return (m_hp <= 0);}
    bool isRobot()const{return false;}
    bool isZhugong()const{return (m_role == role_zhugong);}
    bool isZhongchen()const{return (m_role == role_zhongchen);}
    bool isFanzei()const{return (m_role == role_fanzei);}
    bool isNeijian()const{return (m_role == role_neijian);}
    bool hasHandCard()const{return !m_handCards.isEmpty();}
    bool hasJudgeCard()const{return m_judgeShandian || m_judgeLebusishu;}
    bool hasAttackEquip()const{return m_attackEquip;}
    bool hasDefenseEquip()const{return m_defenseEquip;}
    bool hasAttackHorse()const{return m_attackHorse;}
    bool hasDefenseHorse()const{return m_defenseHorse;}
    bool hasCard(){return hasHandCard() || hasJudgeCard() ||
                   hasAttackEquip() || hasDefenseEquip() ||
                   hasAttackHorse() || hasDefenseHorse();}

    bool hasHandCard(QCard* card);
    bool hasCard(QCard* card);

//inquiry
    int  handCardCount()const{return m_handCards.size();}
    int  health(){return m_hp;}

public:
    Role    m_role;
    QHero*  m_hero;
    QList<QCard*> m_handCards;
    QCard* m_attackEquip;
    QCard* m_defenseEquip;
    QCard* m_attackHorse;
    QCard* m_defenseHorse;
    QCard* m_judgeShandian;
    QCard* m_judgeLebusishu;
    int m_hp;
    QTcpSocket* m_socket;
};

#endif // QPLAYER_H
