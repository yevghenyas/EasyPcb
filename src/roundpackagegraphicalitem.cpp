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

#include "roundpackagegraphicalitem.h"
#include "drawwrapper.h"

RoundPackageGraphicalItem::RoundPackageGraphicalItem(BOARD_LEVEL_ID idLevel,float x,float y,float d,float d1,
                                                     ITEMS_ORIENTATION o,ITEM_ID id):
                            GraphicalItem(idLevel,x,y,id),m_D(d),m_d(d1),m_startAngle(0),
                                                                              m_spanAngle(0),
                                                                              m_closed(false)
{
   m_type = o;
}

RoundPackageGraphicalItem::~RoundPackageGraphicalItem()
{

}

int RoundPackageGraphicalItem::prepareToChange(const QPoint& )
{

}

bool RoundPackageGraphicalItem::isAboveNode(QPoint& , int& )
{
   return false;
}


void RoundPackageGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,
                                                    int zoom,int zoom_d)
{
  float x1,y1;
  coordRelativeToAbsolute(x1,y1);
  if(m_startAngle != 0)
     DrawWrapper::drawArchedPackageF(p,c,x1 + difX/zoom * zoom_d,
                                     y1 + difY,m_D,m_startAngle,
                                     m_spanAngle,m_closed,zoom,zoom_d);
  else
     DrawWrapper::drawRoundPackageF(p,c,x1 + difX/zoom * zoom_d,
                                 y1 + difY,m_D/2,m_d/2,zoom,zoom_d);
}

void RoundPackageGraphicalItem::paintItem(QPainter& p, QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID idLevel)
{
  if(m_level == idLevel)  
     paintItemIntoBuffer(0,0,p,c,zoom,zoom_d); //in this case the buffer will be screen
}

bool RoundPackageGraphicalItem::isPointIn(int x, int y, BOARD_LEVEL_ID )
{
   float xf,yf;
   coordRelativeToAbsolute(xf,yf);

   int x1 = static_cast<int>(xf * PIXELS_PER_MM);
   int y1 = static_cast<int>(yf * PIXELS_PER_MM);
   int D = static_cast<int>(m_D * PIXELS_PER_MM);

   auto dd = std::hypot(x1 - x,y1 - y);

   return dd <= static_cast<float>(D)/2 && dd >= static_cast<float>(D) * 4/10;

}


bool RoundPackageGraphicalItem::isPointInF(float x, float y, BOARD_LEVEL_ID )
{
  float x1,y1;
  coordRelativeToAbsolute(x1,y1);
  float dd = std::hypotf(x1 - x,y1 - y);

  return dd <= m_D/2 && dd >= m_D * 4/10;
}

void RoundPackageGraphicalItem::rotate(BOARD_LEVEL_ID idLevel, int x, int y, int grad)
{
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
   if(m_startAngle != 0)
      m_startAngle -= grad/16;

   coordAbsoluteToRelative(x1,y1);

/*
   float x1,y1;
   if(idLevel != m_level && idLevel != LEVEL_ALL)
      return;

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

SmartPtr<GraphicalItem> RoundPackageGraphicalItem::clone()
{
   auto p = SmartPtr<GraphicalItem>::make_smartptr<RoundPackageGraphicalItem>(m_level,m_x,m_y,m_D,m_d,
                                                                m_type,IDsGenerator::instance()->getNewID());
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent,false);
   p->setType(m_type);
   static_cast<RoundPackageGraphicalItem*>(p.get())->setStartAngle(m_startAngle);
   static_cast<RoundPackageGraphicalItem*>(p.get())->setSpanAngle(m_spanAngle);
   static_cast<RoundPackageGraphicalItem*>(p.get())->setClosed(m_closed);
   return p;         
}

void RoundPackageGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setd(props.rsh_g.r_x);
   setd1(props.rsh_g.r_y);
   setStartAngle(props.rsh_g.a_st);
   setSpanAngle(props.rsh_g.a_sp);
   setClosed(props.rsh_g.closed);
}

void RoundPackageGraphicalItem::getConnectors(set<ITEM_ID>& )
{

}


void RoundPackageGraphicalItem::connect(ITEM_ID )
{

}

void RoundPackageGraphicalItem::disconnect(ITEM_ID )
{

}
bool RoundPackageGraphicalItem::isInArea(int x,int y)
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);
   float dd = std::hypotf(x1 - x,y1 - y);
   return dd <= m_D/2;
}

PointF RoundPackageGraphicalItem::getArea()
{
   PointF res(m_D,m_D);
   return res;
}

string RoundPackageGraphicalItem::getGerberString(GerberGenerator& gen,BOARD_LEVEL_ID idLevel)
{
   return gen.getStringForRoundPckg(getID(),x(),y(),d());
}
