#ifndef QSKILL_H
#define QSKILL_H

class QSkill
{
public:
    QSkill(int id, int ability, int distance);
    int id() const{return m_id;}
    int ability() const{return m_ability;}
    int distance() const{return m_distance;}
private:
    int m_id;
    int m_ability;
    int m_distance;
};

#endif // QSKILL_H
