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

#include "linegraphicalitem.h"
#include <algorithm>
#include <iostream>
#include "drawwrapper.h"
#include "graphicalitemscontainer.h"
/*
using VectorInd = std::vector<PointF>::size_type;

LineGraphicalItem::LineGraphicalItem(vector<PointF>&& points,float w,BOARD_LEVEL_ID level,int k,int k1,ITEM_ID id):
    GraphicalItem(level,k,k1,0,0,id),m_points(points),m_w(w),m_style(LINE_STYLE::LINE_ROUNDED)
{
   k_zoom = k;
   m_bVisible = true;
   m_x = 0;
   m_y = 0;
}

LineGraphicalItem::LineGraphicalItem(vector<QPoint>& points,float w,BOARD_LEVEL_ID level,int k,int k1,ITEM_ID id):
    GraphicalItem(level,k,k1,0,0,id),m_w(w),m_style(LINE_STYLE::LINE_ROUNDED)
{
   k_zoom = k;
   m_points.reserve(points.size());
   for (std::vector<QPoint>::size_type i = 0; i < points.size(); ++i)
   {
      PointF pt(points[i].x() * k/PIXELS_PER_MM/k1,points[i].y() * k/PIXELS_PER_MM/k1);
      m_points.push_back(pt);
   }
   m_bVisible = true;
   m_x = 0;
   m_y = 0;
}

LineGraphicalItem::LineGraphicalItem(vector<VcConCunstrInfo>& points,float w,
                                     BOARD_LEVEL_ID level,int k,int k1,ITEM_ID id):
                                     GraphicalItem(level,k,k1,0,0,id),m_w(w),m_style(LINE_STYLE::LINE_ROUNDED)
{
    k_zoom = k;
    m_points.reserve(points.size());
    for (std::vector<QPoint>::size_type i = 0; i < points.size(); ++i)
    {
       PointF pt(points[i].m_pt.x()/k1,points[i].m_pt.y()/k1);
       m_points.push_back(pt);
       if(points[i].isConnected())
       {
          connectorAdd(points[i].m_id,points[i].m_pt);
       }
    }
    m_bVisible = true;
    m_x = 0;
    m_y = 0;

}
LineGraphicalItem::~LineGraphicalItem()
{

}

void LineGraphicalItem::zoom(int )
{
   k_zoom <<= 1;
}

void LineGraphicalItem::paintInternal(float difX,float difY,QPainter& p, QColor& c)
{
    PointF ptf(0.0f,0.0f);
    if(m_parent)
    {
       ptf.setX(m_parent->x());
       ptf.setY(m_parent->y());
    }
    ptf.setX(ptf.x() + difX/k_zoom * k_zoom_d);
    ptf.setY(ptf.y() + difY/k_zoom * k_zoom_d);
    DrawWrapper::drawLine(p,c,m_points,&ptf,m_selected,
                          m_w,m_style,k_zoom,k_zoom_d);
    //draw electrical connector points if any
    if(m_vcCons)
    {
       for(auto item:*m_vcCons)
       {
          float x = m_points[static_cast<VectorInd>(item.first)].x();
          float y = m_points[static_cast<VectorInd>(item.first)].y();
          if(m_parent)
          {
             x = m_parent->x() + x;
             y = m_parent->y() + y;
          }
          DrawWrapper::drawConnectorPlate(p,virtConColor,x,y,k_zoom );
       }
    }
}

void LineGraphicalItem::paintItem(QPainter& p, QColor& c,BOARD_LEVEL_ID )
{
   if(m_bVisible)
   {
       paintInternal(0,0,p,c);
   }
}

bool LineGraphicalItem::isPointIn(int x, int y, BOARD_LEVEL_ID idLevel)
{
    for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
    {
       int xx,yy,xx1,yy1;
       if(i == 0)
       {
          if(m_parent)
          {
             xx = static_cast<int>((m_points[i].x() + m_parent->x()) * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i].y() + m_parent->y() * PIXELS_PER_MM);
          }
          else
          {
             xx = static_cast<int>(m_points[i].x() * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i].y() * PIXELS_PER_MM);
          }
          if (x == xx && y == yy)
             return true;
       }
       if (i > 0)
       {
          if(m_parent)
          {
             xx = static_cast<int>(m_points[i - 1].x() + m_parent->x() * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i - 1].y() + m_parent->y() * PIXELS_PER_MM);
             xx1 = static_cast<int>(m_points[i].x() + m_parent->x() * PIXELS_PER_MM);
             yy1 = static_cast<int>(m_points[i].y() + m_parent->y() * PIXELS_PER_MM);
          }
          else
          {
             xx = static_cast<int>(m_points[i - 1].x() * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i - 1].y() * PIXELS_PER_MM);
             xx1 = static_cast<int>(m_points[i].x() * PIXELS_PER_MM);
             yy1 = static_cast<int>(m_points[i].y() * PIXELS_PER_MM);
          }
          int ww = static_cast<int>((m_w * PIXELS_PER_MM)/2);
          //parallel case
          if (abs(xx1 - xx) < 2 &&
             x >= std::min(xx1, xx) - ww &&
             x <= std::max(xx1, xx) + ww &&
             y >= std::min(yy1, yy) &&
             y <= std::max(yy1, yy))
             return true;

          //perpendicular case
          if (abs(yy1 - yy) < 2 &&
             y >= std::min(yy1, yy) - ww &&
             y <= std::max(yy1, yy) + ww &&
             x >= std::min(xx1, xx) &&
             x <= std::max(xx1, xx))
             return true;

              //check if the point on the line determined by the two points
          if (x >= std::min(xx1, xx) &&
              x <= std::max(xx1, xx) &&
              y >= std::min(yy1,yy) &&
              y <= std::max(yy1,yy))
          {
             float dif_x = abs(static_cast<float>(xx1) - static_cast<float>(xx));
             float dif_y = abs(static_cast<float>(yy1) - static_cast<float>(yy));
             float tg = dif_y/dif_x;
             float cos = dif_x/std::sqrt(dif_x * dif_x + dif_y * dif_y);
  //           sprintf(buf,"tg=%f",tg);
  //           cout<<buf<<endl;
             //point on the line that lies with the x = point.x
             int y_temp = 0;

             if((xx > xx1 && yy > yy1) || (xx1 >xx && yy > yy1))
                y_temp = static_cast<int>(yy - (static_cast<float>(abs(x - xx))) * tg);
             else
                y_temp = static_cast<int>(yy + (static_cast<float>(abs(x - xx))) * tg);

             ww /= cos;
  //           sprintf(buf,"Inside %d %f",y_temp,ww);
  //           cout<<buf<<endl;
             if(y <= y_temp +  ww && y >= y_temp - ww)
                return true;
          }
          else
          {
  //           sprintf(buf,"minx=%d,maxx=%d,miny=%d,maxy=%d",std::min(xx1, xx),std::max(xx1, xx),std::min(yy1,yy),std::max(yy1,yy));
  //           cout<<buf<<endl;
          }
       }
     }
     return false;
}

bool LineGraphicalItem::isPointInF(float x, float y, BOARD_LEVEL_ID idLevel)
{
  for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
  {
     float xx,yy,xx1,yy1;
     if(i == 0)
     {
        if(m_parent != nullptr)
        {
           xx = m_points[i].x() + m_parent->x();
           yy = m_points[i].y() + m_parent->y();
        }
        else
        {
           xx = m_points[i].x();
           yy = m_points[i].y();
        }
     }
     if (i > 0)
     {
        if(m_parent != nullptr)
        {
           xx = m_points[i - 1].x() + m_parent->x();
           yy = m_points[i - 1].y() + m_parent->y();
           xx1 = m_points[i].x() + m_parent->x();
           yy1 = m_points[i].y() + m_parent->y();
        }
        else
        {
           xx = m_points[i - 1].x();
           yy = m_points[i - 1].y();
           xx1 = m_points[i].x();
           yy1 = m_points[i].y();
        }
        float ww = m_w;
//        char buf[128];
//        sprintf(buf,"x=%d,y=%d,xx1=%d,xx=%d,yy1=%d,yy=%d",x,y,xx1,xx,yy1,yy);
//        cout<<buf<<endl;
        //parallel case
        if (abs(xx1 - xx) < 2 &&
           x >= std::min(xx1, xx) - ww &&
           x <= std::max(xx1, xx) + ww &&
           y >= std::min(yy1, yy) &&
           y <= std::max(yy1, yy))
           return true;

        //perpendicular case
        if (abs(yy1 - yy) < 2 &&
           y >= std::min(yy1, yy) - ww &&
           y <= std::max(yy1, yy) + ww &&
           x >= std::min(xx1, xx) &&
           x <= std::max(xx1, xx))
           return true;
        
            //check if the point in the plate determined by the two points
        if (x >= std::min(xx1, xx) &&
            x <= std::max(xx1, xx) &&
            y >= std::min(yy1,yy) &&
            y <= std::max(yy1,yy))
        {
//           cout<<"Here"<<endl;
           float dif_x = abs(xx1 - xx);
           float dif_y = abs(yy1 - yy);
           float tg = dif_y/dif_x;
           float cos = dif_x/std::sqrt(dif_x * dif_x + dif_y * dif_y);
//           sprintf(buf,"tg=%f",tg);
//           cout<<buf<<endl;
           //point on the line that lies with the x = point.x
           float y_temp = 0;
           
           if((xx > xx1 && yy > yy1) || (xx1 >xx && yy > yy1))
              y_temp = yy - (abs(x - xx)) * tg;
           else
              y_temp = yy + (abs(x - xx)) * tg;
           
           ww = m_w/cos;
//           sprintf(buf,"Inside %d %f",y_temp,ww);           
//           cout<<buf<<endl;           
           if(y <= y_temp +  ww && y >= y_temp - ww)
              return true;
        }
        else
        {
//           sprintf(buf,"minx=%d,maxx=%d,miny=%d,maxy=%d",std::min(xx1, xx),std::max(xx1, xx),std::min(yy1,yy),std::max(yy1,yy));
//           cout<<buf<<endl;
        }
     }
   }   
   return false;
}
void LineGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c)
{
   paintInternal(difX,difY,p,c);
}


void LineGraphicalItem::moveDown(BOARD_LEVEL_ID idLevel)
{
   for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
   {
      m_points[i].setY(m_points[i].y() + 1.0f/PIXELS_PER_MM);
   }
}

void LineGraphicalItem::moveLeft(BOARD_LEVEL_ID )
{
   for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
   {
      m_points[i].setX(m_points[i].x() - 1.0f/PIXELS_PER_MM);
   }
}

void LineGraphicalItem::moveRight(BOARD_LEVEL_ID )
{
   for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
   {
      m_points[i].setX(m_points[i].x() + 1.0f/PIXELS_PER_MM);
   }
}

void LineGraphicalItem::moveUp(BOARD_LEVEL_ID )
{
  for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
  {
     m_points[i].setY(m_points[i].y() - 1.0f/PIXELS_PER_MM);
  }
}
void LineGraphicalItem::addNode(int index)
{

   if (index >= 0 && index <= static_cast<int>(m_points.size()) - 1 )
   {
       PointF point(0.0f,0.0f);
       if(index == 0)
       {
          point.setX(m_points[0].x() + (m_points[1].x() -  m_points[0].x())/2);
          point.setY(m_points[0].y() + (m_points[1].y() -  m_points[0].y())/2);
          m_points.insert(m_points.begin() + 1,point);
       }
       else
       {
          point.setX(m_points[static_cast<VectorInd>(index) - 1].x()
                  + (m_points[static_cast<VectorInd>(index)].x() -
                  m_points[static_cast<VectorInd>(index) - 1].x())/2);
          point.setY(m_points[static_cast<VectorInd>(index) - 1].y()
                  + (m_points[static_cast<VectorInd>(index)].y() -
                     m_points[static_cast<VectorInd>(index) - 1].y())/2);
          m_points.insert(m_points.begin() + index,point);
       }
   }
   return;
}


bool LineGraphicalItem::removeNode(int index,PointF& pt)
{
   if(m_points.size() < 3)
      return false;
   if (index >= 0 && index <= static_cast<int>(m_points.size()) - 1 )
   {
      pt = m_points.at(static_cast<VectorInd>(index));
      m_points.erase(m_points.begin() + index);
      return true;
   }
   return false;
}

bool LineGraphicalItem::isAboveNodeInternal(QPoint &p, int &index)
{
    bool bRes = false;
    for( std::vector<PointF>::size_type i = 0 ; i <  m_points.size(); ++i)
    {
       int x = static_cast<int>(m_points[i].x() * PIXELS_PER_MM * k_zoom);
       int y = static_cast<int>(m_points[i].y() * PIXELS_PER_MM * k_zoom);
//       cout<<"x="<<x<<"y="<<y<<"pt.x="<<p.x()<<"pt.y="<<p.y()<<endl;
       float dif_x = x - p.x();
       float dif_y = y - p.y();
       int dd = static_cast<int>(sqrt(dif_x * dif_x + dif_y * dif_y));
       bRes =  (dd <= (m_w * PIXELS_PER_MM * k_zoom)/2);
       if(bRes)
       {
          index = static_cast<int>(i);
          p.setX(static_cast<int>(m_points[i].x() * PIXELS_PER_MM * k_zoom));
          p.setY(static_cast<int>(m_points[i].y() * PIXELS_PER_MM * k_zoom));
          return bRes;
       }
    }
    return bRes;
}

bool LineGraphicalItem::isAboveNode(QPoint& p,int& index)
{
   if(!m_selected)
      return m_selected;
   return isAboveNodeInternal(p,index);
}

int LineGraphicalItem::prepareToChange(const QPoint& p)
{
   int indNodeToMove = -1;
   //if we are above the node the index will be set to 
   //some value and no need to check result
   isAboveNode(const_cast<QPoint&>(p),indNodeToMove);
   return indNodeToMove;
}


void LineGraphicalItem::moveNode(PointF& p,int index)
{
   if(p.x() < 0)
   {
      //move is finished
      return;
   }
   if(index != -1)
   {
      m_points[static_cast<VectorInd>(index)].setX(p.x());
      m_points[static_cast<VectorInd>(index)].setY(p.y());
   }
}

void LineGraphicalItem::moveFor(BOARD_LEVEL_ID idLevel, PointF dif)
{
   //dif should prepared before calling this function according to zoom value
   if(m_level == idLevel || idLevel == LEVEL_ALL)
   {
      for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
      {
         m_points[i] += dif;
//          m_points[i].setX(m_points[i].x() + dif.x());
//          m_points[i].setY(m_points[i].y() + dif.y());
      }
   }
}



void LineGraphicalItem::setX(float)
{
   //does not make sense for this item
   //use moveFor   
}

void LineGraphicalItem::setY(float)
{
   //does not make sense for this item
   //use moveFor
}

SmartPtr<GraphicalItem> LineGraphicalItem::clone()
{
   PointF pt(m_x,m_y);
   vector<PointF> vec(m_points);
   auto p = SmartPtr<GraphicalItem>::make_smartptr<LineGraphicalItem>(std::move(vec),m_w,m_level,k_zoom,k_zoom_d,
                                                IDsGenerator::instance()->getNewID());
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent,false);
   p->setType(m_type);
   return p;   
}

void LineGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setWidth(props.line_g.width);
   m_style = props.line_g.style;
}

GraphicalItem* LineGraphicalItem::isConnectable(PointF& ptf)
{
   int index = -1;
   QPoint pt(static_cast<int>(ptf.x()*PIXELS_PER_MM*k_zoom),
             static_cast<int>(ptf.y()*PIXELS_PER_MM*k_zoom));
   if (isAboveNodeInternal(pt,index))
   {
       float x = m_points[static_cast<VectorInd>(index)].x();
       float y = m_points[static_cast<VectorInd>(index)].y();
       if(m_parent)
           coordRelativeToAbsolute(x,y);
       ptf.setX(x);
       ptf.setY(y);
       return this;
   }
   return nullptr;
}

void LineGraphicalItem::connectorAdd(ITEM_ID connectorID, PointF &ptf)
{
   int index = -1;
   QPoint pt(static_cast<int>(ptf.x()*PIXELS_PER_MM*k_zoom),
             static_cast<int>(ptf.y()*PIXELS_PER_MM*k_zoom));
   if (isAboveNodeInternal(pt,index))
   {
      if(!m_vcCons)
          m_vcCons.reset(new map<int,ITEM_ID>);
      (*m_vcCons)[index] = connectorID;
   }
}

void LineGraphicalItem::connectorRemove(ITEM_ID connectorID)
{
   if(!m_vcCons)
      return;
   for(auto iter = m_vcCons->begin();iter != m_vcCons->end(); ++iter)
       if(iter->second == connectorID)
       {
         m_vcCons->erase(iter);
         break;
       }
}

void LineGraphicalItem::getConnectors(set<ITEM_ID>& connectors)
{
   if(!m_vcCons)
      return;
   for(auto item:*m_vcCons)
      connectors.insert(item.second);
}

void LineGraphicalItem::getConnectorsEx(map<ITEM_ID,PointF>& connectors)
{

    if(!m_vcCons)
       return;
    for (auto item:*m_vcCons)
    {
       connectors[item.second] = m_points[static_cast<VecCoordIdx>(item.first)];
    }
}

ITEM_ID LineGraphicalItem::isRectOccupied(PointF& top,PointF& bottom)
{
   auto x1 = top.x();
   auto y1 = bottom.y();
   auto x2 = bottom.x();
   auto y2 = top.y();
   if(isPointInF(top.x(),top.y(),LEVEL_ALL) ||
      isPointInF(bottom.x(),bottom.y(),LEVEL_ALL) ||
      isPointInF(x1,y1,LEVEL_ALL) ||
      isPointInF(x2,y2,LEVEL_ALL) ||
      isPointInF((x1 + x2)/2,(y1 + y2)/2,LEVEL_ALL))
       return m_id;
   else {
       return ID_NONE;
   }
}

void LineGraphicalItem::anotherItemWasChanged(GraphicalItem *p,float dx,float dy)
{
   using VecPtIndex = vector<PointF>::size_type;
   if(!m_vcCons)
       return;
   if(GraphicalItemsContainer *container = dynamic_cast<GraphicalItemsContainer*>(p))
   {
      vector<SmartPtr<GraphicalItem> > *pChildren = container->getChildren();
      for (auto item:*pChildren)
      {
         for(auto& conn:*m_vcCons)
         {
            if(item->getID() == conn.second || p->getID() == conn.second)
            {
               //moddify m_points[conn.first]
               m_points[static_cast<VecPtIndex>(conn.first)].m_x += dx;
               m_points[static_cast<VecPtIndex>(conn.first)].m_y += dy;
            }
         }
      }
   }
   else
   {
      for(auto& conn:*m_vcCons)
      {
         if(p->getID() == conn.second)
         {
            //moddify m_points[conn.first]
            m_points[static_cast<VecPtIndex>(conn.first)].m_x += dx;
            m_points[static_cast<VecPtIndex>(conn.first)].m_y += dy;
         }
      }
   }
}
*/
