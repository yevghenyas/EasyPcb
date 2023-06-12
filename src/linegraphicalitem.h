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

#ifndef LINEGRAPHICALITEM_H
#define LINEGRAPHICALITEM_H

#include "graphicalitem.h"
#include <vector>
#include <QPoint>
using namespace std;

/*

class LineGraphicalItem :  public GraphicalItem
{
    using VecCoordIdx = vector<PointF>::size_type;
    vector<PointF> m_points;
    float m_w;
    LINE_STYLE m_style;
    unique_ptr<map<int,ITEM_ID>> m_vcCons;
    void paintInternal(float difX,float difY,QPainter& p, QColor& c);
public:
    LineGraphicalItem(vector<PointF>&& points,float w,BOARD_LEVEL_ID level,int k,int k1,ITEM_ID id);
    LineGraphicalItem(vector<QPoint>& points,float w,BOARD_LEVEL_ID level,int k,int k1,ITEM_ID id);
    LineGraphicalItem(vector<VcConCunstrInfo>& points,float w,BOARD_LEVEL_ID level,int k,int k1,ITEM_ID id);
    LineGraphicalItem(const LineGraphicalItem& other) = delete;
    virtual ~LineGraphicalItem() override;
    virtual void zoom(int k) override;
    virtual void paintItem(QPainter& p, QColor& c,BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c) override;

    virtual bool isPointIn(int x, int y, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointInF(float x, float y, BOARD_LEVEL_ID idLevel) override;
    virtual void moveDown(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void moveLeft(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void moveRight(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void moveUp(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void addNode(int curNode) override;
    virtual bool removeNode(int curNode, PointF& pt) override;
    virtual bool isAboveNode(QPoint& p,int& index) override;
    bool isAboveNodeInternal(QPoint& p,int& index);
    virtual int prepareToChange(const QPoint& p) override;
    virtual void moveNode(PointF& p,int index) override;
    virtual PointF getPos() override{return PointF(0,0);}
    virtual void setX(float x) override;
    virtual void setY(float y) override;
    virtual void moveFor(BOARD_LEVEL_ID idLevel, PointF dif) override;
    float width(){return m_w;}
    void setWidth(float w){m_w = w;}
    vector<PointF>* getPoints(){return &m_points;}
    virtual SmartPtr<GraphicalItem> clone() override;
    virtual void setGeometry(GeomCommonProps& props) override;
    virtual GeomCommonProps getGeometry() override {return GeomCommonProps(m_w,true);}
    virtual GraphicalItem* isConnectable(PointF& pt) override;
    virtual void connectorAdd(ITEM_ID connectorID,PointF& pt) override;
    virtual void connectorRemove(ITEM_ID connectorID) override;
    virtual void getConnectors(set<ITEM_ID>& connectors) override;
    void getConnectorsEx(map<ITEM_ID,PointF>& connectors);
    virtual ITEM_ID isRectOccupied(PointF& ,PointF& ) override;
    void anotherItemWasChanged(GraphicalItem *p,float dx,float dy);
    LINE_STYLE getStyle(){return m_style;}
    bool isAboveTheCenter(PointF&) override {return false;}
};
*/
#endif // LINEGRAPHICALITEM_H
