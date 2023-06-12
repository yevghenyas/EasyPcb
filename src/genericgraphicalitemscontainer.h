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

#ifndef GENERICGRAPHICALITEMSCONTAINER_H
#define GENERICGRAPHICALITEMSCONTAINER_H

#include "graphicalitemscontainer.h"


class GenericGraphicalItemsContainer :  public GraphicalItemsContainer
{
    vector<SmartPtr<GraphicalItem> > m_items;
    bool m_bParent;
protected:
public:
    GenericGraphicalItemsContainer(float x,float y,BOARD_LEVEL_ID level,vector<SmartPtr<GraphicalItem> >&& items,
                                   ITEM_ID id);
    virtual ~GenericGraphicalItemsContainer() override;
    virtual vector<SmartPtr<GraphicalItem> >* getChildren() override;
    virtual bool unGroupItems() override;
    virtual void setChildren(vector<SmartPtr<GraphicalItem> >&& children) override;
    virtual void deleteAllChildren() override;
    virtual bool groupItemsWithCoord(float x,float y);
    virtual void groupItems() override;
    virtual float getMaxX() override;
    virtual float getMaxY() override;
    virtual int prepareToChange(const QPoint& p) override;
    virtual bool isAboveNode(QPoint& p, int& index) override ;
    virtual void moveRight(BOARD_LEVEL_ID idLevel) override;
    virtual void moveLeft(BOARD_LEVEL_ID idLevel) override;
    virtual void moveDown(BOARD_LEVEL_ID idLevel) override;
    virtual void moveUp(BOARD_LEVEL_ID idLevel) override;
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d) override;
    virtual void paintItem(QPainter& p, QColor& c,int zoom,int zoom_d, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointIn(int x, int y, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointInF(float x, float y, BOARD_LEVEL_ID idLevel) override;
    virtual void setSelected(bool select) override;
    virtual void rotate(BOARD_LEVEL_ID idLevel,int x, int y, int grad) override;
    virtual PointF getPos() override {return PointF(m_x,m_y); }
    virtual void moveFor(BOARD_LEVEL_ID idLevel, PointF dif) override;
    virtual void getLevels(set< BOARD_LEVEL_ID >& setIds) override;
    bool isParent(){return m_bParent;}
    void setAsParent(bool bParent,bool bRecalcCoord);
    virtual SmartPtr<GraphicalItem> clone() override;
    virtual void setGeometry(GeomCommonProps& props) override;
    virtual GeomCommonProps getGeometry() override {return GeomCommonProps(0.0f,false);}
    virtual GraphicalItem* isConnectable(PointF& pt) override;
    virtual void setID(ITEM_ID id) override;
    virtual void connect(ITEM_ID connectorID) override;
    virtual void disconnect(ITEM_ID connectorID) override;
    virtual void getConnectors(set<ITEM_ID>& connectors) override;
    virtual ITEM_ID isRectOccupied(PointF& ,PointF& ) override;
    virtual bool isInArea(int x,int y) override;
    virtual PointF getArea() override;
    virtual bool isGerberSupported() override {return true;}
    virtual string getGerberString(GerberGenerator& gen,
                                   BOARD_LEVEL_ID idLevel) override;
};

#endif // GENERICGRAPHICALITEMSCONTAINER_H
