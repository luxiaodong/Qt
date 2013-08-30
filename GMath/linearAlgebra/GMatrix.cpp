#include "GMatrix.h"

GMatrix::GMatrix(const int row, const int col)
{
    m_row = row;
    m_col = col;
    int size = m_row*m_col;
    m_data = new double[size];
    for(int i = 0; i < size; ++i)
    {
        m_data[i] = 0.0f;
    }
}

GMatrix::GMatrix(const double value, const int row, const int col)
{
    m_row = row;
    m_col = col;
    int size = m_row*m_col;
    m_data = new double[size];
    for(int i = 0; i < size; ++i)
    {
        m_data[i] = value;
    }
}

GMatrix::GMatrix(const double* pValue, const int row, const int col)
{
    m_row = row;
    m_col = col;
    int size = m_row*m_col;
    m_data = new double[size];
    for(int i = 0; i < size; ++i)
    {
        m_data[i] = pValue[i];
    }
}

GMatrix::GMatrix(GMatrix& m)
{
    m_row = m.row();
    m_col = m.col();
    int size = m_row*m_col;
    m_data = new double[size];
    for(int i = 0; i < size; ++i)
    {
        m_data[i] = m[i];
    }
}

GMatrix::~GMatrix()
{
    delete[] m_data;
}
