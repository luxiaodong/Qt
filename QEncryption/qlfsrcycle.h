#ifndef QLFSRCYCLE_H
#define QLFSRCYCLE_H

#include <QList>

class QLFSRCycle
{
public:
    QLFSRCycle();
    QLFSRCycle(QList<int>& list);

public:
    void init2_1();
    void init3_1();
    void init3_2();
    void init4_1();
    void init4_2();
    void init5_2();
    void init5_3();
    void init5_3_2_1();
    void init5_4_2_1();
    void init5_4_3_1();
    void init5_4_3_2();
    void init5_3_1(); //bad test
    void init8_4_3_2();
    void init20_3();
    void init29_26_25_22_21_20_19_18_15_14_13_12_9_4();
    void initList(QList<int>& list);

public:
    void cycle();
    int calculateKeyByBit();
    int nextMask(int mask);
    int nextMask2(int mask);

    void test();
private:
    QList<int> m_list;
};

#endif // QLFSRCYCLE_H
