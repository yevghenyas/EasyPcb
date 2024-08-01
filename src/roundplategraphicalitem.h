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

#ifndef ROUNDPLATEGRAPHICALITEM_H
#define ROUNDPLATEGRAPHICALITEM_H

#include "graphicalitem.h"

class RoundPlateGraphicalItem :  public GraphicalItem
{
    float m_d; //diameter of the hole
    float m_D; //diameter of the plate
    unique_ptr<unordered_set<ITEM_ID>> m_vcCons;
    void paintInternal(QPainter& painter,QColor& c,PointF& point,int zoom,int zoom_d);
public:
    RoundPlateGraphicalItem(float x,float y,float d1,float d2,BOARD_LEVEL_ID level,ITEM_ID id);
    virtual ~RoundPlateGraphicalItem() override;
    virtual void paintItem(QPainter& p, QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d) override;
    virtual bool isPointIn(int x, int y, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointInF(float x, float y, BOARD_LEVEL_ID idLevel) override;
    float d1(){return m_d;}
    float d(){return m_D;}
    virtual bool isAboveNode(QPoint& p,int& index) override;
    virtual int prepareToChange(const QPoint& ) override {return -1;}
    virtual void rotate(BOARD_LEVEL_ID idLevel,int x, int y,int grad) override;
    virtual PointF getPos() override {return PointF(m_x,m_y);}
    virtual void setX(float x) override;
    virtual void setY(float y) override;
    void setd(float d){m_D = d;}
    void setd1(float d){m_d = d;}
    virtual SmartPtr<GraphicalItem> clone() override;
    virtual void setGeometry(GeomCommonProps& props) override;
    virtual GeomCommonProps getGeometry() override {return GeomCommonProps(m_D,m_d,true);}
    virtual GraphicalItem* isConnectable(PointF& pt) override;
    virtual void connect(ITEM_ID connectorID) override;
    virtual void getConnectors(set<ITEM_ID>& connectors) override;
    virtual int getConnectorsNumber() override;
    virtual void disconnect(ITEM_ID id) override;
    virtual ITEM_ID isRectOccupied(PointF& ,PointF& ) override;
    virtual bool isInArea(int x,int y) override;
    virtual PointF getArea() override;
    virtual bool isGerberSupported() override {return true;}
    virtual string getGerberString(GerberGenerator& gen,
                                   BOARD_LEVEL_ID idLevel) override;
    virtual bool isHoleCapable() override;
    virtual float getHoleDiameter() override;
};

#endif // ROUNDPLATEGRAPHICALITEM_H
