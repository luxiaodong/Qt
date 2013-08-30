#ifndef GABSTRACTMONSTER_H
#define GABSTRACTMONSTER_H

class GAbstractMonster
{
public:
    GAbstractMonster();

    int m_healthPoint;  //血量
    int m_magicPoint;   //法量

    int m_physicalDamagePoint;  //物理攻击
    int m_magicDamagePoint;     //魔法攻击

    int m_physicalDefensePoint; //物理防御
    int m_magicDefensePoint;    //魔法防御

    int m_speed;    //移动速度
    int m_price;    //死亡后的价格
};

#endif // GABSTRACTMONSTER_H
