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

#ifndef GRAPHICALITEM_H
#define GRAPHICALITEM_H

#include <QPainter>
#include <QColor>
#include <set>
#include <unordered_set>
#include <map>
#include "common.h"
#include "smartptr.h"
#include "gerbergenerator.h"

typedef map<QString,GraphicalItem*> STORAGE_MAP;

//basic class for all types of items
class GraphicalItem : public QObject
{
    Q_OBJECT
protected:
    ITEM_ID m_id;
    //the item may be a child of an other item
    // for example a plate may be a child of chip
    //in this case the item has a parent;
    GraphicalItem *m_parent;
    bool m_bVisible;
    BOARD_LEVEL_ID m_level;
    //selected items are drawn with special color
    bool m_selected;
    ITEMS_ORIENTATION m_type;
    float m_x,m_y; //coordinates of this item
    unique_ptr<PointF> m_dragCoord;
    // when == 0, it may be deleted
public:
    GraphicalItem(BOARD_LEVEL_ID level,float x,float y,ITEM_ID id=-1);
    virtual ~GraphicalItem(){}
    virtual bool isPointIn(int x,int y,BOARD_LEVEL_ID level) = 0;
    virtual bool isPointInF(float x,float y,BOARD_LEVEL_ID level) = 0;
    virtual ITEM_ID isRectOccupied(PointF& top,PointF& bottom) = 0;
    virtual void paintItem(QPainter& p,QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID idLevel = LEVEL_NONE) = 0;
    //paints item into buffer(pixmap for instance) with relative coordinates : difX,difY
    //difX,difY are added to the coordinates of the item
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter&p ,QColor& c,int zoom,int zoom_d) = 0;
//    virtual void paintItemWithConnector(QPainter& p,QColor& c,BOARD_LEVEL_ID idLevel = LEVEL_NONE);
    virtual void setSelected(bool select);
    virtual bool isSelected();
    virtual void moveUp(BOARD_LEVEL_ID idLevel);
    virtual void moveDown(BOARD_LEVEL_ID idLevel);
    virtual void moveLeft(BOARD_LEVEL_ID idLevel);
    virtual void moveRight(BOARD_LEVEL_ID idLevel);
    virtual void moveFor(BOARD_LEVEL_ID idLevel,PointF dif);
    void setVisible(bool visible){ m_bVisible = visible;}
    virtual bool isAboveNode(QPoint& p,int& index) = 0;
    virtual int prepareToChange(const QPoint& p) = 0;
    virtual void moveNode(PointF& ,int ){}
    virtual void insertNode(int){}
    //returns coordinates of removed node if the result == true
    virtual bool removeNode(int , PointF&){return false;}
    BOARD_LEVEL_ID getLevel(){return m_level;}
    //for those items that supports more than one layer
    virtual void getLevels(set<BOARD_LEVEL_ID>& setIds);
    void setLevel(BOARD_LEVEL_ID level){m_level = level;}
    //rotate item around a point
    virtual void rotate(BOARD_LEVEL_ID ,int ,int ,int  ){}
    virtual PointF getPos() = 0;
    //cordinates either relative to the parent(when the item is in the container) either absolute
    virtual float x(){return m_x;}
    virtual float y(){return m_y;}
    //absolute coordinates
    virtual float abs_x();
    virtual float abs_y();
    virtual void setX(float x);
    virtual void setY(float y);
    virtual GraphicalItem* isConnectable(PointF& pPt);
    void setParent(GraphicalItem *p,bool bRecalcCoord = true);
    void unsetParent(bool bRecalcCoord = true);
    void coordRelativeToAbsolute(float& x,float& y);
    void coordAbsoluteToRelative(float& x,float& y);
    void setType(ITEMS_ORIENTATION t){m_type = t;}
    ITEMS_ORIENTATION getType(){return m_type;}
    virtual SmartPtr<GraphicalItem> clone() = 0;
    virtual void setGeometry(GeomCommonProps& props) = 0;
    virtual GeomCommonProps getGeometry() = 0;
    virtual void setID(ITEM_ID id){m_id = id;}
    ITEM_ID getID(){return m_id;}
    virtual void connect(ITEM_ID connectorID) = 0;
    virtual void disconnect(ITEM_ID id) = 0;
    //returns connected items
    virtual void getConnectors(set<ITEM_ID>& connectors) = 0;
    virtual bool isAboveTheCenter(PointF &pt) ;
    virtual void setDragCoord(PointF& pt);
    virtual void releaseDragCoord(){m_dragCoord.release();}
    virtual bool isInArea(int x,int y) = 0;
    //returns squire area occupied by simple item
    //returns PointF(-1,-1) for containers
    virtual PointF getArea() = 0;
    virtual bool isGerberSupported(){return false;}
    virtual string getGerberString(GerberGenerator& gen,BOARD_LEVEL_ID idLevel){return string();}
};

#endif // GRAPHICALITEM_H
