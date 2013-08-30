#ifndef GABSTRACTTOWER_H
#define GABSTRACTTOWER_H

class GAbstractTower
{
public:
    GAbstractTower();

    int m_healthPoint;  //血量
    int m_magicPoint;   //法量

    int m_physicalDamagePoint;  //物理攻击
    int m_magicDamagePoint;     //魔法攻击

    int m_physicalDefensePoint; //物理防御
    int m_magicDefensePoint;    //魔法防御
};

#endif // GABSTRACTTOWER_H
