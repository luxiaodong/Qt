//
//  GAStar.h
//  Fairyland
//
//  Created by  on 12-1-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#ifndef Fairyland_GAStar_h
#define Fairyland_GAStar_h

#include <list>

#define ASTAR_NONE  0
#define ASTAR_OPEN  1
#define ASTAR_CLOSE 2
#define ASTAR_DEAD  3

#define ASTAR_FOUR  0
#define ASTAR_EIGHT 1

class GAStarNode
{
public:
    int f(){return m_g + m_h;}
    int m_h;
    int m_g;
    int m_index;
    int m_tag; //0.none //1.open //2.close. 3.dead
    GAStarNode* m_parent;
};

class GAStar
{
public:
    GAStar();
    ~GAStar();
    void setMapInformation(int* map, int width, int height);
    bool searchPath(int start, int end);
    bool searchPath(int startX, int startY, int endX, int endY);
    
public:
    std::list<int>  m_best;

private:
    void setSearchMode(int mode);//mode 0.4个方向, 1.八个方向
    void clearNodes();
    bool searchPath();
    
    GAStarNode* nodeAt(int index);
    GAStarNode* nodeAt(int i, int j);

    void insertNodeToOpen(int index);
    int  deleteMinfIndexInOpen();
    void addNodeToClose(int index);
    void deleteNodeFromClose(int index);

    bool isValidNode(int x, int y);
    bool isEndNodeInOpen();
    int  calculateHValue(int i, int j);


    bool isNeighourNode(int x1, int y1, int x2, int y2);
    bool isDiagonalNode(int x1, int y1, int x2, int y2);
private:
    int     m_width;
    int     m_height;
    int     m_start;
    int     m_end;

    int     m_endX;
    int     m_endY;
    int     m_searchMode;
    
    GAStarNode**    m_nodes;
    
    std::list<int>  m_open;
    std::list<int>  m_close;
};


#endif
