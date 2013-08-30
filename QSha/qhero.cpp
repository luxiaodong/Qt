#include "qhero.h"

QHero::QHero(int id,int nation, int sex, int hp, int skill1, int skill2, int skill3)
    :m_id(id),
    m_nation(nation),
    m_sex(sex),
    m_hp(hp),
    m_skill1(skill1),
    m_skill2(skill2),
    m_skill3(skill3)
{
}

bool QHero::isWeiguo() const
{
    Nation nation = (Nation)this->nation();
    return (nation == nation_wei);
}

bool QHero::isShuguo() const
{
    Nation nation = (Nation)this->nation();
    return (nation == nation_shu);
}

bool QHero::isWuguo() const
{
    Nation nation = (Nation)this->nation();
    return (nation == nation_wu);
}

bool QHero::isQun() const
{
    Nation nation = (Nation)this->nation();
    return (nation == nation_qun);
}

bool QHero::isFemale() const
{
    Sex sex = (Sex)this->sex();
    return (sex == sex_female);
}

//触发条件
int QHero::canUseAtCondition(const Skill skill) const
{
    int condition = (int)condition_unknow;

    switch(skill)
    {
    case skill_rende://31
        condition = condition_playcard;
        break;

    case skill_jijiang://32
        condition = condition_playcard | condition_needsha;
        break;

    case skill_wusheng:// = 33;
        condition = condition_playcard | condition_needsha;
        break;

    case skill_paoxiao:// = 34;
        condition = condition_playcard;
        break;

    case skill_guanxin:// = 35;
        condition = condition_beginning;
        break;

    case skill_kongcheng:// = 36;
        condition = condition_nohandcard;
        break;

    case skill_longdan:// = 37;
        condition = condition_playcard | condition_needsha | condition_needshan;
        break;

    case skill_tieqi:// = 38;//铁骑要判定
        condition = condition_playcard;
        break;

    case skill_jizhi:// = 39;
        condition = condition_playcard;
        break;

    case skill_qicai:// = 40;
        condition = condition_lock;
        break;

    case skill_niaoji:// = 41;
        condition = condition_lossequip;
        break;

    case skill_jianxiong:// = 42;
        condition = condition_health;
        break;

    case skill_hujia:// = 43;
        condition = condition_needshan;
        break;

    case skill_tuxi:// = 44;
        condition = condition_touchcard;
        break;

    case skill_luosheng:// = 45;
        condition = condition_beginning;
        break;

    case skill_qinguo:// = 46;
        condition = condition_needshan;
        break;

    case skill_fankui:// = 47;
        condition = condition_health;
        break;

    case skill_guicai:// = 48;
        condition = condition_judge;
        break;

    case skill_ganglie:// = 49;跟铁骑一样
        condition = condition_health;
        break;

    case skill_luoyi:// = 50;
        condition = condition_touchcard;
        break;

    case skill_tianji:// = 51;
        condition = condition_judge;
        break;

    case skill_yiji:// = 52;
        condition = condition_health;
        break;

    case skill_zhiheng:// = 53;
        condition = condition_playcard;
        break;

    case skill_jiuyuan:// = 54;
        condition = condition_needtao;
        break;

    case skill_yinzi:// = 55;
        condition = condition_touchcard;
        break;

    case skill_fanjian:// = 56;
        condition = condition_playcard;
        break;

    case skill_qixi:// = 57;
        condition = condition_playcard;
        break;

    case skill_qianxun:// = 58;
        condition = condition_lock;
        break;

    case skill_lianyin:// = 59;
        condition = condition_nohandcard;
        break;

    case skill_guose:// = 60;
        condition = condition_playcard;
        break;

    case skill_liuli:// = 61;
        condition = condition_needshan;
        break;

    case skill_kurou:// = 62;
        condition = condition_playcard;
        break;

    case skill_keji:// = 63;
        condition = condition_dropcard;
        break;

    case skill_jijiu:// = 64;
        condition = condition_needtao;
        break;

    case skill_qinnang:// = 65;
        condition = condition_playcard;
        break;

    case skill_wushang:// = 66;
        condition = condition_playcard;
        break;

    case skill_biyue:// = 67;
        condition = condition_dropcard;
        break;

    case skill_lijian:// = 68;
        condition = condition_playcard;
        break;

    case skill_jieyin:// = 69;
        condition = condition_playcard;
        break;

    case skill_mashu:// = 70;
        condition = condition_lock;
        break;

    case skill_liegong:// = 71;可能不是锁定技能
        condition = condition_lock;
        break;

    case skill_kuanggu:// = 72;
        condition = condition_health;
        break;

    case skill_shensu:// = 73;
        condition = condition_judge | condition_playcard;
        break;

    case skill_jushou:// = 74;
        condition = condition_ending;
        break;

    case skill_tianxiang: // = 75;
        condition = condition_hurt;
        break;

    case skill_hongyan: // = 76;
        condition = condition_lock;
        break;

    case skill_buqu: // = 77;
        condition = condition_needtao;
        break;

    case skill_leiji:// = 78
        condition = condition_judge;
        break;

    case skill_guidao:// = 79
        condition = condition_judge;
        break;

    case skill_huangtian: // = 80
        condition = condition_playcard;
        break;

    case skill_guhuo: // 81
        condition = condition_playcard;
        break;

    default:
        condition = condition_unknow;
        break;
    }

    return condition;
}

bool QHero::isLockedSkill(const Skill skill) const
{
    return (condition_lock == this->canUseAtCondition(skill));
}

bool QHero::canUseAtBeginning(const Skill skill) const
{
    return (condition_beginning & this->canUseAtCondition(skill));
}

bool QHero::canUseAtJudge(const Skill skill) const
{
    return (condition_judge & this->canUseAtCondition(skill));
}

bool QHero::canUseAtTouchCard(const Skill skill) const
{
    return (condition_touchcard & this->canUseAtCondition(skill));
}

bool QHero::canUseAtPlayCard(const Skill skill) const
{
    return (condition_playcard & this->canUseAtCondition(skill));
}

bool QHero::canUseAtDropCard(const Skill skill) const
{
    return (condition_dropcard & this->canUseAtCondition(skill));
}

bool QHero::canUseAtEnding(const Skill skill) const
{
    return (condition_ending & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenMissHealth(const Skill skill) const
{
    return (condition_health & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenSufferHurt(const Skill skill) const
{
    return (condition_hurt & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenNoHandCard(const Skill skill) const
{
    return (condition_nohandcard & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenLossEquip(const Skill skill) const
{
    return (condition_lossequip & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenNeedSha(const Skill skill) const
{
    return (condition_needsha & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenNeedShan(const Skill skill) const
{
    return (condition_needshan & this->canUseAtCondition(skill));
}

bool QHero::canUseWhenNeedTao(const Skill skill) const
{
    return (condition_needtao & this->canUseAtCondition(skill));
}

//skill
bool QHero::hasSkill(const Skill skill) const
{
    Skill skill1 = (Skill)this->skill1();
    Skill skill2 = (Skill)this->skill2();
    Skill skill3 = (Skill)this->skill3();
    return (skill1 == skill || skill2 == skill || skill3 == skill);
}

bool QHero::isZhugongSkill(const Skill skill) const
{
    if(skill == skill_jijiang
       || skill == skill_hujia
       || skill == skill_jiuyuan
       || skill == skill_huangtian
       )
    {
        return true;
    }

    return false;
}

bool QHero::isOnlyOncePerRound(const Skill skill) const
{
//出牌是只能一次的技能
    if(skill == skill_zhiheng
       || skill == skill_fanjian
       || skill == skill_qinnang
       || skill == skill_lijian
       || skill == skill_jieyin
       )
    {
        return true;
    }

    return false;
}

