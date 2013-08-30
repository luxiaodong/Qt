#ifndef GMATRIX_H
#define GMATRIX_H

class GMatrix
{
public:
    explicit GMatrix(const int row, const int col);
    GMatrix(const double value, const int row, const int col);
    GMatrix(const double* pValue, const int row, const int col);
    GMatrix(GMatrix& m);
    ~GMatrix();

    inline double operator[](const int index){return m_data[index];}
    inline int row(){return m_row;}
    inline int col(){return m_col;}
private:
    int     m_row;
    int     m_col;
    double* m_data;
};

#endif // GMATRIX_H
