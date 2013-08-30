#include "qgame.h"

QGame::QGame(QServer* server)
{
    //创建游戏，需要玩家先建立连接,//以下内容要在 建立 链接后 才可以
    m_server = server;
    m_desk = new QDesk(m_server->m_sockets);
    m_root = new QRoundNode(0,m_desk);
    m_current = 0;
    m_player = 0;
}

void QGame::process(QList<int> &msg)
{
//msg
//0 -- 协议
//1 -- 玩家
//2 -- 技能id, 卡牌id
//后面的自定义

//注意:
//每层QRoundNode的返回值 应该全部有处理, 或者等待, 或者返回上一层
//很容易产生bug的地方
    ProcessState state = ps_none;

    while(state < ps_waiting)
    {
        if(m_current == 0)
        {
            m_current = new QRoundAssignrole(m_desk->playerCount(),m_root,m_desk);
            m_current->m_state = ps_assign_role_start;
        }

        if(m_current->isAssignrole())
        {
            state = m_current->process(msg);
            if(state == ps_assign_role_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundSelectzhugonghero(parent,m_desk);
                m_current->m_state = ps_select_zhugong_hero_start;
            }
        }

        if(m_current->isSelectzhugonghero())
        {
            state = m_current->process(msg);
            if(state == ps_select_zhugong_hero_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundSelectotherheros(parent,m_desk);
                m_current->m_state = ps_select_others_hero_waiting;
            }
        }

        if(m_current->isSelectotherheros())
        {
            state = m_current->process(msg);
            if(state == ps_select_others_hero_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundSendcard(parent,m_desk);
                m_current->m_state = ps_send_every_four_card_start;
            }
        }

        if(m_current->isSendcard())
        {
            state = m_current->process(msg);
            if(state == ps_send_every_four_card_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_player = m_desk->zhugong();
                m_current = new QRoundBeginning(m_player,parent,m_desk);
                m_current->m_state = ps_beginning_start;
            }
        }

//-------------六个阶段-----------------

        if(m_current->isBeginning())
        {
            state = m_current->process(msg);
            if(state == ps_beginning_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundJudge(m_player, parent, m_desk);
                m_current->m_state = ps_judge_start;
            }
        }

        if(m_current->isJudge())
        {
            state = m_current->process(msg);
            if(state == ps_judge_end ||
               state == ps_lebusishu_hit)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundCatchcard(m_player, parent, m_desk);
                m_current->m_state = ps_catchcard_start;

                if(state == ps_lebusishu_hit)
                {
                    m_current->m_state = ps_lebusishu_hit;
                }
            }
            else if(state == ps_lebusishu_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundLebusishu(m_player, parent, m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_shandian_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundShandian(m_player,parent, m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_shandian_hit)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundCalculatedemage(0,m_player,parent,m_desk);
                m_current->m_state = ps_hit_shandian_demage;
            }
            else if(state == ps_dead_ok)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_player = m_desk->nextPlayer(m_player);
                m_current = new QRoundBeginning(m_player, parent, m_desk);
                m_current->m_state = ps_beginning_start;
            }
            else if(state == ps_dead_game_over)
            {}
        }

        if(m_current->isCatchcard())
        {
            state = m_current->process(msg);
            if(state == ps_catchcard_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundPlaycard(m_player, parent, m_desk);
                m_current->m_state = ps_playcard_start;
            }
            else if(state == ps_lebusishu_hit)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundDropcard(m_player, parent, m_desk);
                m_current->m_state = ps_dropcard_start;
            }
        }

        if(m_current->isPlaycard())
        {
            state = m_current->process(msg);
            if(state == ps_playcard_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundDropcard(m_player, parent, m_desk);
                m_current->m_state = ps_dropcard_start;
            }
            else if(state == ps_sha_start)
            {
                QRoundNode* parent = m_current;
                QCard* card = m_desk->cardAt(msg.at(2));
                QPlayer* attack = ((QRoundPlaycard*)m_current)->attack();
                QPlayer* defense = ((QRoundPlaycard*)m_current)->defense();
                m_current = new QRoundSha(attack,defense,card,0,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_sha_zhangbashemao_start)
            {
                QRoundNode* parent = m_current;
                QCard* card = m_desk->cardAt(msg.at(3));
                QCard* other = m_desk->cardAt(msg.at(4));
                QPlayer* attack = ((QRoundPlaycard*)m_current)->attack();
                QPlayer* defense = ((QRoundPlaycard*)m_current)->defense();
                m_current = new QRoundSha(attack,defense,card,other,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_shunshouqianyang_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundPlaycard*)m_current)->attack();
                QPlayer* defense = ((QRoundPlaycard*)m_current)->defense();
                m_current = new QRoundShunshouqianyang(attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_guohechaiqiao_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundPlaycard*)m_current)->attack();
                QPlayer* defense = ((QRoundPlaycard*)m_current)->defense();
                m_current = new QRoundGuohechaiqiao(attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wugufengdeng_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundPlaycard*)m_current)->player();
                m_current = new QRoundWugufengdeng(player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wuzhongshenyou_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuzhongshenyou(m_player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_juedou_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundPlaycard*)m_current)->attack();
                QPlayer* defense = ((QRoundPlaycard*)m_current)->defense();
                m_current = new QRoundJuedou(attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_nanmanruqin_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundPlaycard*)m_current)->player();
                m_current = new QRoundNanmanruqin(player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wanjianqifa_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundPlaycard*)m_current)->player();
                m_current = new QRoundWanjianqifa(player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_taoyuanjieyi_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundPlaycard*)m_current)->player();
                m_current = new QRoundTaoyuanjieyi(player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_jiedaosharen_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundPlaycard*)m_current)->player();
                QPlayer* attack = ((QRoundPlaycard*)m_current)->attack();
                QPlayer* defense = ((QRoundPlaycard*)m_current)->defense();
                m_current = new QRoundJiedaosharen(player,attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_dead_ok)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_player = m_desk->nextPlayer(m_player);
                m_current = new QRoundBeginning(m_player, parent, m_desk);
                m_current->m_state = ps_beginning_start;
            }
            else if(state == ps_dead_game_over)
            {}
        }

        if(m_current->isDropcard())
        {
            state = m_current->process(msg);
            if(state == ps_dropcard_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = new QRoundEnding(m_player, parent, m_desk);
                m_current->m_state = ps_ending_start;
            }
        }

        if(m_current->isEnding())
        {
            state = m_current->process(msg);
            if(state == ps_ending_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_player = m_desk->nextPlayer(m_player);
                m_current = new QRoundBeginning(m_player, parent, m_desk);
                m_current->m_state = ps_beginning_start;
            }
        }

//------------------牌的技能----------------

        if(m_current->isWuxiekeji())
        {
            state = m_current->process(msg);
            if(state == ps_wuxiekeji_loop_waiting)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent, m_desk);
                m_current->m_state = ps_wuxiekeji_start;
            }
            else if(state == ps_wuxiekeji_valid ||
                    state == ps_wuxiekeji_unvalid)
            {
                QRoundNode* parent = m_current->parent();
                parent->m_state = state;
                delete m_current;
                m_current = parent;
            }
        }

        if(m_current->isLebusishu())
        {
            state = m_current->process(msg);
            if(state == ps_lebusishu_waiting)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent, m_desk);
                m_current->m_state = ps_wuxiekeji_start;
            }
            else if(state == ps_lebusishu_hit_waiting ||
                    state == ps_lebusishu_unhit_waiting)
            {
                bool hit = (state == ps_lebusishu_hit_waiting) ? true : false;
                QRoundNode* parent = m_current;
                m_current = new QRoundGuicai(hit,parent,m_desk);
                m_current->m_state = ps_guicai_lebusishu_start;
            }
            else if(state == ps_lebusishu_hit ||
                    state == ps_lebusishu_unhit)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isShandian())
        {
            state = m_current->process(msg);
            if(state == ps_shandian_waiting)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent, m_desk);
                m_current->m_state = ps_wuxiekeji_start;
            }
            else if(state == ps_shandian_hit_waiting ||
                    state == ps_shandian_unhit_waiting)
            {
                bool hit = (state == ps_shandian_hit_waiting) ? true : false;
                QRoundNode* parent = m_current;
                m_current = new QRoundGuicai(hit,parent,m_desk);
                m_current->m_state = ps_guicai_shandian_start;
            }
            else if(state == ps_shandian_hit ||
                    state == ps_shandian_unhit)
            {
                QRoundNode* parent = m_current->parent();
                parent->m_state = state;
                delete m_current;
                m_current = parent;
            }
        }

        if(m_current->isSha())
        {
            state = m_current->process(msg);
            if(state == ps_sha_successful ||
               state == ps_sha_failed ||
               state == ps_sha_can_not)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_shan_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundSha*)m_current)->attack();
                QPlayer* defense = ((QRoundSha*)m_current)->defense();
                m_current = new QRoundShan(attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_cixiongshugu_check_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundSha*)m_current)->attack();
                QPlayer* defense = ((QRoundSha*)m_current)->defense();
                m_current = new QRoundCixiongshugu(attack,defense,parent,m_desk);
                m_current->m_state = ps_cixiongshugu_check_start;
            }
            else if(state == ps_qinlongyeyuedao_check_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* m_player = ((QRoundSha*)m_current)->attack();
                m_current = new QRoundQinlongyeyuedao(m_player,parent,m_desk);
                m_current->m_state = ps_qinlongyeyuedao_check_start;
            }
            else if(state == ps_guanshifu_check_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* m_player = ((QRoundSha*)m_current)->attack();
                m_current = new QRoundGuanshifu(m_player,parent,m_desk);
                m_current->m_state = ps_guanshifu_check_start;
            }
            else if(state == ps_hit_sha_normal_demage)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundSha*)m_current)->attack();
                QPlayer* defense = ((QRoundSha*)m_current)->defense();
                m_current = new QRoundCalculatedemage(attack,defense,parent,m_desk);
                m_current->m_state = ps_hit_sha_normal_demage;
            }
            else if(state == ps_dead_ok ||
                    state == ps_dead_game_over)
            {
               QRoundNode* parent = m_current->parent();
               delete m_current;
               m_current = parent;
               m_current->m_state = state;
            }
        }

        if(m_current->isShan())
        {
            state = m_current->process(msg);
            if(state == ps_shan_successful ||
               state == ps_shan_failed)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isCixiongshugu())
        {
            state = m_current->process(msg);
            if(state == ps_cixiongshugu_drop_card ||
               state == ps_cixiongshugu_catch_card ||
               state == ps_cixiongshugu_check_cancel)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isBaguadun())
        {
            state = m_current->process(msg);
            if(state == ps_baguadun_hit ||
               state == ps_baguadun_unhit)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_baguadun_hit_waiting ||
                    state == ps_baguadun_unhit_waiting)
            {
                bool hit = (state == ps_baguadun_hit_waiting) ? true : false;
                QRoundNode* parent = m_current;
                m_current = new QRoundGuicai(hit,parent,m_desk);
                m_current->m_state = ps_guicai_baguadun_start;
            }
        }

        if(m_current->isQinlongyeyuedao())
        {
            state = m_current->process(msg);
            if(state == ps_qinlongyeyuedao_unvalid ||
               state == ps_qinlongyeyuedao_valid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isGuanshifu())
        {
            state = m_current->process(msg);
            if(state == ps_guanshifu_valid ||
               state == ps_guanshifu_valid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isShunshouqianyang())
        {
            state = m_current->process(msg);
            if(state == ps_shunshouqianyang_valid ||
               state == ps_shunshouqianyang_unvalid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_shunshouqianyang_selectcard_waiting)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundShunshouqianyang*)m_current)->attack();
                QPlayer* defense = ((QRoundShunshouqianyang*)m_current)->defense();
                m_current = new QRoundSelectcard(attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
        }

        if(m_current->isGuohechaiqiao())
        {
            state = m_current->process(msg);
            if(state == ps_guohechaiqiao_valid ||
               state == ps_guohechaiqiao_unvalid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_guohechaiqiao_selectcard_waiting)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundGuohechaiqiao*)m_current)->attack();
                QPlayer* defense = ((QRoundGuohechaiqiao*)m_current)->defense();
                m_current = new QRoundSelectcard(attack,defense,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                //QPlayer* player = ((QRoundWugufengdeng*)m_current)->current();
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
        }

        if(m_current->isWugufengdeng())
        {
            state = m_current->process(msg);
            if(state == ps_wugufengdeng_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wugufengdeng_selectcard_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* attack = ((QRoundWugufengdeng*)m_current)->current();
                m_current = new QRoundSelectcard(attack,0,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
        }

        if(m_current->isWuzhongshenyou())
        {
            state = m_current->process(msg);
            if(state == ps_wuzhongshenyou_valid ||
               state == ps_wuzhongshenyou_unvalid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
        }

        if(m_current->isJuedou())
        {
            state = m_current->process(msg);
            if(state == ps_juedou_valid ||
               state == ps_juedou_unvalid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_juedou_attack_win ||
                    state == ps_juedou_defense_win)
            {
                //构造一个伤害的类
            }
            else if(state == ps_hit_juedou_demage)
            {
            }
            else if(state == ps_dead_ok ||
                    state == ps_dead_game_over)
            {
               QRoundNode* parent = m_current->parent();
               delete m_current;
               m_current = parent;
               m_current->m_state = state;
            }
        }

        if(m_current->isNanmanruqin())
        {
            state = m_current->process(msg);
            if(state == ps_nanmanruqin_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_hit_nanmanruqin_demage)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundNanmanruqin*)m_current)->current();
                m_current = new QRoundCalculatedemage(0,player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_dead_ok ||
                    state == ps_dead_game_over)
            {
               QRoundNode* parent = m_current->parent();
               delete m_current;
               m_current = parent;
               m_current->m_state = state;
            }
        }

        if(m_current->isWanjianqifa())
        {
            state = m_current->process(msg);
            if(state == ps_wanjianqifa_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_hit_wanjianqifa_demage)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundWanjianqifa*)m_current)->current();
                m_current = new QRoundCalculatedemage(0,player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_wanjianqifa_shan_start)
            {
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundWanjianqifa*)m_current)->current();
                m_current = new QRoundShan(0,player,parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_dead_ok ||
                    state == ps_dead_game_over)
            {
               QRoundNode* parent = m_current->parent();
               delete m_current;
               m_current = parent;
               m_current->m_state = state;
            }
        }

        if(m_current->isTaoyuanjieyi())
        {
            state = m_current->process(msg);
            if(state == ps_taoyuanjieyi_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                //QPlayer* player = ((QRoundWugufengdeng*)m_current)->current();
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
        }

        if(m_current->isJiedaosharen())
        {
            state = m_current->process(msg);
            if(state == ps_jiedaosharen_valid ||
               state == ps_jiedaosharen_unvalid)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_wuxiekeji_start)
            {
                QRoundNode* parent = m_current;
                //QPlayer* player = ((QRoundWugufengdeng*)m_current)->current();
                m_current = new QRoundWuxiekeji(parent,m_desk);
                m_current->m_state = state;
            }
            else if(state == ps_jiedaosharen_select_start)
            {}
            else if(state == ps_dead_ok ||
                    state == ps_dead_game_over)
            {
               QRoundNode* parent = m_current->parent();
               delete m_current;
               m_current = parent;
               m_current->m_state = state;
            }
        }

//------------------other------------------

        if(m_current->isCalculatedemage())
        {
            state = m_current->process(msg);
            if(state == ps_hit_shanghai_still_live ||
               state == ps_dead_ok ||
               state == ps_dead_game_over)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
            else if(state == ps_hit_shanghai_asktao)
            {
                //构造求桃的类
                QRoundNode* parent = m_current;
                QPlayer* player = ((QRoundCalculatedemage*)m_current)->defense();
                m_current = new QRoundAsktaolist(player,parent,m_desk);
                m_current->m_state = ps_asktao_list_start;
            }
            else if(state == ps_hit_shanghai_dead)
            {
                //构造一个死亡的类
                QRoundNode* parent = m_current;
                m_current = new QRoundDead(m_player,parent,m_desk);
                m_current->m_state = ps_dead_start;
            }
        }

        if(m_current->isAsktaolist())
        {
            state = m_current->process(msg);
            if(state == ps_asktao_list_ok ||
               state == ps_asktao_list_failed)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isAsktao())
        {
            state = m_current->process(msg);
            if(state == ps_asktao_ok || state == ps_asktao_failed)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isDead())
        {
            state = m_current->process(msg);
            if(state == ps_dead_ok || state == ps_dead_game_over)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isSelectcard())
        {
            state = m_current->process(msg);
            if(state == ps_wugufengdeng_selectcard_end ||
               state == ps_shunshouqianyang_selectcard_end ||
               state == ps_guohechaiqiao_selectcard_end)
            {
                QRoundNode* parent = m_current->parent();
                delete m_current;
                m_current = parent;
                m_current->m_state = state;
            }
        }

        if(m_current->isSelectjiedaosharen())
        {
            state = m_current->process(msg);
            if(state == ps_jiedaosharen_select_normal_sha)
            {
            }
        }

//------------------武将技能----------------
    }
}

