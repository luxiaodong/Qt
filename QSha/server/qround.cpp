#include "qround.h"

QRoundNode::QRoundNode(QRoundNode* parent,QDesk *desk)
    :m_parent(parent),m_desk(desk)
{}

//�����
ProcessState QRoundAssignrole::process(QList<int> &msg)
{
    if(m_state != ps_assign_role_start)
    {
        return ps_waiting;
    }

    (void)msg;
    int role_array[10] =
    {
        role_zhugong,role_fanzei,role_neijian,
        role_zhongchen,role_fanzei,role_fanzei,
        role_zhongchen,role_fanzei,role_neijian,
        role_zhongchen
    };

    QList<int> roles;
    for(int i = 0; i < m_playerNumber; ++i)
    {
        roles.append(role_array[i]);
    }

    for(int i = 0; i < m_playerNumber; ++i)
    {
        int index = qrand()%(m_playerNumber);
        QPlayer* player = desk()->playerAt(i);
        player->setRole((Role)roles.at(index));
        roles.removeAt(index);
    }

    //s2c_send_role
    return ps_assign_role_end;
}

//����ѡ���佫
ProcessState QRoundSelectzhugonghero::process(QList<int> &msg)
{
    if(m_state == ps_select_zhugong_hero_start)
    {
        //s2c_send_hero_to_zhugong_select
        m_state = ps_select_zhugong_hero_waiting;
    }
    else if(m_state == ps_select_zhugong_hero_waiting)
    {
        //�ж��Ƿ����յ�����ѡ���佫����Ϣ
        //����������
        //s2c_send_zhugong_hero_to_all
        m_state = ps_select_zhugong_hero_end;
    }

    return m_state;
}

//������ѡ���佫
ProcessState QRoundSelectotherheros::process(QList<int> &msg)
{
    if(m_state == ps_select_others_hero_start)
    {
        //s2c_send_hero_to_others_select
        m_state = ps_select_others_hero_waiting;
    }
    else if(m_state == ps_select_others_hero_waiting)
    {
        //s2c_send_hero_to_others
        //�ж��Ƿ�������Ҷ��Ѿ�ѡ��
        m_state = ps_select_others_hero_end;
    }

    return m_state;
}

//���ƽ׶�
ProcessState QRoundSendcard::process(QList<int> &msg)
{
    if(m_state == ps_send_every_four_card_start)
    {
        //s2c_send_every_four_card
        m_state = ps_send_every_four_card_end;
    }

    return m_state;
}

//�غϿ�ʼ�׶�
ProcessState QRoundBeginning::process(QList<int> &msg)
{
    if(m_state == ps_beginning_start)
    {
        m_state = ps_beginning_end;
    }

//    QHero* hero = m_player->m_hero;
//    if(hero->hasLuosheng() || hero->hasGuanxin() || hero->hasShensu() )
//    {
//        //s2c_ask_use_skill
//        return p_wait;
//    }

    return m_state;
}

//�ж��׶�
ProcessState QRoundJudge::process(QList<int> &msg)
{
    if(m_state == ps_judge_start)
    {
        if(m_player->hasJudgeCard())
        {
            //û�п����Ⱥ�˳�򣬼ٶ�������
            if(m_player->m_judgeShandian)
            {
                m_state = ps_shandian_start;
            }
            else if(m_player->m_judgeLebusishu)
            {
                m_state = ps_lebusishu_start;
            }
        }
        else
        {
            m_state = ps_judge_end;
        }
    }
    else if(m_state == ps_shandian_hit ||
            m_state == ps_shandian_unhit)
    {
        //ȥ�� �ж���
        m_player->m_judgeShandian = 0;
        if(m_state == ps_shandian_unhit)
        {
            m_state = ps_judge_start;
        }
    }
    else if(m_state == ps_lebusishu_hit ||
            m_state == ps_lebusishu_unhit)
    {
        //ȥ�� �ж���
        m_player->m_judgeLebusishu = 0;
        if(m_state == ps_lebusishu_unhit)
        {
            m_state = ps_judge_start;
        }
    }
    else if(m_state == ps_hit_shanghai_still_live)
    {
        m_state = ps_judge_start;
    }

    return m_state;
}

//���ƽ׶�
ProcessState QRoundCatchcard::process(QList<int> &msg)
{
    if(m_state == ps_catchcard_start || m_state == ps_lebusishu_hit)  //�������佫���� ͻϮ ���� Ӣ��
    {
        //����
        if(m_state == ps_catchcard_start)
        {
            m_state = ps_catchcard_end;
        }
    }

    return m_state;
}

//���ƽ׶�
ProcessState QRoundPlaycard::process(QList<int> &msg)
{
    //ȱ�� ������ �������Ĵ���
    //ȱ�� ������ �Ƿ���������������
    if(m_state == ps_playcard_start)
    {
        m_state = ps_playcard_waiting;
    }
    else if(m_state == ps_playcard_waiting)
    {
        Q_ASSERT(m_player == desk()->playerAt(msg.at(1)));

        QHero* hero = m_player->m_hero;
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_playcard_end;
        }
        else if(protocol == c2s_request_use_skill)
        {
            int skillId = msg.at(2);
            if(skillId == skill_rende && hero->hasRende())
            {
                m_state = ps_playcard_waiting;
            }
            else if(skillId == skill_zhiheng && hero->hasZhiheng())
            {
                m_state = ps_playcard_waiting;
            }
            else if(skillId == skill_qinnang && hero->hasQinnang())
            {
                m_state = ps_playcard_waiting;
            }
            else if(skillId == skill_kurou && hero->hasKurou())
            {
                //�ж�����û��
                m_state = ps_playcard_waiting;
            }
            else if(skillId == skill_guose && hero->hasGuose())
            {
                m_state = ps_playcard_waiting;
            }
            else if(skillId == skill_lijian && hero->hasLijian())
            {}
            else if(skillId == skill_qixi && hero->hasQixi())
            {}
            else if(skillId == skill_jieyin && hero->hasJieyin())
            {}
            else if(skillId == skill_fanjian && hero->hasFanjian())
            {
                m_state = ps_fanjian_start;
            }
            else if(skillId == skill_paoxiao && hero->hasPaoxiao())
            {}
            else if(skillId == skill_wusheng && hero->hasWusheng())
            {}
            else if(skillId == skill_longdan && hero->hasLongdan())
            {}
        }
        else if(protocol == c2s_request_use_card)
        {
            QCard* card = desk()->cardAt(msg.at(2));

            if(card->isSha() || card->isZhangbashemao())
            {
                bool canSha = m_isFirstSha;

                if(m_isFirstSha == false)
                {
                    //���������������ŷ��߼��ܣ��������
                    QCard* card = m_player->m_attackEquip;
                    if(card)
                    {
                        if(card->isZhugeliannu())
                        {
                            canSha = true;
                        }
                    }
                }
//��Ӧ�������ﴦ��
                if(canSha == true)
                {
                    if(card->isZhangbashemao())
                    {
                        QCard* first = desk()->cardAt(msg.at(3));
                        QCard* second = desk()->cardAt(msg.at(4));

                        if(first && second)
                        {
                            QPlayer* targetPlayer = desk()->playerAt(msg.at(5));
                            if(targetPlayer && targetPlayer->isDead() == false)
                            {
                                m_attack = m_player;
                                m_defense = targetPlayer;
                                m_state = ps_sha_zhangbashemao_start;
                            }
                        }
                    }
                    else
                    {
                        QPlayer* targetPlayer = desk()->playerAt(msg.at(3));
                        if(targetPlayer && targetPlayer->isDead() == false)
                        {
                            m_attack = m_player;
                            m_defense = targetPlayer;
                            m_state = ps_sha_start;
                        }
                    }
                }

                m_state = ps_playcard_waiting;
            }
            else if(card->isTao())
            {
                m_state = ps_playcard_waiting;
            }
            else if(card->isShunshouqianyang())
            {
                QPlayer* targetPlayer = desk()->playerAt(msg.at(3));
                if(targetPlayer)
                {
                    m_attack = m_player;
                    m_defense = targetPlayer;

                    QRoundShunshouqianyang test(m_attack,m_defense,0,desk());
                    if( test.canShunshouqianyang() )
                    {
                        desk()->dropCard(m_attack,card);
                        m_state = ps_shunshouqianyang_start;
                    }
                }
            }
            else if(card->isGuohechaiqiao())
            {
                QPlayer* targetPlayer = desk()->playerAt(msg.at(3));
                if(targetPlayer)
                {
                    m_attack = m_player;
                    m_defense = targetPlayer;
                    desk()->dropCard(m_attack,card);
                    m_state = ps_guohechaiqiao_start;
                }
            }
            else if(card->isWugufendeng())
            {
                m_state = ps_wugufengdeng_start;
            }
            else if(card->isWuzhongshenyou())
            {
                m_state = ps_wuzhongshenyou_start;
            }
            else if(card->isJuedou())
            {
                m_state = ps_juedou_start;
            }
            else if(card->isNanmanruqin())
            {
                m_state = ps_nanmanruqin_start;
            }
            else if(card->isWanjianqifa())
            {
                m_state = ps_wanjianqifa_start;
            }
            else if(card->isShandian())
            {
            }
            else if(card->isTaoyuanjieyi())
            {
                m_state = ps_taoyuanjieyi_start;
            }
            else if(card->isJiedaosharen())
            {
                m_state = ps_jiedaosharen_start;
            }
            else if(card->isLebusishu())
            {
            }
            else if(card->isZhugeliannu()
                    || card->isCixiongshuanggu()
                    || card->isQingangjian()
                    || card->isQinlongyeyuedao()
                    || card->isZhangbashemao()
                    || card->isGuanshifu()
                    || card->isFangtianhuaji()
                    || card->isQilingong())
            {
                QCard* a_e = m_player->m_attackEquip;
                if(a_e){}
                m_player->m_attackHorse = card;
                m_state = ps_playcard_waiting;
            }
            else if(card->isBaguadun())
            {
                QCard* d_e = m_player->m_defenseEquip;
                if(d_e){}
                m_player->m_attackHorse = card;
                m_state = ps_playcard_waiting;
            }
            else if(card->isChitu()
                    || card->isZixin()
                    || card->isDawan())
            {
                QCard* a_h = m_player->m_attackHorse;
                if(a_h){}
                m_player->m_attackHorse = card;
                m_state = ps_playcard_waiting;
            }
            else if(card->isJueying()
                    || card->isDilu()
                    || card->isZhuahuangfeidian())
            {
                QCard* d_h = m_player->m_defenseHorse;
                if(d_h){}
                m_player->m_defenseHorse = card;
                m_state = ps_playcard_waiting;
            }
        }
    }
    else if(m_state == ps_sha_failed ||
            m_state == ps_sha_can_not)
    {
        m_isFirstSha = false;
        m_state = ps_playcard_waiting;
    }
    else if(m_state == ps_shunshouqianyang_valid ||
            m_state == ps_shunshouqianyang_unvalid ||
            m_state == ps_guohechaiqiao_valid ||
            m_state == ps_guohechaiqiao_unvalid ||
            m_state == ps_wugufengdeng_end ||
            m_state == ps_nanmanruqin_end ||
            m_state == ps_wanjianqifa_end ||
            m_state == ps_taoyuanjieyi_end ||
            m_state == ps_wuzhongshenyou_valid ||
            m_state == ps_wuzhongshenyou_unvalid ||
            m_state == ps_jiedaosharen_valid ||
            m_state == ps_jiedaosharen_unvalid ||
            m_state == ps_juedou_unvalid ||
            m_state == ps_juedou_attack_win ||
            m_state == ps_juedou_defense_win
            )
    {
        m_state = ps_playcard_waiting;
    }

    return m_state;
}

//���ƽ׶�
ProcessState QRoundDropcard::process(QList<int> &msg)
{
    if(m_state == ps_dropcard_start)
    {
        //�Ƿ��м��� �˼�
        m_state = ps_dropcard_end;
    }

    return m_state;
}

//�����׶�
ProcessState QRoundEnding::process(QList<int> &msg)
{
    if(m_state == ps_ending_start)
    {
        //�Ƿ��м��� ���� ����
        m_state = ps_ending_end;
    }

    return m_state;
}

//--------------�Ƶļ���---------
//��и�ɻ�
ProcessState QRoundWuxiekeji::process(QList<int> msg)
{
    if(m_state == ps_wuxiekeji_start)
    {
        //ѯ������Ƿ����и�ɻ�
        m_state = ps_wuxiekeji_waiting;
    }
    else if(m_state == ps_wuxiekeji_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_wuxiekeji_unvalid;
        }
        else if(protocol == c2s_request_wuxiekeji_cancel)
        {
            //�Ƿ������˶�����ȡ��
            if(1)
            {
                m_state = ps_wuxiekeji_unvalid;
            }
        }
        else if(protocol == c2s_request_use_card)
        {
            QPlayer* player = desk()->playerAt(msg.at(1));
            QCard* card = desk()->cardAt(msg.at(2));
            if(card->isWuxiekeji())
            {
                desk()->dropHandCard(player,card);
                m_state = ps_wuxiekeji_loop_waiting;
            }
        }
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_wuxiekeji_valid;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_wuxiekeji_unvalid;
    }

    return m_state;
}

//�ֲ�˼��
ProcessState QRoundLebusishu::process(QList<int> msg)
{
    if(m_state == ps_lebusishu_start)
    {
        m_state = ps_lebusishu_waiting;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_lebusishu_unhit;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        int index = desk()->popCard();
        QCard* card = desk()->cardAt(index);
        bool hit = card->isJudgeLebusishu();

        if(desk()->hasGaipanSkill())
        {
            if(hit)
            {
                m_state = ps_lebusishu_hit_waiting;
            }
            else
            {
                m_state = ps_lebusishu_unhit_waiting;
            }
        }
        else
        {
            if(hit)
            {
                m_state = ps_lebusishu_hit;
            }
            else
            {
                m_state = ps_lebusishu_unhit;
            }
        }
    }
    else if(m_state == ps_guicai_hit ||
            m_state == ps_guicai_unhit)
    {
        if(m_state == ps_guicai_hit)
        {
            m_state = ps_lebusishu_hit;
        }
        else
        {
            m_state = ps_lebusishu_unhit;
        }
    }

    return m_state;
}

//����
ProcessState QRoundShandian::process(QList<int> msg)
{
    if(m_state == ps_shandian_start)
    {
        m_state = ps_shandian_waiting;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_shandian_unhit;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        int index = desk()->popCard();
        QCard* card = desk()->cardAt(index);
        bool hit = card->isJudgeShandian();

        if(desk()->hasGaipanSkill())
        {
            if(hit)
            {
                m_state = ps_shandian_hit_waiting;
            }
            else
            {
                m_state = ps_shandian_unhit_waiting;
            }
        }
        else
        {
            if(hit)
            {
                m_state = ps_shandian_hit;
            }
            else
            {
                m_state = ps_shandian_unhit;
            }
        }
    }
    else if(m_state == ps_guicai_hit ||
            m_state == ps_guicai_unhit)
    {
        if(m_state == ps_guicai_hit)
        {
            m_state = ps_shandian_hit;
        }
        else
        {
            m_state = ps_shandian_unhit;
        }
    }

    return m_state;
}

//ɱ
ProcessState QRoundSha::process(QList<int> &msg)
{
    if(m_state == ps_sha_start)
    {
        if(canAttack() == false)
        {
            m_state = ps_sha_can_not;
        }

        if(canCixiongshugu() == true)
        {
            m_state = ps_cixiongshugu_check_start;
        }
    }
    else if(m_state == ps_cixiongshugu_drop_card ||
            m_state == ps_cixiongshugu_catch_card ||
            m_state == ps_cixiongshugu_check_cancel)
    {
        m_state = ps_shan_start;
    }
    else if(m_state == ps_shan_failed ||
            m_state == ps_shan_successful)
    {
        if(m_state == ps_shan_failed)
        {
            m_state = ps_sha_successful;
        }
        else
        {
            if(canQinlongyeyuedao())
            {
                m_state = ps_qinlongyeyuedao_check_start;
            }
            else if(canGuanshifu())
            {
                m_state = ps_guanshifu_check_start;
            }
        }
    }

//old
    if(m_state == ps_sha_start)
    {
        if(canAttack() == false)
        {
            m_state = ps_sha_can_not;
        }

        //���ж���û�д���˫��
        if(canCixiongshugu() == true)
        {
            m_state = ps_sha_normal_waiting;
            return ps_cixiongshugu_check_start;
        }

        if(canBaguadun())
        {
            //���״̬,��ϸ����
            m_state = ps_sha_normal_waiting;
            return ps_baguadun_check_start;
        }

        m_state = ps_sha_normal_waiting;
    }
    else if(m_state == ps_sha_normal_waiting)
    {
        //���ɱ�ɹ�
        m_state = ps_sha_successful;
        //�������
        m_state = ps_sha_failed;
    }
    else if(m_state == ps_baguadun_hit ||
            m_state == ps_baguadun_unhit)
    {
        if(m_state == ps_baguadun_hit)
        {
            m_state = ps_sha_failed;
        }
        else
        {
            m_state = ps_sha_normal_waiting;
        }
    }
    else if(m_state == ps_qinlongyeyuedao_unvalid ||
            m_state == ps_qinlongyeyuedao_valid)
    {
        //��Ϣ����msg����,Ҫ���Ƶ�
        m_state = ps_sha_start;
    }
    else if(m_state == ps_guanshifu_valid ||
            m_state == ps_guanshifu_unvalid)
    {
        if(m_state == ps_guanshifu_unvalid)
        {
            m_state = ps_sha_failed;
        }
        else
        {
            m_state = ps_sha_successful;
        }
    }

    if(m_state == ps_sha_failed)
    {
        if(canQinlongyeyuedao() == true)
        {
            m_state = ps_sha_normal_waiting;
            return ps_qinlongyeyuedao_check_start;
        }

        if(canGuanshifu() == true)
        {
            m_state = ps_sha_normal_waiting;
            return ps_guanshifu_check_start;
        }
    }
    else if(m_state == ps_sha_successful)
    {
        if(canHanbingjian() == true)
        {
            //Ŀǰ������û�к�����
            m_state = ps_sha_normal_waiting;
            //return ps_sha_hanbingjian_check_start;
        }

        m_state = ps_hit_sha_normal_demage;
        //���
    }

    return m_state;
}

bool QRoundSha::canAttack()
{
    int baseDistance = desk()->distanceBetweenTwoPlayer(m_attack,m_defense);
    if(baseDistance == -1)
    {
        //�����ڹ���Ŀ��
        return false;
    }

    if(m_attack->hasAttackHorse())
    {
        baseDistance--;
    }

    if(m_defense->hasDefenseHorse())
    {
        baseDistance++;
    }

    if(m_attack->hasAttackEquip())
    {
        QCard* card = m_attack->m_attackEquip;
        QSkill* skill = desk()->skillAt(card->skill());
        baseDistance -= skill->distance();
        baseDistance++;
    }

    //����

    if(baseDistance <= 1)
    {
        return true;
    }

    return false;
}

bool QRoundSha::canCixiongshugu()
{
    QCard* card = m_attack->m_attackEquip;
    if(card != 0)
    {
        if(m_attack->m_hero->isFemale() ^ m_defense->m_hero->isFemale())
        {
            return true;
        }
    }

    return false;
}

bool QRoundSha::canBaguadun()
{
    QCard* d_e = m_defense->m_defenseEquip;
    if(d_e && d_e->isBaguadun())
    {
        QCard* a_e = m_attack->m_attackEquip;
        if(a_e && a_e->isQingangjian())
        {
            return false;
        }

        return true;
    }

    return false;
}

bool QRoundSha::canQinlongyeyuedao()
{
    QCard* a_e = m_attack->m_attackEquip;
    if(a_e && a_e->isQinlongyeyuedao())
    {
        return true;
    }

    return false;
}

bool QRoundSha::canGuanshifu()
{
    QCard* a_e = m_attack->m_attackEquip;
    if(a_e && a_e->isGuanshifu())
    {
        return true;
    }

    return false;
}

//��
ProcessState QRoundShan::process(QList<int> msg)
{
    if(m_state == ps_wanjianqifa_shan_start ||
       m_state == ps_shan_start)
    {
        QCard* d_e = m_defense->m_defenseEquip;
        if(d_e)
        {
            if(d_e->isBaguadun())
            {
                bool needCheck = false;
                if(m_attack)
                {
                    QCard* a_e = m_attack->m_attackEquip;
                    if(a_e && a_e->isQingangjian())
                    {
                        needCheck = false;
                    }
                    else
                    {
                        needCheck = true;
                    }
                }
                else
                {
                    needCheck = true;
                }

                if(needCheck)
                {
                    m_state = ps_baguadun_check_start;
                    return m_state;
                }
            }
        }
        m_state = ps_shan_waiting;
    }
    else if(m_state == ps_baguadun_hit ||
            m_state == ps_baguadun_unhit)
    {
        if(m_state == ps_baguadun_hit)
        {
            m_state = ps_shan_successful;
        }
        else
        {
            m_state = ps_shan_waiting;
        }
    }
    else if(m_state == ps_shan_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai ||
           protocol == c2s_request_shan_cancel)
        {
            m_state = ps_shan_failed;
        }
        else if(protocol == c2s_request_use_card)
        {
            if(m_defense == desk()->playerAt(msg.at(1)))
            {
                QCard* card = desk()->cardAt(msg.at(2));
                if(card->isShan() && desk()->hasHandCard(m_defense,card))
                {
                    m_state = ps_shan_successful;
                }
            }
        }
    }

    return m_state;
}

//����˫��
ProcessState QRoundCixiongshugu::process(QList<int> msg)
{
    if(m_state == ps_cixiongshugu_check_start)
    {
        m_state = ps_cixiongshugu_check_fire_waiting;
    }
    else if(m_state == ps_cixiongshugu_check_fire_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_cixiongshugu_check_cancel;
        }
        else if(protocol == c2s_request_cixiongshugu_cancel)
        {
            if(m_attack == desk()->playerAt(msg.at(1)))
            {
                m_state = ps_cixiongshugu_check_cancel;
            }
        }
        else if(protocol == c2s_request_cixiongshugu_ok)
        {
            if(m_attack == desk()->playerAt(msg.at(1)))
            {
                m_state = ps_cixiongshugu_check_select_waiting;
            }
        }
    }
    else if(m_state == ps_cixiongshugu_check_select_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_cixiongshugu_catch_card;
            desk()->sendCard(m_attack,1);
        }
        else if(protocol == c2s_request_cixiongshugu_catch)
        {
            if(m_defense == desk()->playerAt(msg.at(1)))
            {
                m_state = ps_cixiongshugu_catch_card;
                desk()->sendCard(m_attack,1);
            }
        }
        else if(protocol == c2s_request_cixiongshugu_drop)
        {
            if(m_defense == desk()->playerAt(msg.at(1)))
            {
                desk()->dropCard(m_defense,msg.at(2));
                m_state = ps_cixiongshugu_drop_card;
            }
        }
    }

    return m_state;
}

//���Զ�
ProcessState QRoundBaguadun::process(QList<int> msg)
{
    if(m_state == ps_baguadun_check_start)
    {
        m_state = ps_baguadun_check_waiting;
    }
    else if(m_state == ps_baguadun_check_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_baguadun_unhit;
        }
        else if(protocol == c2s_request_baguadun_cancel)
        {
            if(m_player == desk()->playerAt(msg.at(1)))
            {
                m_state = ps_baguadun_unhit;
            }
        }
        else if(protocol == c2s_request_baguandun_ok)
        {
            if(m_player == desk()->playerAt(msg.at(1)))
            {
                int index = desk()->popCard();
                QCard* card = desk()->cardAt(index);
                bool hit = card->isJudgeBaguadun();

                if(desk()->hasGaipanSkill())
                {
                    if(hit)
                    {
                        m_state = ps_baguadun_hit_waiting;
                    }
                    else
                    {
                        m_state = ps_baguadun_unhit_waiting;
                    }
                }
                else
                {
                    if(hit)
                    {
                        m_state = ps_baguadun_hit;
                    }
                    else
                    {
                        m_state = ps_baguadun_unhit;
                    }
                }
            }
        }
    }
    else if(m_state == ps_guicai_hit ||
            m_state == ps_guicai_unhit)
    {
        if(m_state == ps_guicai_hit)
        {
            m_state = ps_baguadun_hit;
        }
        else
        {
            m_state = ps_baguadun_unhit;
        }
    }

    return m_state;
}

//�������µ�
ProcessState QRoundQinlongyeyuedao::process(QList<int> msg)
{
    if(m_state == ps_qinlongyeyuedao_check_start)
    {
        m_state = ps_qinlongyeyuedao_check_waiting;
    }
    else if(m_state == ps_qinlongyeyuedao_check_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_qinlongyeyuedao_unvalid;
        }
        else if(protocol == c2s_request_qinlongyeyuedao_cancel)
        {
            if(m_player == desk()->playerAt(msg.at(1)))
            {
                m_state = ps_qinlongyeyuedao_unvalid;
            }
        }
        else if(protocol == c2s_request_qinlongyeyuedao_ok)
        {
            if(m_player == desk()->playerAt(msg.at(1)))
            {
                //������ι���һ��ɱ
                m_state = ps_qinlongyeyuedao_valid;
            }
        }
    }

    return m_state;
}

//��ʯ��
ProcessState QRoundGuanshifu::process(QList<int> msg)
{
    if(m_state == ps_guanshifu_check_start)
    {
        m_state = ps_guanshifu_check_waiting;
    }
    else if(m_state == ps_guanshifu_check_waiting)
    {
        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            m_state = ps_guanshifu_unvalid;
        }
        else if(protocol == c2s_request_guanshifu_cancel)
        {
            if(m_player == desk()->playerAt(msg.at(1)))
            {
                m_state = ps_guanshifu_unvalid;
            }
        }
        else if(protocol == c2s_request_guanshifu_ok)
        {
            if(m_player == desk()->playerAt(msg.at(1)))
            {
                QCard* card1 = desk()->cardAt(msg.at(2));
                QCard* card2 = desk()->cardAt(msg.at(3));

                if(desk()->hasHandCard(m_player,card1) && desk()->hasHandCard(m_player,card2))
                {
                    desk()->dropHandCard(m_player,card1);
                    desk()->dropHandCard(m_player,card2);
                    m_state = ps_guanshifu_valid;
                }
            }
        }
    }

    return m_state;
}

//˳��ǣ��
ProcessState QRoundShunshouqianyang::process(QList<int> msg)
{
    if(m_state == ps_shunshouqianyang_start)
    {
        if(canShunshouqianyang() == false)
        {
            m_state = ps_wuxiekeji_unvalid;
        }
        else
        {
            m_state = ps_wuxiekeji_start;
        }
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_shunshouqianyang_unvalid;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_shunshouqianyang_selectcard_start;
    }
    else if(m_state == ps_shunshouqianyang_selectcard_end)
    {
        m_state = ps_shunshouqianyang_valid;
    }

    return m_state;
}

bool QRoundShunshouqianyang::canShunshouqianyang()
{

    if(m_attack && m_defense)
    {
        //½ѷ����Ч
        if(0)
        {
            return false;
        }

        //����Ӣ �޾�������
        if(0)
        {
            return true;
        }

        int baseDistance = desk()->distanceBetweenTwoPlayer(m_attack,m_defense);

        if(m_attack->hasAttackHorse())
        {
            baseDistance--;
        }

        if(m_defense->hasDefenseHorse())
        {
            baseDistance++;
        }

        //����
        if(baseDistance <= 1)
        {
            if(m_defense->hasCard())
            {
                return true;
            }
        }
    }

    return false;
}

//���Ӳ���
ProcessState QRoundGuohechaiqiao::process(QList<int> &msg)
{
    if(m_state == ps_guohechaiqiao_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_guohechaiqiao_unvalid;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_guohechaiqiao_selectcard_start;
    }
    else if(m_state == ps_guohechaiqiao_selectcard_end)
    {
        m_state = ps_guohechaiqiao_valid;
    }

    return m_state;
}

//��ȷ��
ProcessState QRoundWugufengdeng::process(QList<int> &msg)
{
    if(m_state == ps_wugufengdeng_start)
    {
        desk()->createWugufengdengCards();
        m_index = 0;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_index++;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_wugufengdeng_selectcard_start;
    }
    else if(m_state == ps_wugufengdeng_selectcard_end)
    {
        m_index++;
    }

    if(m_index == desk()->livePlayerCount())
    {
        desk()->destoryWugufengdengResidualCards();
        m_state = ps_wugufengdeng_end;
    }
    else
    {
        m_state = ps_wuxiekeji_start;
    }

    return m_state;
}

QPlayer* QRoundWugufengdeng::current()
{
    QPlayer* player = m_player;
    for(int i = 0; i < m_index; ++i)
    {
        QPlayer* nextPlayer = desk()->nextPlayer(player);
        player = nextPlayer;
    }

    return player;
}

//��������
ProcessState QRoundWuzhongshenyou::process(QList<int> &msg)
{
    if(m_state == ps_wuzhongshenyou_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_wuzhongshenyou_unvalid;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        desk()->sendCard(m_player,2);
        m_state = ps_wuzhongshenyou_valid;
    }

    return m_state;
}

//����
ProcessState QRoundJuedou::process(QList<int> &msg)
{
    if(m_state == ps_juedou_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_juedou_unvalid;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_juedou_waiting;
        m_attack_win = true;
    }
    else if(m_state == ps_juedou_waiting)
    {
        m_state = ps_juedou_attack_win;
        m_state = ps_juedou_defense_win;
    }

    return m_state;
}

//��������
ProcessState QRoundNanmanruqin::process(QList<int> &msg)
{
    if(m_state == ps_nanmanruqin_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_index++;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_nanmanruqin_sha_start;
    }
    else if(m_state == ps_sha_successful ||
            m_state == ps_sha_failed)
    {
        //m_index++;
        if(m_state == ps_sha_failed)
        {
            m_state = ps_hit_nanmanruqin_demage;
            return m_state;
        }
        else
        {
            m_index++;
        }
    }

    if(m_index == desk()->livePlayerCount())
    {
        m_state = ps_nanmanruqin_end;
    }
    else
    {
        m_state = ps_wuxiekeji_start;
    }

    return m_state;
}

QPlayer* QRoundNanmanruqin::current()
{
    QPlayer* player = m_player;
    for(int i = 0; i < m_index; ++i)
    {
        QPlayer* nextPlayer = desk()->nextPlayer(player);
        player = nextPlayer;
    }

    return player;
}

//����뷢
ProcessState QRoundWanjianqifa::process(QList<int> &msg)
{
    if(m_state == ps_wanjianqifa_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_index++;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_wanjianqifa_shan_start;
    }
    else if(m_state == ps_shan_successful ||
            m_state == ps_shan_failed)
    {
        //m_index++; //�������Ѿ�����ȥ��
        if(m_state == ps_shan_failed)
        {
            m_state = ps_hit_wanjianqifa_demage;
            return m_state;
        }
        else
        {
            m_index++;
        }
    }

    if(m_index == desk()->livePlayerCount())
    {
        m_state = ps_wanjianqifa_end;
    }
    else
    {
        m_state = ps_wuxiekeji_start;
    }

    return m_state;
}

QPlayer* QRoundWanjianqifa::current()
{
    QPlayer* player = m_player;
    for(int i = 0; i < m_index; ++i)
    {
        QPlayer* nextPlayer = desk()->nextPlayer(player);
        player = nextPlayer;
    }

    return player;
}

//��԰����
ProcessState QRoundTaoyuanjieyi::process(QList<int> &msg)
{
    if(m_state == ps_taoyuanjieyi_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_index++;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        //��Ѫ
        m_index++;
    }

    if(m_index == desk()->livePlayerCount())
    {
        m_state = ps_taoyuanjieyi_end;
    }
    else
    {
        m_state = ps_wuxiekeji_start;
    }

    return m_state;
}

QPlayer* QRoundTaoyuanjieyi::current()
{
    QPlayer* player = m_player;
    for(int i = 0; i < m_index; ++i)
    {
        QPlayer* nextPlayer = desk()->nextPlayer(player);
        player = nextPlayer;
    }

    return player;
}

//�赶ɱ��
ProcessState QRoundJiedaosharen::process(QList<int> &msg)
{
    if(m_state == ps_jiedaosharen_start)
    {
        m_state = ps_wuxiekeji_start;
    }
    else if(m_state == ps_wuxiekeji_valid)
    {
        m_state = ps_jiedaosharen_unvalid;
    }
    else if(m_state == ps_wuxiekeji_unvalid)
    {
        m_state = ps_jiedaosharen_select_start;
    }

    return m_state;
}

//------------other----------
//�����˺�
ProcessState QRoundCalculatedemage::process(QList<int> &msg)
{
    if(m_state == ps_hit_shandian_demage ||
       m_state == ps_hit_sha_normal_demage ||
       m_state == ps_hit_juedou_demage ||
       m_state == ps_hit_nanmanruqin_demage ||
       m_state == ps_hit_wanjianqifa_demage)
    {
        //����Ѫ�� �ж��Ƿ񻹻���
        //���㣬����Ƶ��
        m_state = ps_hit_shanghai_still_live;
        m_state = ps_hit_shanghai_dead;
        m_state = ps_hit_shanghai_move;
        m_state = ps_hit_shanghai_asktao;
    }
    else if(m_state == ps_asktao_list_ok)
    {
        m_state = ps_hit_shanghai_still_live;
    }
    else if(m_state == ps_asktao_list_failed)
    {
        //����ʧ�ܣ���һ���������в���
        m_state = ps_hit_shanghai_dead;
    }
    if(m_state == ps_hit_shanghai_still_live)
    {
        //���� ���� ���� �ż�
        //���빭�ж�
        if(m_attack)
        {
            QCard* a_e = m_attack->m_attackEquip;
            if(a_e && a_e->isQilingong())
            {
                //�������빭
            }
        }
    }

    return m_state;
}

//�����б�
ProcessState QRoundAsktaolist::process(QList<int> &msg)
{
    if(m_state == ps_asktao_list_start)
    {
        //��m_player��ʼ,����ѯ�����е���
    }
    else if(m_state == ps_asktao_ok || m_state == ps_asktao_failed)
    {
        //����ɹ�,�ж��Ƿ���Ҫ
        //���ʧ��,����ѯ����һ��
        //ps_asktao_list_ok
        //ps_asktao_list_failed
    }

    return m_state;
}

//����
ProcessState QRoundAsktao::process(QList<int> &msg)
{
    if(m_state == ps_asktao_start)
    {
        m_state = ps_asktao_waiting;
    }
    else if(m_state == ps_asktao_waiting)
    {
        //����
        m_state = ps_asktao_ok;
        m_state = ps_asktao_failed;
    }

    return m_state;
}

//����
ProcessState QRoundDead::process(QList<int> &msg)
{
    if(m_state == ps_dead_start)
    {
        if(desk()->isGameOverAfterLeave(m_player))
        {
            m_state = ps_dead_game_over;
        }
        else
        {
            m_state = ps_dead_ok;
        }
    }

    return m_state;
}

//ѡ����
ProcessState QRoundSelectcard::process(QList<int> &msg)
{
    int protocol = msg.at(0);
    if(m_state == ps_wugufengdeng_selectcard_start)
    {
        m_state = ps_wugufengdeng_selectcard_waiting;
    }
    else if(m_state == ps_wugufengdeng_selectcard_waiting)
    {
        if(m_attack)
        {
            Q_ASSERT(m_attack == desk()->playerAt(msg.at(1)));
            if(protocol == s2s_robot_ai)
            {
                desk()->selectWugufengdengCard(m_attack,0);
                m_state = ps_wugufengdeng_selectcard_end;
            }
            else if(protocol == c2s_request_wugufengdeng)
            {
                QCard* card = desk()->cardAt(msg.at(2));
                desk()->selectWugufengdengCard(m_attack,card);
                m_state = ps_wugufengdeng_selectcard_end;
            }
        }
    }
    else if(m_state == ps_shunshouqianyang_selectcard_start)
    {
        //��defense���Ƹ�attackѡ��
        m_state = ps_shunshouqianyang_selectcard_waiting;
    }
    else if(m_state == ps_shunshouqianyang_selectcard_waiting)
    {
        if(m_attack && m_defense)
        {
            if(protocol == s2s_robot_ai)
            {
                QCard* card = 0;
                desk()->m_selectCards.clear();
                if(m_defense->hasHandCard())
                {
                    card = m_defense->m_handCards.first();
                }
                else if(m_defense->hasAttackEquip())
                {
                    card = m_defense->m_attackEquip;
                }
                else if(m_defense->hasDefenseEquip())
                {
                    card = m_defense->m_defenseEquip;
                }
                else if(m_defense->hasAttackHorse())
                {
                    card = m_defense->m_attackHorse;
                }
                else if(m_defense->hasDefenseHorse())
                {
                    card = m_defense->m_defenseHorse;
                }
                else if(m_defense->hasJudgeCard())
                {
                    if(m_defense->m_judgeLebusishu)
                    {
                        card = m_defense->m_judgeLebusishu;
                    }

                    if(m_defense->m_judgeShandian)
                    {
                        card = m_defense->m_judgeShandian;
                    }
                }

                Q_ASSERT(card);
                desk()->m_selectCards.append(card);
                m_state = ps_shunshouqianyang_selectcard_end;
            }
            else if(protocol == c2s_request_shunshouqianyang)
            {
                Q_ASSERT(m_attack == desk()->playerAt(msg.at(1)));
                Q_ASSERT(m_defense == desk()->playerAt(msg.at(3)));

                int index = msg.at(2);
                QCard* card = desk()->findWhichCardSelected(m_defense,index);
                Q_ASSERT(card);
                desk()->m_selectCards.clear();
                desk()->m_selectCards.append(card);
                m_state = ps_shunshouqianyang_selectcard_end;
            }
        }
    }
    else if(m_state == ps_guohechaiqiao_selectcard_start)
    {
        //��defense���Ƹ�attackѡ��
        m_state = ps_guohechaiqiao_selectcard_waiting;
    }
    else if(m_state == ps_guohechaiqiao_selectcard_waiting)
    {
        if(m_attack && m_defense)
        {
            if(protocol == s2s_robot_ai)
            {
                QCard* card = 0;
                desk()->m_selectCards.clear();
                if(m_defense->hasHandCard())
                {
                    card = m_defense->m_handCards.first();
                }
                else if(m_defense->hasAttackEquip())
                {
                    card = m_defense->m_attackEquip;
                }
                else if(m_defense->hasDefenseEquip())
                {
                    card = m_defense->m_defenseEquip;
                }
                else if(m_defense->hasAttackHorse())
                {
                    card = m_defense->m_attackHorse;
                }
                else if(m_defense->hasDefenseHorse())
                {
                    card = m_defense->m_defenseHorse;
                }
                else if(m_defense->hasJudgeCard())
                {
                    if(m_defense->m_judgeLebusishu)
                    {
                        card = m_defense->m_judgeLebusishu;
                    }

                    if(m_defense->m_judgeShandian)
                    {
                        card = m_defense->m_judgeShandian;
                    }
                }

                Q_ASSERT(card);
                desk()->m_selectCards.append(card);
                m_state = ps_guohechaiqiao_selectcard_end;
            }
            else if(protocol == c2s_request_guohechaiqiao)
            {
                Q_ASSERT(m_attack == desk()->playerAt(msg.at(1)));
                Q_ASSERT(m_defense == desk()->playerAt(msg.at(3)));

                int index = msg.at(2);
                QCard* card = desk()->findWhichCardSelected(m_defense,index);
                Q_ASSERT(card);
                desk()->m_selectCards.clear();
                desk()->m_selectCards.append(card);
                m_state = ps_guohechaiqiao_selectcard_end;
            }

        }
    }

    return m_state;
}

ProcessState QRoundSelectjiedaosharen::process(QList<int> &msg)
{
    if(m_state == ps_jiedaosharen_select_start)
    {
        m_state = ps_jiedaosharen_select_waiting;
    }
    else if(m_state == ps_jiedaosharen_select_waiting)
    {
        //ps_jiedaosharen_select_normal_sha
        //ps_jiedaosharen_select_skill_sha
        //ps_jiedaosharen_select_drop
    }

    return m_state;
}

//----------------------�������佫����-----------------
//���
ProcessState QRoundGuicai::process(QList<int> &msg)
{
    if(m_state == ps_guicai_lebusishu_start ||
       m_state == ps_guicai_shandian_start ||
       m_state == ps_guicai_baguadun_start)
    {
        if(m_state == ps_guicai_lebusishu_start)
        {
            m_type = guicai_lebusishu;
        }
        else if(m_state == ps_guicai_shandian_start)
        {
            m_type = guicai_shandian;
        }
        else if(m_state == ps_guicai_baguadun_start)
        {
            m_type = guicai_baguadun;
        }

        m_state = ps_guicai_waiting;
    }
    else if(m_state == ps_guicai_waiting)
    {
        if(m_type == guicai_none)
        {
            return m_state;
        }

        int protocol = msg.at(0);
        if(protocol == s2s_robot_ai)
        {
            if(m_hit)
            {
                m_state = ps_guicai_hit;
            }
            else
            {
                m_state = ps_guicai_unhit;
            }
        }
        else if(protocol == c2s_request_use_skill)
        {
            QPlayer* player = desk()->playerAt(msg.at(1));
            if(player->m_hero->hasGuicai())
            {
                QCard* card = desk()->cardAt(msg.at(2));
                if(m_type == guicai_lebusishu)
                {
                    if(card->isJudgeLebusishu())
                    {
                        m_hit = true;
                    }
                    else
                    {
                        m_hit = false;
                    }
                }
                else if(m_type == guicai_shandian)
                {
                    if(card->isJudgeShandian())
                    {
                        m_hit = true;
                    }
                    else
                    {
                        m_hit = false;
                    }
                }
                else if(m_type == guicai_baguadun)
                {
                    if(card->isJudgeBaguadun())
                    {
                        m_hit = true;
                    }
                    else
                    {
                        m_hit = false;
                    }
                }

                if(m_hit)
                {
                    m_state = ps_guicai_hit;
                }
                else
                {
                    m_state = ps_guicai_unhit;
                }
            }
        }
    }

    return m_state;
}



//����
QRoundLuoshen::QRoundLuoshen(QPlayer* player,QRoundNode* parent,QDesk *desk)
    :QRoundNode(parent,desk),m_player(player)
{}

ProcessState QRoundLuoshen::process(QList<int> &msg)
{
//    QHero* hero = m_player->m_hero;
//    Q_ASSERT(hero->hasLuosheng());

//    int index = desk()->lookCard(0);
//    QCard* card = desk()->cardAt(index);
//    if(card->isJudgeLuoshen())
//    {
//        //s2c_send_luoshen_card
//        return p_luoshen_ok;
//    }

    return ps_waiting;
}

//����
QRoundGuanxin::QRoundGuanxin(QPlayer* player,QRoundNode* parent,QDesk *desk)
    :QRoundNode(parent,desk),m_player(player)
{}

ProcessState QRoundGuanxin::process(QList<int> &msg)
{
//    int needCard = desk()->guanxinNeedCardNumber();
//    for(int i = 0; i < needCard; i++)
//    {
//        //int index = desk()->lookCard(i);
//        //s2c_send_guanxin_card
//    }

    return ps_waiting;
}

//����
QRoundShensu::QRoundShensu(QPlayer* player,QRoundNode* parent,QDesk *desk)
    :QRoundNode(parent,desk),m_player(player)
{}
