#include "GVector.h"

GVector::GVector(int n)
{
    m_length = n;
    m_data = new double[n];
    for(int i = 0; i < n; ++i)
    {
        m_data[i] = 0.0f;
    }
}

GVector::GVector(const double value, const int n)
{
    m_length = n;
    m_data = new double[n];
    for(int i = 0; i < n; ++i)
    {
        m_data[i] = value;
    }
}

GVector::GVector(const double* pValue, const int n)
{
    m_length = n;
    m_data = new double[n];
    for(int i = 0; i < n; ++i)
    {
        m_data[i] = pValue[i];
    }
}

GVector::GVector(GVector& v)
{
    int n = v.size();
    m_length = n;
    m_data = new double[n];
    for(int i = 0; i < n; ++i)
    {
        m_data[i] = v[i];
    }
}

GVector::~GVector()
{
    delete[] m_data;
}
