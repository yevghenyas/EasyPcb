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

#ifndef RECTPACKAGEGRAPHICALITEM_H
#define RECTPACKAGEGRAPHICALITEM_H

#include "graphicalitem.h"
#include <QPoint>

class RectPackageGraphicalItem :  public GraphicalItem
{
   float m_width;
   float m_height;
   bool filled;
public:
    RectPackageGraphicalItem(BOARD_LEVEL_ID idLevel,PointF p,float width,float height,
                             ITEMS_ORIENTATION type,ITEM_ID id,bool fill = false);
    virtual ~RectPackageGraphicalItem() override;
    virtual int prepareToChange(const QPoint& p) override;
    virtual bool isAboveNode(QPoint& p, int& index) override;
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d) override;
    virtual void paintItem(QPainter& p, QColor& c,int zoom,int zoom_d, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointIn(int x, int y, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointInF(float x, float y, BOARD_LEVEL_ID idLevel) override;
    virtual void rotate(BOARD_LEVEL_ID idLevel, int x, int y, int grad) override;
    virtual PointF getPos() override{return PointF(m_x,m_y);}
    float width(){return m_width;}
    float height(){return m_height;}
    void setWidth(float w){m_width = w;}
    void setHeight(float h){m_height = h;}
    virtual SmartPtr<GraphicalItem> clone() override;
    virtual void setGeometry(GeomCommonProps& props) override;
    virtual GeomCommonProps getGeometry() override {return GeomCommonProps(m_height,m_width);}
    virtual void connect(ITEM_ID connectorID) override;
    virtual void disconnect(ITEM_ID connectorID) override;
    virtual void getConnectors(set<ITEM_ID>& connectors) override;
    int getConnectorsNumber() override {return 0;}
    virtual ITEM_ID isRectOccupied(PointF& ,PointF& ) override {return ID_NONE;}
    virtual bool isInArea(int x,int y) override;
    virtual PointF getArea() override;
    bool isFilled(){return filled;}
    virtual bool isGerberSupported() override {return true;}
    virtual string getGerberString(GerberGenerator& gen,BOARD_LEVEL_ID idLevel) override;
};

#endif // RECTPACKAGEGRAPHICALITEM_H
