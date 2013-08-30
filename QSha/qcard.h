#ifndef QCARD_H
#define QCARD_H

enum Color{
    color_fangkuai = 0,
    color_hongtao = 1,
    color_caohua = 2,
    color_heitao = 3,
};

enum Skill{
//poker skill
    skill_none = 0,
    skill_sha = 1,//=1
    skill_shan,// = 2;
    skill_tao, //= 3;
    skill_shunshouqianyang,// = 4;//˳��ǣ��
    skill_guohechaiqiao,// = 5;//���Ӳ���
    skill_wugufendeng,// = 6;//��ȷ��
    skill_wuzhongshenyou,// = 7;//��������
    skill_juedou,// = 8;//����
    skill_nanmanruqin,// = 9;//��������
    skill_wanjianqifa,// = 10;//����뷢
    skill_shandian,// = 11;//����
    skill_taoyuanjieyi,// = 12;//��԰����
    skill_wuxiekeji,// = 13;//��и�ɻ�
    skill_jiedaosharen,// = 14;//�赶ɱ��
    skill_lebusishu,// = 15;//�ֲ�˼��
    skill_baguadun,// = 16;//���Զ�
    skill_chitu,// = 17;//����
    skill_zixin,// = 18;
    skill_dawan,// = 19;//����
    skill_jueying,// = 20;//��Ӱ
    skill_dilu,// = 21;//��¬
    skill_zhuahuangfeidian,// = 22;//ץ�Ʒɵ�
    skill_zhugeliannu,// = 23;//�������
    skill_cixiongshuanggu,// = 24;//����˫��
    skill_qingangjian,// = 25;//��ս�
    skill_qinlongyeyuedao,// = 26;//�������µ�
    skill_zhangbashemao,// = 27;//�Ȱ���ì
    skill_guanshifu,// = 28;//��ʯ��
    skill_fangtianhuaji,// = 29;//���컭�
    skill_qilingong,// = 30;//���빭
//hero skill
    skill_rende = 31,//�ʵ�
    skill_jijiang,// = 32;
    skill_wusheng,// = 33;��ʥ
    skill_paoxiao,// = 34;����
    skill_guanxin,// = 35;
    skill_kongcheng,// = 36;
    skill_longdan,// = 37;����
    skill_tieqi,// = 38;
    skill_jizhi,// = 39;
    skill_qicai,// = 40;
    skill_niaoji,// = 41;
    skill_jianxiong,// = 42;
    skill_hujia,// = 43;
    skill_tuxi,// = 44;
    skill_luosheng,// = 45;
    skill_qinguo,// = 46;
    skill_fankui,// = 47;
    skill_guicai,// = 48;
    skill_ganglie,// = 49;
    skill_luoyi,// = 50;
    skill_tianji,// = 51;
    skill_yiji,// = 52;
    skill_zhiheng,// = 53; �ƺ�
    skill_jiuyuan,// = 54;
    skill_yinzi,// = 55;
    skill_fanjian,// = 56; ����
    skill_qixi,// = 57;  ��Ϯ,�����Ӳ���
    skill_qianxun,// = 58;
    skill_lianyin,// = 59;
    skill_guose,// = 60; ��ɫ
    skill_liuli,// = 61;
    skill_kurou,// = 62; ����
    skill_keji,// = 63;
    skill_jijiu,// = 64;
    skill_qinnang,// = 65; ����
    skill_wushang,// = 66;
    skill_biyue,// = 67;
    skill_lijian,// = 68; ���
    skill_jieyin,// = 69; ����
    skill_mashu,// = 70;
    skill_liegong,// = 71;
    skill_kuanggu,// = 72;
    skill_shensu,// = 73;
    skill_jushou,// = 74;
    skill_tianxiang, // = 75;
    skill_hongyan, // = 76;
    skill_buqu, // = 77;
    skill_leiji,// = 78
    skill_guidao,// = 79
    skill_huangtian, // = 80
    skill_guhuo, // 81
};

//PlayCard.dat��ÿ������������ԣ����е��ĺ͵�����ͬһ��ֵ������ָ����
class QCard
{
public:
    QCard(int id,int number, int color, int skill, int skill2);
    int id() const{return m_id;}
    int color() const{return m_color;}
    int skill() const{return m_skill;}
    int skill2() const{return m_skill2;}
    bool isCard(Skill card) const;
public:
    int number() const{return m_number;}

public:
    bool isSha() const{return this->isCard(skill_sha);}
    bool isShan() const{return this->isCard(skill_shan);}
    bool isTao() const{return this->isCard(skill_tao);}
    bool isShunshouqianyang() const{return this->isCard(skill_shunshouqianyang);}
    bool isGuohechaiqiao() const{return this->isCard(skill_guohechaiqiao);}
    bool isWugufendeng() const{return this->isCard(skill_wugufendeng);}
    bool isWuzhongshenyou() const{return this->isCard(skill_wuzhongshenyou);}
    bool isJuedou() const{return this->isCard(skill_juedou);}
    bool isNanmanruqin() const{return this->isCard(skill_nanmanruqin);}
    bool isWanjianqifa() const{return this->isCard(skill_wanjianqifa);}
    bool isShandian() const{return this->isCard(skill_shandian);}
    bool isTaoyuanjieyi() const{return this->isCard(skill_taoyuanjieyi);}
    bool isWuxiekeji() const{return this->isCard(skill_wuxiekeji);}
    bool isJiedaosharen() const{return this->isCard(skill_jiedaosharen);}
    bool isLebusishu() const{return this->isCard(skill_lebusishu);}
    bool isBaguadun() const{return this->isCard(skill_baguadun);}
    bool isChitu() const{return this->isCard(skill_chitu);}
    bool isZixin() const{return this->isCard(skill_zixin);}
    bool isDawan() const{return this->isCard(skill_dawan);}
    bool isJueying() const{return this->isCard(skill_jueying);}
    bool isDilu() const{return this->isCard(skill_dilu);}
    bool isZhuahuangfeidian() const{return this->isCard(skill_zhuahuangfeidian);}
    bool isZhugeliannu() const{return this->isCard(skill_zhugeliannu);}
    bool isCixiongshuanggu() const{return this->isCard(skill_cixiongshuanggu);}
    bool isQingangjian() const{return this->isCard(skill_qingangjian);}
    bool isQinlongyeyuedao() const{return this->isCard(skill_qinlongyeyuedao);}
    bool isZhangbashemao() const{return this->isCard(skill_zhangbashemao);}
    bool isGuanshifu() const{return this->isCard(skill_guanshifu);}
    bool isFangtianhuaji() const{return this->isCard(skill_fangtianhuaji);}
    bool isQilingong() const{return this->isCard(skill_qilingong);}

//��ɫ�ж�
    bool isFangkuai() const;
    bool isHongtao() const;
    bool isCaohua() const;
    bool isHeitao() const;

//�Ƿ��Ƿ��ӳ������
    bool isNonDeferredJingnang() const;
    bool isJingnang() const;

//�ж�
    bool isJudgeShandian() const;
    bool isJudgeLebusishu() const;
    bool isJudgeBaguadun() const;
    bool isJudgeRenwangdun() const;
    bool isJudgeLuoshen() const;

//����,����,��
    bool isAttackEquip() const;
    bool isDefenseEquip() const;
    bool isAttackHorse() const;
    bool isDefenseHorse() const;

private:
    int m_id;
    int m_number;
    int m_color;
    int m_skill;
    int m_skill2;
};


#endif // QCARD_H
