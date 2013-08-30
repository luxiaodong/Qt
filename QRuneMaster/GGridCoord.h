//
//  GGridCoord.h
//  Fairyland
//
//  Created by  on 12-1-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef GGRIDCOORD_H
#define GGRIDCOORD_H

class GGridCoord
{
public:
    GGridCoord();

public:
    void setGridAndCoord(int gridWidth, int gridHeight, float coordWidth, float coordHeight);
    bool convertGridToCoord(int gridWidthIndex, int gridHeightIndex);
    bool convertCoordToGrid(float coordWidthValue, int coordHeightValue);

public:
    int     m_gridWidthIndex;
    int     m_gridHeightIndex;
    float   m_coordWidthValue;
    float   m_coordHeightValue;

private:
    float   positiveSlope(float coordX, float coordY);
    float   negativeSlope(float coordX, float coordY);

private:
    int     m_gridWidth;
    int     m_gridHeight;
    float   m_halfCoordWidth;
    float   m_halfCoordHeight;

    float   m_positiveSlopeMin;
    float   m_positiveSlopeMax;
    float   m_negativeSlopeMin;
    float   m_negativeSlopeMax;
};

#endif // GGRIDCOORD_H
