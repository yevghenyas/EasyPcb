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

#ifndef ROUNDPACKAGEGRAPHICALITEM_H
#define ROUNDPACKAGEGRAPHICALITEM_H

#include "graphicalitem.h"

class RoundPackageGraphicalItem :  public GraphicalItem
{
    float m_D; //diameter of package
    float m_d; //second diameter ( for ellipse )
    int m_startAngle;
    int m_spanAngle;//<360 degrees will be arc
    bool m_closed;//only for arc(the ends of arc may be connected with the line)
public:
    RoundPackageGraphicalItem(BOARD_LEVEL_ID idLevel,float x,float y,float d,float d1,ITEMS_ORIENTATION o,ITEM_ID id);
    virtual ~RoundPackageGraphicalItem() override;
    virtual int prepareToChange(const QPoint& p)  override;
    virtual bool isAboveNode(QPoint& p, int& index) override;
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d) override;
    virtual void paintItem(QPainter& p, QColor& c,int zoom,int zoom_d, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointIn(int x, int y, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointInF(float x, float y, BOARD_LEVEL_ID idLevel) override;
    virtual void rotate(BOARD_LEVEL_ID idLevel, int x, int y, int grad) override;
    virtual PointF getPos() override {return PointF(m_x,m_y);}
    float d(){return m_D;}
    float d1(){return m_d;}
    void setd(float d){m_D = d;}
    void setd1(float d){m_d = d;}
    int getSpanAngle(){return m_spanAngle;}
    void setSpanAngle(int a){m_spanAngle = a;}
    int getStartAngle(){return m_startAngle;}
    void setStartAngle(int a){m_startAngle = a;}
    void setClosed(bool closed){m_closed = closed;}
    bool isClosed(){return m_closed;}
    virtual SmartPtr<GraphicalItem> clone() override;
    virtual void setGeometry(GeomCommonProps& props) override;
    virtual GeomCommonProps getGeometry() override {return GeomCommonProps(m_D);}
    virtual void connect(ITEM_ID connectorID) override;
    virtual void disconnect(ITEM_ID connectorID) override;
    virtual void getConnectors(set<ITEM_ID>& connectors) override;
    int getConnectorsNumber() override {return 0;}
    virtual ITEM_ID isRectOccupied(PointF& ,PointF& ) override {return ID_NONE;}
    virtual bool isInArea(int x,int y) override;
    virtual PointF getArea() override;
    virtual bool isGerberSupported() override {return true;}
    virtual string getGerberString(GerberGenerator& gen,BOARD_LEVEL_ID idLevel) override;

};

#endif // ROUNDPACKAGEGRAPHICALITEM_H
