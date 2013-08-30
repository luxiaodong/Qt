#ifndef QROUND_H
#define QROUND_H

#include "qdesk.h"
#include "../protocol.h"

enum ProcessState
{
    ps_none = 0,
    ps_assign_role_start,
    ps_assign_role_end,

    ps_select_zhugong_hero_start,
    ps_select_zhugong_hero_end,

    ps_select_others_hero_start,
    ps_select_others_hero_end,

    ps_send_every_four_card_start,
    ps_send_every_four_card_end,

    ps_beginning_start,
    ps_beginning_end,

    ps_judge_start,
    ps_judge_end,

    ps_catchcard_start,
    ps_catchcard_end,

    ps_playcard_start,
    ps_playcard_end,

    ps_dropcard_start,
    ps_dropcard_end,

    ps_ending_start,
    ps_ending_end,

//牌的技能
    ps_wuxiekeji_start,
    ps_wuxiekeji_valid,
    ps_wuxiekeji_unvalid,

    ps_lebusishu_start,
    ps_lebusishu_hit,
    ps_lebusishu_unhit,

    ps_shandian_start,
    ps_shandian_hit,
    ps_shandian_unhit,

    ps_sha_start,
    ps_sha_zhangbashemao_start,
    ps_sha_can_not,
    ps_sha_failed,
    ps_sha_successful,

    ps_shan_start,
    ps_shan_successful,
    ps_shan_failed,

    ps_cixiongshugu_check_start,
    ps_cixiongshugu_check_cancel,
    ps_cixiongshugu_drop_card,
    ps_cixiongshugu_catch_card,

    ps_baguadun_check_start,
    ps_baguadun_hit,
    ps_baguadun_unhit,

    ps_qinlongyeyuedao_check_start,
    ps_qinlongyeyuedao_valid,
    ps_qinlongyeyuedao_unvalid,

    ps_guanshifu_check_start,
    ps_guanshifu_valid,
    ps_guanshifu_unvalid,

    ps_shunshouqianyang_start,
    ps_shunshouqianyang_valid,
    ps_shunshouqianyang_selectcard_start,
    ps_shunshouqianyang_selectcard_end,
    ps_shunshouqianyang_unvalid,

    ps_guohechaiqiao_start,
    ps_guohechaiqiao_valid,
    ps_guohechaiqiao_selectcard_start,
    ps_guohechaiqiao_selectcard_end,
    ps_guohechaiqiao_unvalid,

    ps_wugufengdeng_start,
    ps_wugufengdeng_selectcard_start,
    ps_wugufengdeng_selectcard_end,
    ps_wugufengdeng_end,

    ps_wuzhongshenyou_start,
    ps_wuzhongshenyou_valid,
    ps_wuzhongshenyou_unvalid,

    ps_juedou_start,
    ps_juedou_attack_win,
    ps_juedou_defense_win,
    ps_juedou_unvalid,
    ps_juedou_valid,

    ps_nanmanruqin_start,
    ps_nanmanruqin_sha_start,
    ps_nanmanruqin_sha_end,
    ps_nanmanruqin_end,

    ps_wanjianqifa_start,
    ps_wanjianqifa_shan_start,
    ps_wanjianqifa_shan_end,
    ps_wanjianqifa_end,

    ps_taoyuanjieyi_start,
    ps_taoyuanjieyi_end,

    ps_jiedaosharen_start,
    ps_jiedaosharen_valid,
    ps_jiedaosharen_unvalid,
    ps_jiedaosharen_select_start,
    ps_jiedaosharen_select_normal_sha,
    ps_jiedaosharen_select_drop,

//other
    ps_judge_shandian_hit,
    ps_judge_lebusishu_hit,

    ps_hit_shandian_demage,
    ps_hit_shanghai_still_live,
    ps_hit_shanghai_dead,
    ps_hit_shanghai_move,
    ps_hit_shanghai_asktao,

    ps_hit_sha_normal_demage,
    ps_hit_juedou_demage,
    ps_hit_nanmanruqin_demage,
    ps_hit_wanjianqifa_demage,

    ps_asktao_list_start,
    ps_asktao_start,
    ps_asktao_ok,
    ps_asktao_failed,
    ps_asktao_list_ok,
    ps_asktao_list_failed,

    ps_dead_start,
    ps_dead_ok,
    ps_dead_game_over,

//武将技能
    ps_guicai_lebusishu_start,
    ps_guicai_shandian_start,
    ps_guicai_baguadun_start,
    ps_guicai_hit,
    ps_guicai_unhit,

    ps_fanjian_start,
    ps_fanjian_waiting,
    ps_fanjian_guess_hit,
    ps_fanjian_guess_unhit,

//--------------各种等待消息------------
    ps_waiting,
    ps_select_zhugong_hero_waiting,
    ps_select_others_hero_waiting,
    ps_beginning_waiting,
    ps_playcard_waiting,

//牌的技能
    ps_sha_normal_waiting,
    ps_shan_waiting,
    ps_wugufengdeng_selectcard_waiting,
    ps_juedou_waiting,
    ps_shunshouqianyang_selectcard_waiting,
    ps_guohechaiqiao_selectcard_waiting,
    ps_nanmanruqin_sha_waiting,
    ps_wanjianqifa_shan_waiting,
    ps_jiedaosharen_select_waiting,

    ps_cixiongshugu_check_fire_waiting,
    ps_cixiongshugu_check_select_waiting,
    ps_qinlongyeyuedao_check_waiting,
    ps_guanshifu_check_waiting,

    ps_lebusishu_waiting,
    ps_lebusishu_hit_waiting,
    ps_lebusishu_unhit_waiting,

    ps_shandian_waiting,
    ps_shandian_hit_waiting,
    ps_shandian_unhit_waiting,

    ps_baguadun_check_waiting,
    ps_baguadun_hit_waiting,
    ps_baguadun_unhit_waiting,

    ps_wuxiekeji_waiting,
    ps_wuxiekeji_loop_waiting,

    ps_asktao_waiting,

//武将技能
    ps_guicai_waiting,
};

class QRoundNode
{
public:
    QRoundNode(QRoundNode* parent = 0,QDesk *desk = 0);
    QRoundNode* parent()const{return m_parent;}
    QDesk* desk()const{return m_desk;}
    void setState(ProcessState state);

//check
    virtual bool isAssignrole()const{return false;}
    virtual bool isSelectzhugonghero()const{return false;}
    virtual bool isSelectotherheros()const{return false;}

    virtual bool isSendcard() const{return false;}

    virtual bool isBeginning()const{return false;}
    virtual bool isJudge()const{return false;}
    virtual bool isCatchcard()const{return false;}
    virtual bool isPlaycard()const{return false;}
    virtual bool isDropcard()const{return false;}
    virtual bool isEnding()const{return false;}

    virtual bool isSha()const{return false;}
    virtual bool isShan()const{return false;}
    virtual bool isShunshouqianyang()const{return false;}
    virtual bool isGuohechaiqiao()const{return false;}
    virtual bool isWugufengdeng()const{return false;}
    virtual bool isWuzhongshenyou()const{return false;}
    virtual bool isJuedou()const{return false;}
    virtual bool isNanmanruqin()const{return false;}
    virtual bool isWanjianqifa()const{return false;}
    virtual bool isTaoyuanjieyi()const{return false;}
    virtual bool isWuxiekeji()const{return false;}
    virtual bool isJiedaosharen()const{return false;}
    virtual bool isLebusishu()const{return false;}
    virtual bool isShandian()const{return false;}
    virtual bool isCixiongshugu()const{return false;}
    virtual bool isBaguadun() const {return false;}
    virtual bool isQinlongyeyuedao()const{return false;}
    virtual bool isGuanshifu()const{return false;}

    virtual bool isCalculatedemage()const{return false;}
    virtual bool isAsktaolist()const{return false;}
    virtual bool isAsktao()const{return false;}
    virtual bool isDead()const{return false;}
    virtual bool isSelectcard()const{return false;}
    virtual bool isSelectjiedaosharen()const{return false;}

    virtual bool isGuicai()const{return false;}
    virtual bool isLuoshen() const {return false;}
    virtual bool isGuanxin() const {return false;}
    virtual bool isShensu() const {return false;}

    virtual ProcessState process(QList<int> msg){return ps_waiting;}
public:
    ProcessState m_state;
private:
    QRoundNode* m_parent;
    QDesk *m_desk;
};

//发身份
class QRoundAssignrole : public QRoundNode
{
public:
    QRoundAssignrole(int playerNumber,QRoundNode* parent,QDesk* desk)
        :QRoundNode(parent,desk){}
    ProcessState process(QList<int> &msg);
    bool isAssignrole() const{return true;}
private:
    int m_playerNumber;
};

//主公选择武将
class QRoundSelectzhugonghero : public QRoundNode
{
public:
    QRoundSelectzhugonghero(QRoundNode* parent,QDesk *desk)
            :QRoundNode(parent,desk){}
    ProcessState process(QList<int> &msg);
    bool isSelectzhugonghero() const{return true;}
};

//其他人选择武将
class QRoundSelectotherheros : public QRoundNode
{
public:
    QRoundSelectotherheros(QRoundNode* parent,QDesk *desk)
            :QRoundNode(parent,desk){}
    ProcessState process(QList<int> &msg);
    bool isSelectotherheros() const{return true;}
};

//送牌阶段
class QRoundSendcard : public QRoundNode
{
public:
    QRoundSendcard(QRoundNode* parent,QDesk *desk)
            :QRoundNode(parent,desk){}
    ProcessState process(QList<int> &msg);
    bool isSendcard() const{return true;}
};

//回合开始阶段
class QRoundBeginning : public QRoundNode
{
public:
    QRoundBeginning(QPlayer* player, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    ProcessState process(QList<int> &msg);
    bool isBeginning()const{return true;}
private:
    QPlayer* m_player;
};

//判定阶段
class QRoundJudge : public QRoundNode
{
public:
    QRoundJudge(QPlayer* player, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    ProcessState process(QList<int> &msg);
    bool isJudge()const{return true;}
private:
    QPlayer* m_player;
};

//摸牌阶段
class QRoundCatchcard : public QRoundNode
{
public:
    QRoundCatchcard(QPlayer* player, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    ProcessState process(QList<int> &msg);
    bool isCatchcard()const{return true;}
private:
    QPlayer* m_player;
};

//出牌阶段
class QRoundPlaycard : public QRoundNode
{
public:
    QRoundPlaycard(QPlayer* player, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_player(player),m_attack(0),m_defense(0),m_isFirstSha(true){}
    QPlayer* player(){return m_player;}
    QPlayer* attack(){return m_attack;}
    QPlayer* defense(){return m_defense;}
    ProcessState process(QList<int> &msg);
    bool isPlaycard()const{return true;}
private:
    QPlayer* m_player;
    QPlayer* m_attack;
    QPlayer* m_defense;
    bool m_isFirstSha;
};

//弃牌阶段
class QRoundDropcard : public QRoundNode
{
public:
    QRoundDropcard(QPlayer* player, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    ProcessState process(QList<int> &msg);
    bool isDropcard()const{return true;}
private:
    QPlayer* m_player;
};

//结束阶段
class QRoundEnding : public QRoundNode
{
public:
    QRoundEnding(QPlayer* player, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    ProcessState process(QList<int> &msg);
    bool isEnding()const{return true;}
private:
    QPlayer* m_player;
};

//---------------------牌的技能-----------------
//无懈可击
class QRoundWuxiekeji : public QRoundNode
{
public:
    QRoundWuxiekeji(QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk){}
    bool isWuxiekeji() const {return true;}
    ProcessState process(QList<int> msg);
};

//乐不思蜀
class QRoundLebusishu : public QRoundNode
{
public:
    QRoundLebusishu(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    bool isLebusishu() const {return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_player;
};

//闪电
class QRoundShandian : public QRoundNode
{
public:
    QRoundShandian(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    bool isShandian() const {return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_player;
};

//杀,这个类可能也要拆开来
class QRoundSha : public QRoundNode
{
public:
    QRoundSha(QPlayer* attack, QPlayer* defense, QCard* card, QCard* other, QRoundNode* parent, QDesk *desk)
        :QRoundNode(parent,desk),m_attack(attack),m_defense(defense),m_card(card),m_other(other){}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    virtual bool isSha()const{return true;}
    ProcessState process(QList<int> &msg);
    bool canAttack();
    bool canCixiongshugu();
    bool canBaguadun();
    bool canQinlongyeyuedao();
    bool canGuanshifu();
    bool canHanbingjian(){return false;}
    bool canBaiyinshizi(){return false;}
    bool canTengjia(){return false;}
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
    QCard* m_card;
    QCard* m_other; //只为仗八蛇矛
};

class QRoundShan : public QRoundNode
{
public:
    QRoundShan(QPlayer* attack, QPlayer* defense, QRoundNode* parent, QDesk* desk)
        :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    virtual bool isShan()const{return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
};

//雌雄双股
class QRoundCixiongshugu : public QRoundNode
{
public:
    QRoundCixiongshugu(QPlayer* attack, QPlayer* defense, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    bool isCixiongshugu() const {return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
};

//八卦盾
class QRoundBaguadun : public QRoundNode
{
public:
    QRoundBaguadun(QPlayer* player, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    bool isBaguadun() const {return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_player;
};

//青龙偃月刀
class QRoundQinlongyeyuedao : public QRoundNode
{
public:
    QRoundQinlongyeyuedao(QPlayer* player, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    bool isQinlongyeyuedao() const {return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_player;
};

//贯石斧
class QRoundGuanshifu : public QRoundNode
{
public:
    QRoundGuanshifu(QPlayer* player, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    bool isGuanshifu() const {return true;}
    ProcessState process(QList<int> msg);
private:
    QPlayer* m_player;
};

//顺手牵羊
class QRoundShunshouqianyang : public QRoundNode
{
public:
    QRoundShunshouqianyang(QPlayer* attack, QPlayer* defense, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    virtual bool isShunshouqianyang()const{return true;}
    ProcessState process(QList<int> msg);
    bool canShunshouqianyang();
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
};

//过河拆桥
class QRoundGuohechaiqiao : public QRoundNode
{
public:
    QRoundGuohechaiqiao(QPlayer* attack, QPlayer* defense, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    virtual bool isGuohechaiqiao()const{return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
};

//五谷丰登
class QRoundWugufengdeng: public QRoundNode
{
public:
    QRoundWugufengdeng(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player),m_index(0){}
    QPlayer* player(){return m_player;}
    bool isWugufengdeng() const {return true;}
    ProcessState process(QList<int> &msg);
    QPlayer* current();
private:
    QPlayer* m_player;
    int m_index;
};

//无中生有
class QRoundWuzhongshenyou: public QRoundNode
{
public:
    QRoundWuzhongshenyou(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    bool isWuzhongshenyou() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};

//决斗,出决斗牌的为attack
class QRoundJuedou: public QRoundNode
{
public:
    QRoundJuedou(QPlayer* attack, QPlayer* defense, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    virtual bool isJuedou()const{return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
    bool m_attack_win;
};

//南蛮入侵
class QRoundNanmanruqin : public QRoundNode
{
public:
    QRoundNanmanruqin(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player),m_index(0){}
    QPlayer* player()const{return m_player;}
    bool isNanmanruqin() const {return true;}
    ProcessState process(QList<int> &msg);
    QPlayer* current();
private:
    QPlayer* m_player;
    int m_index;
};

//万箭齐发
class QRoundWanjianqifa : public QRoundNode
{
public:
    QRoundWanjianqifa(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    bool isWanjianqifa() const {return true;}
    ProcessState process(QList<int> &msg);
    QPlayer* current();
private:
    QPlayer* m_player;
    int m_index;
};

//桃园结义
class QRoundTaoyuanjieyi : public QRoundNode
{
public:
    QRoundTaoyuanjieyi(QPlayer* player,QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player),m_index(0){}
    QPlayer* player()const{return m_player;}
    bool isTaoyuanjieyi() const {return true;}
    ProcessState process(QList<int> &msg);
    QPlayer* current();
private:
    QPlayer* m_player;
    int m_index;
};

//借刀杀人
class QRoundJiedaosharen : public QRoundNode
{
public:
    QRoundJiedaosharen(QPlayer* player, QPlayer* attack, QPlayer* defense, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player),m_attack(attack),m_defense(defense){}
    QPlayer* player()const{return m_player;}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    bool isJiedaosharen()const{return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
    QPlayer* m_attack;
    QPlayer* m_defense;
};

//-----------------------other----------------
//伤害结算
class QRoundCalculatedemage : public QRoundNode
{
public:
    QRoundCalculatedemage(QPlayer* attack,QPlayer*defense,QRoundNode* parent = 0, QDesk *desk = 0)
            :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    bool isCalculatedemage() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
};

//求桃列表
class QRoundAsktaolist : public QRoundNode
{
public:
    QRoundAsktaolist(QPlayer* player,QRoundNode* parent = 0, QDesk *desk = 0)
            :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    bool isAsktaolist() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};

//求桃
class QRoundAsktao : public QRoundNode
{
public:
    QRoundAsktao(QPlayer* player,QRoundNode* parent = 0, QDesk *desk = 0)
            :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    bool isAsktao() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};

//死亡
class QRoundDead : public QRoundNode
{
public:
    QRoundDead(QPlayer* player,QRoundNode* parent = 0, QDesk *desk = 0)
            :QRoundNode(parent,desk),m_player(player){}
    QPlayer* player()const{return m_player;}
    bool isDead() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};

//选择卡牌
class QRoundSelectcard : public QRoundNode
{
public:
    QRoundSelectcard(QPlayer* attack,QPlayer*defense,QRoundNode* parent = 0, QDesk *desk = 0)
            :QRoundNode(parent,desk),m_attack(attack),m_defense(defense){}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    bool isSelectcard() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_attack;
    QPlayer* m_defense;
};

class QRoundSelectjiedaosharen : public QRoundNode
{
public:
    QRoundSelectjiedaosharen(QPlayer* player, QPlayer* attack, QPlayer* defense, QRoundNode* parent,QDesk *desk)
        :QRoundNode(parent,desk),m_player(player),m_attack(attack),m_defense(defense){}
    QPlayer* player()const{return m_player;}
    QPlayer* attack()const{return m_attack;}
    QPlayer* defense()const{return m_defense;}
    bool isSelectjiedaosharen()const{return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
    QPlayer* m_attack;
    QPlayer* m_defense;
};











//-------------un dowith class--------------
class QRoundGuicai : public QRoundNode
{
    enum{
        guicai_none = 0,
        guicai_lebusishu,
        guicai_shandian,
        guicai_baguadun,
    };
public:
    QRoundGuicai(bool hit,QRoundNode* parent = 0, QDesk *desk = 0)
        :QRoundNode(parent,desk),m_hit(hit),m_type(guicai_none){}
    bool isGuicai()const{return true;}
    ProcessState process(QList<int> &msg);
    bool m_hit;
    int m_type;
};









//-----------------------武将技能-----------
//洛神
class QRoundLuoshen : public QRoundNode
{
public:
    QRoundLuoshen(QPlayer* player,QRoundNode* parent = 0, QDesk *desk = 0);
    QPlayer* player()const{return m_player;}
    bool isLuoshen() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};

//观星
class QRoundGuanxin : public QRoundNode
{
public:
    QRoundGuanxin(QPlayer* player,QRoundNode* parent = 0, QDesk *desk = 0);
    QPlayer* player()const{return m_player;}
    bool isGuanxin() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};

//神速
class QRoundShensu : public QRoundNode
{
public:
    QRoundShensu(QPlayer* player,QRoundNode* parent = 0, QDesk *desk = 0);
    QPlayer* player()const{return m_player;}
    bool isShensu() const {return true;}
    ProcessState process(QList<int> &msg);
private:
    QPlayer* m_player;
};



#endif // QROUND_H
