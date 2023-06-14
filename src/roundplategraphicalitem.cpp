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

#include <cstdio>
#include <iostream>
#include <cmath>
#include <QPainterPath>
#include "common.h"
#include "roundplategraphicalitem.h"
#include "drawwrapper.h"
#include <math.h>

using namespace std;

RoundPlateGraphicalItem::RoundPlateGraphicalItem(float x,float y,float d1,float d2,BOARD_LEVEL_ID level,ITEM_ID id):
    GraphicalItem(level,x,y,id),m_d(d2),m_D(d1)
{
   //add code here
   m_bVisible = true;
}



RoundPlateGraphicalItem::~RoundPlateGraphicalItem()
{
   
}


void RoundPlateGraphicalItem::paintItem(QPainter& p, QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID)
{
   if(m_bVisible)
   {
//      char buf[64];
      float x,y;
      coordRelativeToAbsolute(x,y);
      PointF pointf(x,y);
//      sprintf(buf,"paint x=%f y=%f",x,y);
      paintInternal(p,c,pointf,zoom,zoom_d);
   }
}

bool RoundPlateGraphicalItem::isPointIn(int xx, int yy, BOARD_LEVEL_ID idLevel)
{
   float xf,yf;
   coordRelativeToAbsolute(xf,yf);

   int x = static_cast<int>(xf * PIXELS_PER_MM);
   int y = static_cast<int>(yf * PIXELS_PER_MM);
   int D = static_cast<int>(m_D * PIXELS_PER_MM);
   int d = static_cast<int>(m_d * PIXELS_PER_MM);
   float dd = std::hypotf(x - xx,y - yy);
/*
   float dif_x = x - xx;
   float dif_y = y - yy;
   float dd = sqrt(dif_x * dif_x + dif_y * dif_y);
*/
   return dd <= static_cast<float>(D)/2 && dd > static_cast<float>(d)/2;
}


bool RoundPlateGraphicalItem::isPointInF(float xx, float yy, BOARD_LEVEL_ID )
{
  float x,y;
  coordRelativeToAbsolute(x,y);
  float dd = std::hypotf(x - xx,y - yy);
/*
  float dif_x = x - xx;
  float dif_y = y - yy;
  float dd = sqrt(dif_x * dif_x + dif_y * dif_y);
*/
  return dd <= m_D/2 && dd > m_d/2;  
}


void RoundPlateGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,
                                                  int zoom,int zoom_d)
{
//   char buf[64];
   float x,y;
   coordRelativeToAbsolute(x,y);
   PointF pointf(x + difX,y + difY);
//   sprintf(buf,"paint x=%d y=%d",x,y);
   paintInternal(p,c,pointf,zoom,zoom_d);
}
void RoundPlateGraphicalItem::setX(float x)
{
   m_x = x;
}
void RoundPlateGraphicalItem::setY(float y)
{
   m_y = y;
}
void RoundPlateGraphicalItem::paintInternal(QPainter& painter, QColor& c,PointF& point,int zoom,int zoom_d)
{
   DrawWrapper::drawPlateF(painter,c,point.x(),point.y(),m_D,m_d,zoom,zoom_d);
   if(m_vcCons)
   {
      //using virtual connector color to draw connector plates
//      const QColor vc_color(VC_CON_COLOR);
//      DrawWrapper::drawConnectorPlate(painter,vc_color ,point.x(),point.y(),k_zoom);
   }

}


bool RoundPlateGraphicalItem::isAboveNode(QPoint& ,int& )
{
   return false;
}

void RoundPlateGraphicalItem::rotate(BOARD_LEVEL_ID idLevel,int x, int y,int grad)
{
   //x,y - center around which we should rotate
   // -1 means that the center of this item is the center of rotation
   if(idLevel != m_level && idLevel != LEVEL_ALL)
      return;

   float x1,y1;
   coordRelativeToAbsolute(x1,y1);
   PointF center(static_cast<float>(x)/PIXELS_PER_MM,
                  static_cast<float>(y)/PIXELS_PER_MM);
   if(x == -1)
      center.setX(x1);
   if(y == -1)
      center.setY(y1);

   rotateRoundShape(center,x1,y1,grad);
   coordAbsoluteToRelative(x1,y1);

/*
   float x1,y1;
   if(idLevel != m_level && idLevel != LEVEL_ALL)
      return;

 
   //updated with the appearance of parent
   coordRelativeToAbsolute(x1,y1);
   int x1i = castFloatToInt(x1 * PIXELS_PER_MM);
   int y1i = castFloatToInt(y1 * PIXELS_PER_MM);
   if(x == -1)
      x = x1i;
   if(y == -1)
      y = y1i;
   
   QPoint center(x,y);
   rotateRoundShape(center,x1i,y1i,grad);
   x1 = x1i/PIXELS_PER_MM;
   y1 = y1i/PIXELS_PER_MM;
   coordAbsoluteToRelative(x1,y1);
*/
   
}

SmartPtr<GraphicalItem> RoundPlateGraphicalItem::clone()
{
   auto p = SmartPtr<GraphicalItem>::make_smartptr<RoundPlateGraphicalItem>(m_x,m_y,m_D,m_d,m_level,
                                                            IDsGenerator::instance()->getNewID());
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent,false);
   p->setType(m_type);
   return p;   
}

void RoundPlateGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setd(props.rpl_g.d_ex);
   setd1(props.rpl_g.d_in);
}

GraphicalItem* RoundPlateGraphicalItem::isConnectable(PointF &pt)
{
   if (isPointInF(pt.x(),pt.y(),LEVEL_ALL))
   {
      float x1,y1;
      coordRelativeToAbsolute(x1,y1);
      pt.setX(x1);
      pt.setY(y1);
      return this;
   }
   return nullptr;
}


void RoundPlateGraphicalItem::connect(ITEM_ID connectorID)
{
   if(!m_vcCons)
      m_vcCons.reset(new unordered_set<ITEM_ID>);

   m_vcCons->insert(connectorID);

}

void RoundPlateGraphicalItem::disconnect(ITEM_ID id)
{
   if(m_vcCons.get() != nullptr)
      m_vcCons->erase(id);
}


void RoundPlateGraphicalItem::getConnectors(set<ITEM_ID>& set)
{
   if(!m_vcCons.get())
      return;
   for(auto id:*m_vcCons.get())
     set.insert(id);
}

ITEM_ID RoundPlateGraphicalItem::isRectOccupied(PointF& top,PointF& bottom)
{
    float x,y;
    coordRelativeToAbsolute(x,y);
    float d = m_D;
    auto l = [&d,&x,&y](float xx,float yy) {
        float dif_x = x - xx;
        float dif_y = y - yy;
        float dd = sqrt(dif_x * dif_x + dif_y * dif_y);
        return dd <= d/2;
    };
    auto x1 = top.x();
    auto y1 = bottom.y();
    auto x2 = bottom.x();
    auto y2 = top.y();
    auto x3 = (top.x() + bottom.x())/2;
    auto y3 = (top.y() + bottom.y())/2;
    if(l(top.x(),top.y()) ||
       l(bottom.x(),bottom.y()) ||
       l(x1,y1) ||
       l(x2,y2) ||
       l(x3,y3))
    {
//        cout<<"for id="<<m_id<<" occupied"<<endl;
        return m_id;
    }
    else {
        return ID_NONE;
    }
}

bool RoundPlateGraphicalItem::isInArea(int xx,int yy)
{
    float xf,yf;
    coordRelativeToAbsolute(xf,yf);

    int x = static_cast<int>(xf * PIXELS_PER_MM);
    int y = static_cast<int>(yf * PIXELS_PER_MM);
    int D = static_cast<int>(m_D * PIXELS_PER_MM);
    int d = static_cast<int>(m_d * PIXELS_PER_MM);
    float dd = std::hypotf(x - xx,y - yy);

    return dd <= static_cast<float>(D)/2;
}

PointF RoundPlateGraphicalItem::getArea()
{
   PointF res(m_D,m_D);
   return res;
}

string RoundPlateGraphicalItem::getGerberString(GerberGenerator& gen,
                                                BOARD_LEVEL_ID )
{
   return gen.getStringForRoundPlate(getID(),x(),y(),d(),d1());
}
