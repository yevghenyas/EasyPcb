/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef DIPCHIPGRAPHICALITEM_H
#define DIPCHIPGRAPHICALITEM_H

#include "graphicalitem.h"
#include "graphicalitemscontainer.h"
#include <vector>

#define PACK_LINE_WIDTH 2

class DipChipGraphicalItem :  public GraphicalItemsContainer
{
   int m_x;
   int m_y;
   int m_width;
   int m_n;
   BOARD_LEVEL_ID m_packLevel;
   vector<GraphicalItem*> m_plates;
public:
    enum ORIENTATION {VERTICAL,HORIZONTAL};
    //n - number of contacts,
    DipChipGraphicalItem(BOARD_LEVEL_ID idLevel,
                         BOARD_LEVEL_ID idPackLevel,QPoint p,int width,int n,
                         int k = 1,int k1 = 1);
    ~DipChipGraphicalItem();
    virtual void prepareToChange(const QPoint& p);
    virtual bool isAboveNode(QPoint p, int& index);
    virtual void moveRight(BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void moveLeft(BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void moveDown(BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void moveUp(BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void zoomIn(BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void zoomOut(BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void zoom(int k);
    virtual void paintItemIntoBuffer(QPainter& p, QColor& c,int zoom_d);
    virtual void paintItem(QPainter& p, QColor& c,BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual bool isPointIn(int x, int y);
    virtual void groupItems(std::vector< GraphicalItem* > toGroup);
    virtual bool unGroupItems();
    virtual void setPackLevel(BOARD_LEVEL_ID packLevel);
    virtual vector<GraphicalItem*>* getChildren();
    virtual BOARD_LEVEL_ID getPackLevel();
    virtual void setSelected(bool select);
    virtual void groupItems(std::vector< GraphicalItem* >& toGroup);
    virtual QPoint getPos(){return QPoint(m_x,m_y);};
    
};

#endif // DIPCHIPGRAPHICALITEM_H
