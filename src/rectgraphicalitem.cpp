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

#include "rectgraphicalitem.h"
#include "drawwrapper.h"

RectGraphicalItem::RectGraphicalItem(float x,float y,float w,float h,BOARD_LEVEL_ID idLevel,ITEMS_ORIENTATION type,ITEM_ID id):
                                   GraphicalItem(idLevel,x,y,id),m_w(w),m_h(h)
{
   m_selected = false;
   m_type = type;
}

RectGraphicalItem::~RectGraphicalItem()
{

}

int RectGraphicalItem::prepareToChange(const QPoint& )
{

}

bool RectGraphicalItem::isAboveNode(QPoint& , int& )
{
  return false;
}


void RectGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,
                                            int zoom,int zoom_d)
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);
   DrawWrapper::drawRectPlateF(p,c,m_type,x1 + difX,
                                          y1 + difY,
                                           m_w,m_h,zoom,zoom_d);
/*   if(m_vcCons && m_vcCons->size() > 0)
   {
      //using virtual connector color to draw connector plates
      const QColor vc_color(VC_CON_COLOR);
      DrawWrapper::drawConnectorPlate(p,vc_color ,x1 + difX/k_zoom * k_zoom_d,
                                      y1 + difY/k_zoom * k_zoom_d,k_zoom);
   }
*/
}

void RectGraphicalItem::paintItem(QPainter& p, QColor& c,int zoom,int zoom_d, BOARD_LEVEL_ID idLevel)
{
   if(m_level == idLevel)
   {
      float x1,y1;
      coordRelativeToAbsolute(x1,y1);   
      DrawWrapper::drawRectPlateF(p,c,m_type,x1,y1,m_w,m_h,zoom,zoom_d);
/*      if(m_vcCons && m_vcCons->size() > 0)
      {
         //using virtual connector color to draw connector plates
         const QColor vc_color(VC_CON_COLOR);
         DrawWrapper::drawConnectorPlate(p,vc_color ,x1,y1,k_zoom);
      }
*/
   }
}

bool RectGraphicalItem::isPointIn(int x, int y, BOARD_LEVEL_ID idLevel)
{
   float xf,yf;
   coordRelativeToAbsolute(xf,yf);
   auto x1 = static_cast<int>(xf * PIXELS_PER_MM);
   auto y1 = static_cast<int>(yf * PIXELS_PER_MM);
   auto w = static_cast<int>(m_w * PIXELS_PER_MM);
   auto h = static_cast<int>(m_h * PIXELS_PER_MM);
   //in case it is 0 as a result of cast float to int when < 1,set it to 1
   auto w_d2 = w/2 > 0 ? w/2 : 1;
   auto h_d2 = h/2 > 0 ? h/2 : 1;
   if(m_type == O_VERTICAL_TOP || m_type == O_VERTICAL_BOTTOM)
       return (x > (x1 - w_d2) &&
                x < (x1 + w_d2) &&
                y > (y1 - h_d2) &&
                y < (y1 + h_d2));
   else
       return (x > (x1 - h_d2) &&
                x < (x1 + h_d2) &&
                y > (y1 - w_d2) &&
                y < (y1 + w_d2));

}

bool RectGraphicalItem::isPointInF(float x, float y, BOARD_LEVEL_ID idLevel)
{
   float x1,y1;
   coordRelativeToAbsolute(x1,y1);
   if(m_type == O_VERTICAL_TOP || m_type == O_VERTICAL_BOTTOM)
       return ((x > (x1 - m_w/2) &&
               x < (x1 + m_w/2) &&
               y > (y1 - m_h/2) &&
               y < (y1 + m_h/2)));
   else
       return ((x > (x1 - m_h/2) &&
               x < (x1 + m_h/2) &&
               y > (y1 - m_w/2) &&
               y < (y1 + m_w/2)));



/*
   return (x > (x1 - m_w/2) * k_zoom/k_zoom_d && 
           x < (x1 + m_w/2) * k_zoom/k_zoom_d && 
           y > (y1 - m_h/2) * k_zoom/k_zoom_d && 
           y < (y1 + m_h/2) * k_zoom/k_zoom_d);
*/   
}

void RectGraphicalItem::rotate(BOARD_LEVEL_ID idLevel, int x, int y, int grad)
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

SmartPtr<GraphicalItem> RectGraphicalItem::clone()
{
   PointF pt(m_x,m_y);
   auto p = SmartPtr<GraphicalItem>::make_smartptr<RectGraphicalItem>(m_x,m_y,m_w,m_h,m_level,
                                       m_type,IDsGenerator::instance()->getNewID());
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent,false);
   p->setType(m_type);
   return p;   
}

void RectGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setWidth(props.sqw_g.width);
   setHeight(props.sqw_g.height);
}

GraphicalItem* RectGraphicalItem::isConnectable(PointF& pt)
{
//   cout<<"pt x="<<pt.x()<<"y="<<pt.y()<<"m_x="<<m_x<<"m_y="<<m_y<<endl;
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

void RectGraphicalItem::connect(ITEM_ID connectorID)
{
   if(!m_vcCons)
      m_vcCons.reset(new unordered_set<ITEM_ID>);

   m_vcCons->insert(connectorID);
}


void RectGraphicalItem::getConnectors(set<ITEM_ID>& set)
{
   if(!m_vcCons.get())
      return;
   for(auto id:*m_vcCons.get())
     set.insert(id);

}

int RectGraphicalItem::getConnectorsNumber()
{
   if(!m_vcCons.get())
      return 0;
   else
      return m_vcCons->size();

}

ITEM_ID RectGraphicalItem::isRectOccupied(PointF& top,PointF& bottom)
{
   auto x1 = top.x();
   auto y1 = bottom.y();
   auto x2 = bottom.x();
   auto y2 = top.y();
   auto x3 = (top.x() + bottom.x())/2;
   auto y3 = (top.y() + bottom.y())/2;

   if(isPointInF(top.x(),top.y(),LEVEL_ALL) ||
      isPointInF(bottom.x(),bottom.y(),LEVEL_ALL) ||
      isPointInF(x1,y1,LEVEL_ALL) ||
      isPointInF(x2,y2,LEVEL_ALL) ||
      isPointInF(x3,y3,LEVEL_ALL) ||
      isPointInF(x3,y3,LEVEL_ALL))
   {

//       cout<<"for id="<<m_id<<" occupied"<<endl;
       return m_id;
   }
   else {
       return ID_NONE;
   }
}

void RectGraphicalItem::disconnect(ITEM_ID id)
{
   if(m_vcCons.get() != nullptr)
      m_vcCons->erase(id);
}

bool RectGraphicalItem::isInArea(int x,int y)
{
   return isPointIn(x,y,LEVEL_ALL);
}

PointF RectGraphicalItem::getArea()
{

   PointF res;
   switch (m_type) {
      case O_HORIZONTAL_LEFT:
      case O_HORIZONTAL_RIGHT:
         res.setX(m_h);
         res.setY(m_w);
         break;
      default:
         res.setX(m_w);
         res.setY(m_h);
         break;
   }
   return res;
}

string RectGraphicalItem::getGerberString(GerberGenerator& gen,
                                          BOARD_LEVEL_ID )
{
   if(m_type == O_HORIZONTAL_LEFT || m_type == O_HORIZONTAL_RIGHT)
      return gen.getStringForRect(getID(),x(),y(),height(),width());
   else
      return gen.getStringForRect(getID(),x(),y(),width(),height());
}
