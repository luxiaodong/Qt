#ifndef GVECTOR_H
#define GVECTOR_H

class GVector
{
public:
    explicit GVector(const int n);
    GVector(const double value, const int n);
    GVector(const double* pValue, const int n);
    GVector(GVector& v);
    ~GVector();

    //inline double at(int i){return m_data[i];}
    inline const double operator[](const int i){return m_data[i];}
    inline int size() const{return m_length;}
private:
    int     m_length;
    double* m_data;
};

#endif // GVECTOR_H
