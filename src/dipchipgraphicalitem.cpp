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

#include "dipchipgraphicalitem.h"
#include "roundplategraphicalitem.h"
#include "drawwrapper.h"


DipChipGraphicalItem::DipChipGraphicalItem(BOARD_LEVEL_ID idLevel,
                                           BOARD_LEVEL_ID idPackLevel,
                                           QPoint p,int width,int n,int k,int k1):GraphicalItemsContainer(idLevel,k,k1,p.x()/k,p.y()/k),
                                           m_packLevel(idPackLevel),m_width(width),m_n(n),
                                           m_x(p.x()/k),m_y(p.y()/k)
                                           
{
   m_type = O_VERTICAL_TOP;
   k_zoom = k;
   m_plates.reserve(n);
   int D = CHIP_PLATE_D_EX * PIXELS_PER_MM;
   int d = CHIP_PLATE_D_IN;
   for(int j = 0; j < 2; ++j)
   {
      
      int x = (m_x - m_width/2 - D/2 - PACK_LINE_WIDTH/2) * k_zoom + j * (D + m_width + PACK_LINE_WIDTH) * k_zoom;
      for(int i = 0; i < n/2 ; ++i)
      {
         int y = (m_y - 2.5 * PIXELS_PER_MM * m_n/4) * k_zoom + (2.5 * PIXELS_PER_MM/2 + i * 2.5 * PIXELS_PER_MM) * k_zoom;
         RoundPlateGraphicalItem *p = new RoundPlateGraphicalItem(x,y,D,d,m_level,k,k1);
         m_plates.push_back(p);
      }
   }
}

DipChipGraphicalItem::~DipChipGraphicalItem()
{

}

void DipChipGraphicalItem::prepareToChange(const QPoint& p)
{

}

bool DipChipGraphicalItem::isAboveNode(QPoint p, int& index)
{
   return false;
}

void DipChipGraphicalItem::moveRight(BOARD_LEVEL_ID idLevel)
{
   if (idLevel == m_level)
   {   
   ++m_x;
   for(auto item:m_plates)
   {
      item->moveRight(idLevel);
   }
   }
}

void DipChipGraphicalItem::moveLeft(BOARD_LEVEL_ID idLevel)
{
   if (idLevel == m_level)
   {   
   --m_x;
   for(auto item:m_plates)
   {
      item->moveLeft(idLevel);
   }
   }
}

void DipChipGraphicalItem::moveDown(BOARD_LEVEL_ID idLevel)
{
   if (idLevel == m_level)
   {   
   ++m_y;
   for(auto item:m_plates)
   {
      item->moveDown(idLevel);
   }
   }
}

void DipChipGraphicalItem::moveUp(BOARD_LEVEL_ID idLevel)
{
   if (idLevel == m_level)
   {
   --m_y;
   for(auto item:m_plates)
   {
      item->moveUp(idLevel);
   }
   }
}


void DipChipGraphicalItem::zoom(int k)
{

}


void DipChipGraphicalItem::paintItemIntoBuffer(QPainter& p, QColor& c,int zoom_d)
{

}

void DipChipGraphicalItem::paintItem(QPainter& p, QColor& c,BOARD_LEVEL_ID idLevel)
{
   if(idLevel == m_packLevel)
   {
      if(m_type == O_VERTICAL_TOP)
      {
         QPen pen;       
         pen.setWidth(PACK_LINE_WIDTH);
         p.setPen(pen);
         DrawWrapper::drawPackege(p,c,m_type,m_x,m_y,m_width,m_n,k_zoom);
//         p.drawRect((m_x - m_width/2) * k_zoom,(m_y - m_n/4 * 2.5 * PIXELS_PER_MM) * k_zoom,m_width * k_zoom,m_n/2 * 2.5 * PIXELS_PER_MM * k_zoom);
//         p.drawRect((m_x - m_width/4) * k_zoom,(m_y - m_n/4 * 2.5 * PIXELS_PER_MM) * k_zoom,m_width/2 * k_zoom,PIXELS_PER_MM * k_zoom);
      }
   }
   if(idLevel == m_level)
   {
      for(auto item:m_plates)
      {
         item->paintItem(p,c);
      }
   }
   
}

bool DipChipGraphicalItem::isPointIn(int x, int y)
{
   if(x > m_x * k_zoom && y > m_y * k_zoom 
      && x < (m_x + m_width) * k_zoom 
      && y < (m_y + m_n/2 * 2.5 * PIXELS_PER_MM) * k_zoom)
      return true;
   for(auto item:m_plates)
   {
      if(item->isPointIn(x,y))
         return true;
   }
   return false;
}


void DipChipGraphicalItem::groupItems(std::vector< GraphicalItem* > toGroup)
{

}

bool DipChipGraphicalItem::unGroupItems()
{

}
void DipChipGraphicalItem::zoomIn(BOARD_LEVEL_ID idLevel)
{
   if(idLevel == m_packLevel)
      GraphicalItem::zoomIn(idLevel);
   else if(idLevel == m_level)
   {
      for(auto item:m_plates)
      {
         item->zoomIn(idLevel);
      }    
   }
}

void DipChipGraphicalItem::zoomOut(BOARD_LEVEL_ID idLevel)
{
   if(idLevel == m_packLevel)
      GraphicalItem::zoomOut(idLevel);
   else if(idLevel == m_level)
   {
      for(auto item:m_plates)
      {
         item->zoomOut(idLevel);
      }    
   }
}

BOARD_LEVEL_ID DipChipGraphicalItem::getPackLevel()
{
   return m_packLevel;
}

void DipChipGraphicalItem::setPackLevel(BOARD_LEVEL_ID packLevel)
{
   m_packLevel = packLevel;
}

std::vector< GraphicalItem* >* DipChipGraphicalItem::getChildren()
{
   return &m_plates;
}

void DipChipGraphicalItem::groupItems(std::vector< GraphicalItem* >& toGroup)
{

}


void DipChipGraphicalItem::setSelected(bool select)
{
   GraphicalItem::setSelected(select);
   for(auto item:m_plates)
      item->setSelected(select);
}

