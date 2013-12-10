#include "qlfsrcycle.h"
#include <QDebug>

QLFSRCycle::QLFSRCycle()
{}

QLFSRCycle::QLFSRCycle(QList<int>& list)
{
    this->initList(list);
}

void QLFSRCycle::init2_1()
{
    QList<int> list;
    list.append(2);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init3_1()
{
    QList<int> list;
    list.append(3);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init3_2()
{
    QList<int> list;
    list.append(3);
    list.append(2);
    this->initList(list);
}

void QLFSRCycle::init4_1()
{
    QList<int> list;
    list.append(4);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init4_2()
{
    QList<int> list;
    list.append(4);
    list.append(3);
    this->initList(list);
}


void QLFSRCycle::init5_2()
{
    QList<int> list;
    list.append(5);
    list.append(2);
    this->initList(list);
}

void QLFSRCycle::init5_3()
{
    QList<int> list;
    list.append(5);
    list.append(3);
    this->initList(list);
}

void QLFSRCycle::init5_3_2_1()
{
    QList<int> list;
    list.append(5);
    list.append(3);
    list.append(2);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init5_4_2_1()
{
    QList<int> list;
    list.append(5);
    list.append(4);
    list.append(2);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init5_4_3_1()
{
    QList<int> list;
    list.append(5);
    list.append(4);
    list.append(3);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init5_4_3_2()
{
    QList<int> list;
    list.append(5);
    list.append(4);
    list.append(3);
    list.append(2);
    this->initList(list);
}

void QLFSRCycle::init5_3_1()
{
    QList<int> list;
    list.append(5);
    list.append(3);
    list.append(1);
    this->initList(list);
}

void QLFSRCycle::init8_4_3_2()
{
    QList<int> list;
    list.append(8);
    list.append(4);
    list.append(3);
    list.append(2);
    this->initList(list);
}

void QLFSRCycle::init20_3()
{
    QList<int> list;
    list.append(20);
    list.append(3);
    this->initList(list);
}

void QLFSRCycle::init29_26_25_22_21_20_19_18_15_14_13_12_9_4()
{
    QList<int> list;
    list.append(29);
    list.append(26);
    list.append(25);
    list.append(22);
    list.append(21);
    list.append(20);
    list.append(19);
    list.append(18);
    list.append(15);
    list.append(14);
    list.append(13);
    list.append(12);
    list.append(9);
    list.append(4);
    this->initList(list);
}

void QLFSRCycle::initList(QList<int>& list)
{
    m_list = list;
}

void QLFSRCycle::test()
{
    this->init29_26_25_22_21_20_19_18_15_14_13_12_9_4();
    this->cycle();
    return ;

    this->init2_1();
    this->cycle();
    this->init3_1();
    this->cycle();
    this->init3_2();
    this->cycle();
    this->init4_1();
    this->cycle();
    this->init4_2();
    this->cycle();
    this->init5_2();
    this->cycle();
    this->init5_3();
    this->cycle();
    this->init5_3_2_1();\
    this->cycle();
    this->init5_4_2_1();
    this->cycle();
    this->init5_4_3_1();
    this->cycle();
    this->init5_4_3_2();
    this->cycle();

    this->init8_4_3_2();
    this->cycle();
}

void QLFSRCycle::cycle()
{
    int key = this->calculateKeyByBit();
    int loopCount = 0;
    int mask = key;
    qDebug()<<mask;
    return ;
    while(1)
    {
        mask = this->nextMask(mask);
        loopCount++;

        if(mask == key)
        {
            break;
        }

        if( ( (loopCount - 1) & loopCount ) == 0 )
        {
            qDebug()<<loopCount;
        }
    }

    qDebug()<<"the loop is "<<loopCount<<"\n";
}

int QLFSRCycle::nextMask(int mask)
{
    int nextMask = mask>>1;
    int lastBit = 0;

    for(int i = 0; i < m_list.size(); ++i)
    {
        lastBit ^= ((mask>>(m_list.first() - m_list.at(i))) & 0x01);
    }

    if(lastBit == 1)
    {
        nextMask ^= (1<<(m_list.first() - 1));
    }

    //qDebug()<<"the next Mask is "<<nextMask;
    return nextMask;
}

int QLFSRCycle::nextMask2(int mask)
{
    int nextMask = mask<<1;
    int lastBit = 0;

    for(int i = 0; i < m_list.size(); ++i)
    {
        lastBit ^= ((mask>>(m_list.at(i) - 1)) & 0x01);
    }

    nextMask += lastBit;

    int fullone = (2<<m_list.first()) - 1;
    nextMask &= fullone;

    qDebug()<<"the next Mask is "<<nextMask;
    return nextMask;
}

int QLFSRCycle::calculateKeyByBit()
{
    int key = 0;
    foreach(int single, m_list)
    {
        key += (1<<(single - 1));
    }

    return key;
}


