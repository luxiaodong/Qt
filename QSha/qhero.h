#ifndef QHERO_H
#define QHERO_H

#include "qcard.h"

enum Hero{
    hero_none = 0,
    hero_liubei = 1,
    hero_guanyu,// = 2,
    hero_zhangfei,// = 3,
    hero_zhugeliang,// = 4,
    hero_zhaoyun,// = 5,
    hero_machao,// = 6,
    hero_huangyueyin,// = 7,
    hero_sunquan,// = 8,
    hero_ganning,// = 9,
    hero_lvmeng,// = 10,
    hero_huanggai,// = 11,
    hero_zhouyu,// = 12,
    hero_daqiao,// = 13,
    hero_luxun,// = 14,
    hero_caocao,// = 15,
    hero_simayi,// = 16,
    hero_xiahoudun,// = 17,
    hero_zhangliao,// = 18,
    hero_xuchu,// = 19,
    hero_guojia,// = 20,
    hero_zhenji,// = 21,
    hero_huatuo,// = 22,
    hero_lvbu,// = 23,
    hero_diaochan,// = 24,
    hero_sunshangxiang,// = 25,
    hero_huangzhong,// = 26,
    hero_weiyan,// = 27,
    hero_xiahouyuan,// = 28,
    hero_caoren,// = 29,
    hero_xiaoqiao,// = 30,
    hero_zhoutai,// = 31
    hero_zhangjiao,// = 32
    hero_yuji,// = 33
};

enum Nation{
    nation_wei = 0,//Îº
    nation_shu = 1,//Êñ
    nation_wu = 2,//Îâ
    nation_qun = 3//Èº
};

enum Sex{
    sex_male = 0,//ÄÐ
    sex_female = 1,//Å®
};

//condition
enum SkillUseCondition
{
    condition_unknow = 0,
    condition_beginning = 1,
    condition_judge = 1<<1,
    condition_touchcard = 1<<2,
    condition_playcard = 1<<3,
    condition_dropcard = 1<<4 ,
    condition_ending = 1<<5,

    condition_health = 1<<6, //¿ÛÑª
    condition_hurt = 1<<7,   //ÊÜÉËº¦
    condition_nohandcard = 1<<8,
    condition_lossequip = 1<<9,

    condition_needsha = 1<<10,
    condition_needshan = 1<<11,
    condition_needtao = 1<<12,

    condition_lock = 1<<15,
};

//character.dat
class QHero
{
public:
    QHero(int id,int nation, int sex, int hp, int skill1, int skill2, int skill3);
private:
    int id() const{return m_id;}
    int nation() const{return m_nation;}
    int sex() const{return m_sex;}
    int hp() const{return m_hp;}
    int skill1() const{return m_skill1;}
    int skill2() const{return m_skill2;}
    int skill3() const{return m_skill3;}
    bool hasSkill(const Skill skill) const;
    int canUseAtCondition(const Skill skill) const;

public:
    bool isWeiguo() const;
    bool isShuguo() const;
    bool isWuguo() const;
    bool isQun() const;
    bool isFemale() const;

    bool isLockedSkill(const Skill skill) const;//ÊÇ·ñÊÇËø¶¨¼¼ÄÜ
    bool canUseAtBeginning(const Skill skill) const;//»ØºÏ¿ªÊ¼½×¶Î
    bool canUseAtJudge(const Skill skill) const;//ÅÐ¶¨½×¶Î
    bool canUseAtTouchCard(const Skill skill) const;//ÃþÅÆ½×¶Î
    bool canUseAtPlayCard(const Skill skill) const;//³öÅÆ½×¶Î
    bool canUseAtDropCard(const Skill skill) const;//ÆúÅÆ½×¶Î
    bool canUseAtEnding(const Skill skill) const;//»ØºÏ½áÊø½×¶Î
    bool canUseWhenMissHealth(const Skill skill) const;
    bool canUseWhenSufferHurt(const Skill skill) const;
    bool canUseWhenNoHandCard(const Skill skill) const;
    bool canUseWhenLossEquip(const Skill skill) const;
    bool canUseWhenNeedSha(const Skill skill) const;
    bool canUseWhenNeedShan(const Skill skill) const;
    bool canUseWhenNeedTao(const Skill skill) const;

    bool hasRende() const{return this->hasSkill(skill_rende);}
    bool hasJijiang() const{return this->hasSkill(skill_jijiang);}
    bool hasWusheng() const{return this->hasSkill(skill_wusheng);}
    bool hasPaoxiao() const{return this->hasSkill(skill_paoxiao);}
    bool hasGuanxin() const{return this->hasSkill(skill_guanxin);}
    bool hasKongcheng() const{return this->hasSkill(skill_kongcheng);}
    bool hasLongdan() const{return this->hasSkill(skill_longdan);}
    bool hasTieqi() const{return this->hasSkill(skill_tieqi);}
    bool hasJizhi() const{return this->hasSkill(skill_jizhi);}
    bool hasQicai() const{return this->hasSkill(skill_qicai);}
    bool hasNiaoji() const{return this->hasSkill(skill_niaoji);}
    bool hasJianxiong() const{return this->hasSkill(skill_jianxiong);}
    bool hasHujia() const{return this->hasSkill(skill_hujia);}
    bool hasTuxi() const{return this->hasSkill(skill_tuxi);}
    bool hasLuosheng() const{return this->hasSkill(skill_luosheng);}
    bool hasQinguo() const{return this->hasSkill(skill_qinguo);}
    bool hasFankui() const{return this->hasSkill(skill_fankui);}
    bool hasGuicai() const{return this->hasSkill(skill_guicai);}
    bool hasGanglie() const{return this->hasSkill(skill_ganglie);}
    bool hasLuoyi() const{return this->hasSkill(skill_luoyi);}
    bool hasTianji() const{return this->hasSkill(skill_tianji);}
    bool hasYiji() const{return this->hasSkill(skill_yiji);}
    bool hasZhiheng() const{return this->hasSkill(skill_zhiheng);}
    bool hasJiuyuan() const{return this->hasSkill(skill_jiuyuan);}
    bool hasYinzi() const{return this->hasSkill(skill_yinzi);}
    bool hasFanjian() const{return this->hasSkill(skill_fanjian);}
    bool hasQixi() const{return this->hasSkill(skill_qixi);}
    bool hasQianxun() const{return this->hasSkill(skill_qianxun);}
    bool hasLianyin() const{return this->hasSkill(skill_lianyin);}
    bool hasGuose() const{return this->hasSkill(skill_guose);}
    bool hasLiuli() const{return this->hasSkill(skill_liuli);}
    bool hasKurou() const{return this->hasSkill(skill_kurou);}
    bool hasKeji() const{return this->hasSkill(skill_keji);}
    bool hasJijiu() const{return this->hasSkill(skill_jijiu);}
    bool hasQinnang() const{return this->hasSkill(skill_qinnang);}
    bool hasWushang() const{return this->hasSkill(skill_wushang);}
    bool hasBiyue() const{return this->hasSkill(skill_biyue);}
    bool hasLijian() const{return this->hasSkill(skill_lijian);}
    bool hasJieyin() const{return this->hasSkill(skill_jieyin);}
    bool hasMashu() const{return this->hasSkill(skill_mashu);}
    bool hasLiegong() const{return this->hasSkill(skill_liegong);}
    bool hasKuanggu() const{return this->hasSkill(skill_kuanggu);}
    bool hasShensu() const{return this->hasSkill(skill_shensu);}
    bool hasJushou() const{return this->hasSkill(skill_jushou);}
    bool hasTianxiang() const{return this->hasSkill(skill_tianxiang);}
    bool hasHongyan() const{return this->hasSkill(skill_hongyan);}
    bool hasBuqu() const{return this->hasSkill(skill_buqu);}
    bool hasLeiji() const{return this->hasSkill(skill_leiji);}
    bool hasGuidao() const{return this->hasSkill(skill_guidao);}
    bool hasHuangtian() const{return this->hasSkill(skill_huangtian);}
    bool hasGuhuo() const{return this->hasSkill(skill_guhuo);}

    bool isZhugongSkill(const Skill skill) const;
    bool isOnlyOncePerRound(const Skill skill) const;

private:
    int m_id;
    int m_nation;
    int m_sex;
    int m_hp;
    int m_skill1;
    int m_skill2;
    int m_skill3;
};

#endif // QHERO_H
