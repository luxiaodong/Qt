#include "qcard.h"

QCard::QCard(int id,int number, int color, int skill, int skill2)
    :m_id(id),
    m_number(number),
    m_color(color),
    m_skill(skill),
    m_skill2(skill2)
{
}

bool QCard::isCard(Skill card) const
{
    Skill skill = (Skill)this->skill();
    return (skill == card);
}

//ÑÕÉ«
bool QCard::isHeitao() const
{
    Color color = (Color)this->color();
    return (color == color_heitao);
}

bool QCard::isHongtao() const
{
    Color color = (Color)this->color();
    return (color == color_hongtao);
}

bool QCard::isCaohua() const
{
    Color color = (Color)this->color();
    return (color == color_caohua);
}

bool QCard::isFangkuai() const
{
    Color color = (Color)this->color();
    return (color == color_fangkuai);
}

//ÅÐ¶¨ÅÆ
bool QCard::isJudgeShandian() const
{
    return (this->isHeitao() && (this->number() >= 2) && (this->number()) <= 9 );
}

bool QCard::isJudgeLebusishu() const
{
    return (this->isHongtao() ? false : true);
}

bool QCard::isJudgeBaguadun() const
{
    if(this->isHongtao() || this->isFangkuai())
    {
        return true;
    }

    return false;
}

bool QCard::isJudgeRenwangdun() const
{
    return false;
}

bool QCard::isJudgeLuoshen() const
{
    return (this->isHeitao() || this->isCaohua());
}

//ÊÇ·ñÊÇ·ÇÑÓ³ÙÀà½õÄÒ
bool QCard::isNonDeferredJingnang() const
{
    Skill skill = (Skill)this->skill();

    if(skill == skill_shunshouqianyang
       || skill == skill_guohechaiqiao
       || skill == skill_wugufendeng
       || skill == skill_wuzhongshenyou
       || skill == skill_juedou
       || skill == skill_nanmanruqin
       || skill == skill_wanjianqifa
       || skill == skill_taoyuanjieyi
       || skill == skill_wuxiekeji
       || skill == skill_jiedaosharen
       )
    {
        return true;
    }

    return false;
}

//¹¥»÷ÎäÆ÷
bool QCard::isAttackEquip() const
{
    Skill skill = (Skill)this->skill();

    if(skill == skill_zhugeliannu
       || skill == skill_cixiongshuanggu
       || skill == skill_qingangjian
       || skill == skill_qinlongyeyuedao
       || skill == skill_zhangbashemao
       || skill == skill_guanshifu
       || skill == skill_fangtianhuaji
       || skill == skill_qilingong
       )
    {
        return true;
    }

    return false;
}

bool QCard::isDefenseEquip() const
{
    return this->isBaguadun();
}

//¹¥»÷Âí·ÀÓùÂí
bool QCard::isAttackHorse() const
{
    return (this->isChitu() || this->isZixin() || this->isDawan());
}

bool QCard::isDefenseHorse() const
{
    return (this->isZhuahuangfeidian() || this->isDilu() || this->isJueying());
}

