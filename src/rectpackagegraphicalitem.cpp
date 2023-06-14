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

#include "rectpackagegraphicalitem.h"
#include "drawwrapper.h"

RectPackageGraphicalItem::RectPackageGraphicalItem(BOARD_LEVEL_ID idLevel,
                                           PointF p,float width,float height,
                                           ITEMS_ORIENTATION type,ITEM_ID id,bool fill)
    :GraphicalItem(idLevel,p.x(),p.y(),id),m_width(width),m_height(height),filled(fill)
{
   m_type = type;
   m_selected = false;
}

RectPackageGraphicalItem::~RectPackageGraphicalItem()
{

}

int RectPackageGraphicalItem::prepareToChange(const QPoint& )
{

}

bool RectPackageGraphicalItem::isAboveNode(QPoint& , int& )
{
   return false;
}




void RectPackageGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,
                                                   int zoom,int zoom_d)
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);
   if(filled)
      DrawWrapper::drawFilledRectPackageF(p,c,m_type,x1 + difX,
                                  y1 + difY,
                                  m_width,m_height,zoom,zoom_d);
   else
      DrawWrapper::drawRectPackageF(p,c,m_type,x1 + difX,
                                 y1 + difY,
                                 m_width,m_height,zoom,zoom_d);
}

void RectPackageGraphicalItem::paintItem(QPainter& p, QColor& c,int zoom,int zoom_d, BOARD_LEVEL_ID idLevel)
{
   if(idLevel == m_level)
   {
      float x1,y1;
      coordRelativeToAbsolute(x1,y1);
      if(filled)
         DrawWrapper::drawFilledRectPackageF(p,c,m_type,x1,y1,m_width,m_height,zoom,zoom_d);
      else
         DrawWrapper::drawRectPackageF(p,c,m_type,x1,y1,m_width,m_height,zoom,zoom_d);
   }
}

bool RectPackageGraphicalItem::isPointIn(int xx, int yy, BOARD_LEVEL_ID)
{
   float xf,yf;
   coordRelativeToAbsolute(xf,yf);

   int x1 = static_cast<int>(xf * PIXELS_PER_MM);
   int y1 = static_cast<int>(yf * PIXELS_PER_MM);
   int w = static_cast<int>(m_width * PIXELS_PER_MM);
   int h = static_cast<int>(m_height * PIXELS_PER_MM);

    w = w/2 ? w : 2;
    h = h/2 ? h : 2;
    bool bHor = m_type == O_HORIZONTAL_LEFT || m_type == O_HORIZONTAL_RIGHT;
    auto dHor = bHor ? w/2 : h/2;
    auto dVert = bHor ? h/2 : w/2;

    return xx > x1 - dHor && xx < x1 + dHor && yy > y1 - dVert && yy < y1 + dVert &&
            (xx > x1 + dHor/5*4 || xx < x1 - dHor/5*4 ||
             yy > y1 + dVert/5*4 || yy < y1 - dVert/5*4);
}

bool RectPackageGraphicalItem::isPointInF(float xx, float yy, BOARD_LEVEL_ID )
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);   
   bool bHor = m_type == O_HORIZONTAL_LEFT || m_type == O_HORIZONTAL_RIGHT;
   auto dHor = bHor ? m_width/2 : m_height/2;
   auto dVert = bHor ? m_height/2 : m_width/2;

   return xx > x1 - dHor && xx < x1 + dHor && yy > y1 - dVert && yy < y1 + dVert &&
           (xx > x1 + dHor/5*4 || xx < x1 - dHor/5*4 ||
            yy > y1 + dVert/5*4 || yy < y1 - dVert/5*4);



}

void RectPackageGraphicalItem::rotate(BOARD_LEVEL_ID idLevel, int x, int y, int grad)
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

SmartPtr<GraphicalItem> RectPackageGraphicalItem::clone()
{
   PointF pt(m_x,m_y);
   auto p = SmartPtr<GraphicalItem>::make_smartptr<RectPackageGraphicalItem>(m_level,pt,m_width,m_height,m_type,
                                                              IDsGenerator::instance()->getNewID(),filled);
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent,false);
   p->setType(m_type);
   return p;   
}

void RectPackageGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setWidth(props.sqw_g.width);
   setHeight(props.sqw_g.height);
}

void RectPackageGraphicalItem::getConnectors(set<ITEM_ID>& )
{

}

void RectPackageGraphicalItem::connect(ITEM_ID )
{

}

void RectPackageGraphicalItem::disconnect(ITEM_ID )
{

}

bool RectPackageGraphicalItem::isInArea(int x,int y)
{
   float xf,yf;
   coordRelativeToAbsolute(xf,yf);

   int x1 = static_cast<int>(xf * PIXELS_PER_MM);
   int y1 = static_cast<int>(yf * PIXELS_PER_MM);
   int w = static_cast<int>(m_width * PIXELS_PER_MM);
   int h = static_cast<int>(m_height * PIXELS_PER_MM);

   if(m_type == O_HORIZONTAL_LEFT || m_type == O_HORIZONTAL_RIGHT)
   {
      return x > (x1 - w/2) && y > (y1 - h/2) &&
             x < (x1 + w/2) && y < (y1 + h/2);
   }
   else
   {
      return x > (x1 - h/2) && y > (y1 - w/2) &&
             x < (x1 + h/2) && y < (y1 + w/2);
   }
}

PointF RectPackageGraphicalItem::getArea()
{
   PointF res(m_width,m_height);
   return res;
}

string RectPackageGraphicalItem::getGerberString(GerberGenerator& gen,
                                          BOARD_LEVEL_ID )
{
   if(m_type == ITEMS_ORIENTATION::O_VERTICAL_BOTTOM ||
           m_type == ITEMS_ORIENTATION::O_VERTICAL_TOP)
      return gen.getStringForRectPckg(getID(),x(),y(),width(),height(),filled);
   else
      return gen.getStringForRectPckg(getID(),x(),y(),height(),width(),filled);
}
