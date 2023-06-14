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
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.#asm
    MOVLW 0
            MOVFF WREG, ANSELD
            MOVFF WREG, ANSELE
    MOVLW 0x0F
            MOVFF WREG, ANSELA
#endasm

 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#include "capgraphicalitem.h"
#include "drawwrapper.h"

CapGraphicalItem::CapGraphicalItem(float x,float y,float f,BOARD_LEVEL_ID idLevel,ITEM_ID id)
                     :GraphicalItem(idLevel,x,y,id),m_f(f)
{
   m_type = O_HORIZONTAL_LEFT;
   m_selected = false;
}

CapGraphicalItem::~CapGraphicalItem()
{

}

int CapGraphicalItem::prepareToChange(const QPoint& )
{

}

bool CapGraphicalItem::isAboveNode(QPoint& , int& )
{
   return false;
}


void CapGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d)
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);   
   DrawWrapper::DrawCapF(p,c,m_type,x1 + difX/zoom * zoom_d,
                                    y1 + difY/zoom * zoom_d,
                                    m_f,zoom,zoom_d);
}

void CapGraphicalItem::paintItem(QPainter& p, QColor& c, int zoom,int zoom_d,BOARD_LEVEL_ID idLevel)
{
   if(m_level == idLevel)
   {
      float x1,y1;
      coordRelativeToAbsolute(x1,y1);
      DrawWrapper::DrawCapF(p,c,m_type,x1,y1,m_f,zoom);
   }
}

bool CapGraphicalItem::isPointIn(int x, int y, BOARD_LEVEL_ID )
{
    float xf,yf;
    coordRelativeToAbsolute(xf,yf);
    int x1 = static_cast<int>(xf * PIXELS_PER_MM);
    int y1 = static_cast<int>(yf * PIXELS_PER_MM);
    int f = static_cast<int>(m_f * PIXELS_PER_MM);
    if(m_type == O_HORIZONTAL_RIGHT || m_type == O_HORIZONTAL_LEFT)
       return x > x1 - f/8 && y > y1 -f/2 && x < x1 + f/8 && y < y1 + f/2;
    else
       return x > x1 - f/2 && y > y1 -f/8 && x < x1 + f/2 && y < y1 + f/8;
}

bool CapGraphicalItem::isPointInF(float x, float y, BOARD_LEVEL_ID )
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);
   if(m_type == O_HORIZONTAL_RIGHT || m_type == O_HORIZONTAL_LEFT)
      return x > x1 - m_f/8 && y > y1 -m_f/2 && x < x1 + m_f/8 && y < y1 + m_f/2;
   else
      return x > x1 - m_f/2 && y > y1 -m_f/8 && x < x1 + m_f/2 && y < y1 + m_f/8;
}

void CapGraphicalItem::rotate(BOARD_LEVEL_ID idLevel,int x,int y,int grad)
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

   rotateRectShape(center,x1,y1,grad,m_type);
   coordAbsoluteToRelative(x1,y1);

/*
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);   
   int x1i = castFloatToInt(x1 * PIXELS_PER_MM);
   int y1i = castFloatToInt(y1 * PIXELS_PER_MM);
   if(x == -1)
      x = x1i;
   if(y == -1)
      y = y1i;

   QPoint center(x,y);
   rotateRectShape(center,x1i,y1i,grad,m_type);
   x1 = x1i/PIXELS_PER_MM;
   y1 = y1i/PIXELS_PER_MM;
   coordAbsoluteToRelative(x1,y1);
*/
   
}

SmartPtr<GraphicalItem> CapGraphicalItem::clone()
{
//   CapGraphicalItem *p = new CapGraphicalItem(m_x,m_y,m_f,m_level,k_zoom,k_zoom_d,
//                                             IDsGenerator::instance()->getNewID());
   SmartPtr<GraphicalItem> p =
           SmartPtr<GraphicalItem>::make_smartptr<CapGraphicalItem>(m_x,m_y,m_f,m_level,
                                                                    IDsGenerator::instance()->getNewID());
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent);
   p->setType(m_type);   
   return p;      
}


void CapGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setf(props.cap_g.width);
}

void CapGraphicalItem::getConnectors(set<ITEM_ID>& )
{

}

void CapGraphicalItem::connect(ITEM_ID )
{

}

void CapGraphicalItem::disconnect(ITEM_ID )
{

}

bool CapGraphicalItem::isInArea(int x,int y)
{
   return isPointIn(x,y,BOARD_LEVEL_ID::LEVEL_ALL);
}

PointF CapGraphicalItem::getArea()
{
   PointF res(-1,-1);
   return res;
}
