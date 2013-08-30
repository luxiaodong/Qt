//
//  GAStar.cpp
//  Fairyland
//
//  Created by  on 12-1-13.
//  Copyright (c) 2012年 __MyCompanyName__. All rights reserved.
//

#include "GAStar.h"

GAStar::GAStar()
{
    m_searchMode = 0;
    m_nodes = ASTAR_FOUR;
}

GAStar::~GAStar()
{
    this->clearNodes();
}

void GAStar::setMapInformation(int* map, int width, int height)
{
    this->clearNodes();
    
    m_width = width;
    m_height = height;
    int size = width*height;
    
    m_nodes = new GAStarNode*[size];

    for(int i = 0; i < size; ++i)
    {
        GAStarNode* node = new GAStarNode();
        node->m_g = 0;
        node->m_h = 0;
        node->m_index = i;
        node->m_tag = ASTAR_NONE;
        if(map[i] == 0)
        {
            node->m_tag = ASTAR_DEAD;
        }
        node->m_parent = 0;
        m_nodes[i] = node;
    }
}

void GAStar::setSearchMode(int searchMode)
{
    m_searchMode = searchMode;
}

bool GAStar::searchPath(int start, int end)
{
    if(start < 0 || start >= m_width*m_height)
    {
        return false;
    }

    if(end < 0 || end >= m_width*m_height)
    {
        return false;
    }

    if(start == end)
    {
        m_best.clear();
        return true;
    }

    m_start = start;
    m_end = end;
    m_endX = end%m_width;
    m_endY = end/m_width;
    
    return this->searchPath();
}

bool GAStar::searchPath(int startX, int startY, int endX, int endY)
{
    if(this->isValidNode(startX, startY) == false)
    {
        return false;
    }

    if(this->isValidNode(endX, endY) == false)
    {
        return false;
    }

    if(startX == endX || startY == endY)
    {
        m_best.clear();
        return true;
    }

    m_start = startY*m_width + startX;
    m_end = endY*m_width + endX;
    m_endX = endX;
    m_endY = endY;
    
    return this->searchPath();
}

bool GAStar::searchPath()
{
    m_open.clear();
    m_close.clear();
    m_best.clear();
    this->insertNodeToOpen(m_start);

    int x[8] = {1,-1,0,0,-1,-1,1,1};
    int y[8] = {0,0,1,-1,1,-1,1,-1};

    int size = 0;

    if(m_searchMode == ASTAR_FOUR)
    {
        size = 4;
    }
    else if(m_searchMode == ASTAR_EIGHT)
    {
        size = 8;
    }
    
    bool bFind = false;
    while(1)
    {
        if(m_open.size() == 0)
        {
            break;
        }

        if(isEndNodeInOpen() == true)
        {
            bFind = true;
            break;
        }

        int currentIndex = this->deleteMinfIndexInOpen();
        this->addNodeToClose(currentIndex);
        GAStarNode* parent = this->nodeAt(currentIndex);
        int currentX = currentIndex%m_width;
        int currentY = currentIndex/m_width;
        for(int k = 0; k < size; ++k)
        {
            int i = currentX + x[k];
            int j = currentY + y[k];
            int s = 1;
            if(isValidNode(i, j) == true)
            {
                GAStarNode* node = this->nodeAt(i, j);

                if(node->m_tag == ASTAR_OPEN)
                {
                    if(node->m_g > parent->m_g + s)
                    {
                        node->m_parent = parent;
                        node->m_g = parent->m_g + s;
                    }
                }
                else if(node->m_tag == ASTAR_CLOSE)
                {
                    if(parent->m_parent != node)
                    {
                        if(node->m_g > parent->m_g + s)
                        {
                            //may not use
                            node->m_parent = parent;
                            node->m_g = parent->m_g + s;
                            this->deleteNodeFromClose(j*m_width + i);
                            this->insertNodeToOpen(j*m_width + i);
                        }
                    }
                }
                else if(node->m_tag == ASTAR_NONE)
                {
                    node->m_parent = parent;
                    node->m_g = parent->m_g + s;
                    node->m_h = this->calculateHValue(i, j);
                    this->insertNodeToOpen(j*m_width + i);
                }
            }
        }
    }

    if(bFind == true)
    {
        GAStarNode* node = this->nodeAt(m_end);
        while(1)
        {
            m_best.push_front(node->m_index);

            if(node->m_parent == 0)
            {
                break;
            }
            else
            {
                node = node->m_parent;
            }
        }
    }
    
    return bFind;
}

bool GAStar::isEndNodeInOpen()
{
    std::list<int>::iterator it;
    for (it = m_open.begin(); it != m_open.end(); it++)
    {
        if(*it == m_end)
        {
            return true;
        }
    }

    return false;
}

void GAStar::insertNodeToOpen(int index)
{
    m_open.push_back(index);
    this->nodeAt(index)->m_tag = ASTAR_OPEN;
}

void GAStar::addNodeToClose(int index)
{
    m_close.push_back(index);
    this->nodeAt(index)->m_tag = ASTAR_CLOSE;
}

void GAStar::deleteNodeFromClose(int index)
{
    std::list<int>::iterator it;
    for (it = m_close.begin(); it != m_close.end(); it++)
    {
        if(*it == index)
        {
            m_close.erase(it);
            return ;
        }
    }
}

int GAStar::calculateHValue(int i, int j)
{
    int deltX = i - m_endX;
    int deltY = j - m_endY;
    
    if(deltX < 0)
    {
        deltX = 0 - deltX;
    }
    
    if(deltY < 0)
    {
        deltY = 0 - deltY;
    }

    if(m_searchMode == ASTAR_FOUR)
    {
        return deltX + deltY;
    }
    else if(m_searchMode == ASTAR_EIGHT)
    {
        return (deltX > deltY) ? deltX : deltY;
    }

    return 0;
}

int GAStar::deleteMinfIndexInOpen()
{
    int fValue = -1;
    int fIndex = -1;

    std::list<int>::iterator it;
    for (it = m_open.begin(); it != m_open.end(); it++)
    {
        GAStarNode* node = this->nodeAt(*it);
        if(fValue == -1)
        {
            fValue = node->f();
            fIndex = node->m_index;
        }
        else
        {
            if(fValue > node->f())
            {
                fValue = node->f();
                fIndex = node->m_index;
            }
        }
    }

    for (it = m_open.begin(); it != m_open.end(); it++)
    {
        if(*it == fIndex)
        {
            m_open.erase(it);
            break;
        }
    }

    return fIndex;
}

void GAStar::clearNodes()
{
    if(m_nodes != 0)
    {
        int size = m_width*m_height;
        for(int i = 0; i < size; ++i)
        {
            delete m_nodes[i];
        }
        
        delete []m_nodes;
    }
    
    m_nodes = 0;
}

bool GAStar::isValidNode(int x, int y)
{
    if(x < 0 || x >= m_width)
    {
        return false;
    }
    
    if(y < 0 || y >= m_height)
    {
        return false;
    }
    
    return true;
}

bool GAStar::isDiagonalNode(int x1, int y1, int x2, int y2)
{
    int deltX = x1 - x2;
    int deltY = y1 - y2;
    
    if(deltX == 1 || deltX == -1)
    {
        if(deltY == 1 || deltY == -1)
        {
            return true;
        }
    }
    
    return false;
}

bool GAStar::isNeighourNode(int x1, int y1, int x2, int y2)
{
    int deltX = x1 - x2;
    int deltY = y1 - y2;
    
    if(deltX == 0)
    {
        if(deltY == 1 || deltY == -1)
        {
            return true;
        }
    }
    
    if(deltY == 0)
    {
        if(deltX == 1 || deltX == -1)
        {
            return true;
        }
    }
    
    return false;
}

GAStarNode* GAStar::nodeAt(int index)
{
    return this->m_nodes[index];
}

GAStarNode* GAStar::nodeAt(int i, int j)
{
    return this->nodeAt(j*m_width + i);
}
