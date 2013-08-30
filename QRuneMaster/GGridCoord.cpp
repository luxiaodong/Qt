//
//  GGridCoord.h
//  Fairyland
//
//  Created by  on 12-1-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GGridCoord.h"

GGridCoord::GGridCoord()
{
    m_gridWidth = 0;
    m_gridHeight = 0;
}

void GGridCoord::setGridAndCoord(int gridWidth, int gridHeight, float coordWidth, float coordHeight)
{
    m_gridWidth = gridWidth;
    m_gridHeight = gridHeight;
    m_halfCoordWidth = coordWidth/2;
    m_halfCoordHeight = coordHeight/2;

    m_positiveSlopeMin = this->positiveSlope(0.0f - m_halfCoordWidth, 0.0f);
    m_positiveSlopeMax = this->positiveSlope(m_halfCoordWidth, 0.0f);
    m_negativeSlopeMin = this->negativeSlope(0.0f - m_halfCoordWidth, 0.0f);
    m_negativeSlopeMax = this->negativeSlope(m_halfCoordWidth, 0.0f);
}

bool GGridCoord::convertGridToCoord(int gridWidthIndex, int gridHeightIndex)
{
    if(gridWidthIndex < 0 || gridWidthIndex >= m_gridWidth)
    {
        return false;
    }

    if(gridHeightIndex < 0 || gridHeightIndex >= m_gridHeightIndex)
    {
        return false;
    }

    float positiveSlopeValue = (m_positiveSlopeMax - m_positiveSlopeMin)*gridWidthIndex/m_gridWidth + m_positiveSlopeMin;
    float negativeSlopeValue = (m_negativeSlopeMax - m_negativeSlopeMin)*gridHeightIndex/m_gridHeight + m_negativeSlopeMin;

    m_coordWidthValue = (negativeSlopeValue + positiveSlopeValue)/(2*m_halfCoordHeight);
    m_coordHeightValue = (negativeSlopeValue - positiveSlopeValue)/(2*m_halfCoordWidth);
    return true;
}

bool GGridCoord::convertCoordToGrid(float coordWidthValue, int coordHeightValue)
{
    float positiveSlopeValue = this->positiveSlope(coordWidthValue, coordHeightValue);
    float negativeSlopeValue = this->negativeSlope(coordWidthValue, coordHeightValue);

    if(positiveSlopeValue < m_positiveSlopeMin ||
       positiveSlopeValue > m_positiveSlopeMax)
    {
        return false;
    }

    if(negativeSlopeValue < m_negativeSlopeMin ||
       negativeSlopeValue > m_negativeSlopeMax)
    {
        return false;
    }

    m_gridWidthIndex = (positiveSlopeValue - m_positiveSlopeMin)*m_gridWidth/(m_positiveSlopeMax - m_positiveSlopeMin);
    m_gridHeightIndex = (negativeSlopeValue - m_negativeSlopeMin)*m_gridHeight/(m_negativeSlopeMax - m_negativeSlopeMin);
    return true;
}

float GGridCoord::positiveSlope(float coordX, float coordY)
{
    return m_halfCoordHeight*coordX - m_halfCoordWidth*coordY;
}

float GGridCoord::negativeSlope(float coordX, float coordY)
{
    return m_halfCoordHeight*coordX + m_halfCoordWidth*coordY;
}

