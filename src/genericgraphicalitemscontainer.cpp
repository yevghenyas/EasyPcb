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
#include "genericgraphicalitemscontainer.h"
#include "itemsfactory.h"
#include <iostream>
#include "rectgraphicalitem.h"
#include "roundplategraphicalitem.h"
#include "rectpackagegraphicalitem.h"
#include "roundpackagegraphicalitem.h"

GenericGraphicalItemsContainer::GenericGraphicalItemsContainer(float x,float y,
                           BOARD_LEVEL_ID level,vector<SmartPtr<GraphicalItem> >&& items,ITEM_ID id):
GraphicalItemsContainer(level,x,y,id),m_items(items),m_bParent(false)
{
}

GenericGraphicalItemsContainer::~GenericGraphicalItemsContainer()
{
}

vector<SmartPtr<GraphicalItem> >* GenericGraphicalItemsContainer::getChildren()
{
   return &m_items;
}

bool GenericGraphicalItemsContainer::unGroupItems()
{
   setAsParent(false,false);
}
//sets 'this' as parent for all items in GenericGraphicalItemsContainer
//NOTE! the coordinates of center of GenericGraphicalItemsContainer 
//should be set before this operation
//otherwise use groupItemsWithCoord(...)
void GenericGraphicalItemsContainer::groupItems()
{
   setAsParent(true,true);
}



void GenericGraphicalItemsContainer::setAsParent(bool bParent,bool bRecalcCoord)
{
   for(auto& item:m_items)
   {
      if(bParent)
         item->setParent(this,bRecalcCoord);
      else
         item->unsetParent();
   }
   m_bParent = bParent;
}



bool GenericGraphicalItemsContainer::groupItemsWithCoord(float x, float y)
{
   m_x = x;
   m_y = y;
   groupItems();
}


int GenericGraphicalItemsContainer::prepareToChange(const QPoint& )
{

}

bool GenericGraphicalItemsContainer::isAboveNode(QPoint& , int& )
{
   //this is not a connector
   return false;
}

void GenericGraphicalItemsContainer::moveRight(BOARD_LEVEL_ID idLevel)
{
   if(!m_bParent)
   {   
      for(auto& item:m_items)
      {
         if(item->getLevel() == idLevel || idLevel == LEVEL_ALL)
         {
            item->moveRight(idLevel);
         }
      }
   }
   else
   {
      m_x += minGraularity;
   }
}

void GenericGraphicalItemsContainer::moveLeft(BOARD_LEVEL_ID idLevel)
{
   if(!m_bParent)
   {      
      for(auto& item:m_items)
      {
         if(item->getLevel() == idLevel || idLevel == LEVEL_ALL)
         {
            item->moveLeft(idLevel);
         }
      }
   }
   else
   {
      m_x -= minGraularity;
   }
}

void GenericGraphicalItemsContainer::moveDown(BOARD_LEVEL_ID idLevel)
{
   if(!m_bParent)
   {         
      for(auto& item:m_items)
      {
         if(item->getLevel() == idLevel || idLevel == LEVEL_ALL)
         {
            item->moveDown(idLevel);
         }
      }
   }
   else
   {
      m_y += minGraularity;
   }
}

void GenericGraphicalItemsContainer::moveUp(BOARD_LEVEL_ID idLevel)
{
   if(!m_bParent)
   {      
      for(auto& item:m_items)
      {
         if(item->getLevel() == idLevel || idLevel == LEVEL_ALL)
         {
            item->moveUp(idLevel);
         }
      }
   }
   else
   {
      m_y -= minGraularity;
   }
}


void GenericGraphicalItemsContainer::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor&
                                                         ,int zoom,int zoom_d)
{
   for(auto& item:m_items)
   {
      QColor col(LevelsWrapper::getColorForLevel(item->getLevel()));
      item->paintItemIntoBuffer(difX,difY,p,col,zoom,zoom_d);
   }
}

void GenericGraphicalItemsContainer::paintItem(QPainter& p, QColor& c,int zoom,int zoom_d, BOARD_LEVEL_ID idLevel)
{
   for(auto& item:m_items)
   {
      if(item->getLevel() == idLevel)
      {
         item->paintItem(p,c,zoom,zoom_d,idLevel);
      }
   }
}

bool GenericGraphicalItemsContainer::isPointIn(int x, int y, BOARD_LEVEL_ID idLevel)
{
   for(auto& item:m_items)
   {
      if(idLevel != LEVEL_ALL &&
              idLevel != item->getLevel())
         continue;
      if(item->isPointIn(x,y,idLevel))
      {
         return true;
      }
   }
   return false;
}


bool GenericGraphicalItemsContainer::isPointInF(float x, float y, BOARD_LEVEL_ID idLevel)
{
   for(auto& item:m_items)
   {
      if(idLevel != LEVEL_ALL &&
               idLevel != item->getLevel())
          continue;

      if(item->isPointInF(x,y,idLevel))
         return true;
   }
   return false;
}

void GenericGraphicalItemsContainer::setSelected(bool select)
{
   GraphicalItem::setSelected(select);
   for(auto& item:m_items)
   {
      item->setSelected(select);
   }    
    
}

void GenericGraphicalItemsContainer::rotate(BOARD_LEVEL_ID idLevel,int x, int y, int grad)
{


    int x1i = castFloatToInt(m_x * PIXELS_PER_MM);
    int y1i = castFloatToInt(m_y * PIXELS_PER_MM);
    if(x == -1)
       x = x1i;
    if(y == -1)
       y = y1i;
    for(auto item:m_items)
       item->rotate(idLevel,x,y,grad);
}



void GenericGraphicalItemsContainer::moveFor(BOARD_LEVEL_ID idLevel, PointF dif)
{
   if(!m_bParent)
   {
      for(auto& item:m_items)
      {
         item->moveFor(idLevel,dif);
      } 
   }
   else
   {
      m_x += dif.x();
      m_y += dif.y();
   }      
}

void GenericGraphicalItemsContainer::getLevels(std::set< BOARD_LEVEL_ID >& setIds)
{
   for(auto item:m_items)
   {
      item->getLevels(setIds);
   }   
}

SmartPtr<GraphicalItem> GenericGraphicalItemsContainer::clone()
{
   vector<SmartPtr<GraphicalItem> > items;
   for(auto& item:m_items)
   {
      items.push_back(item->clone());
   }
   auto p = ItemsFactory::createContainer(m_x,m_y,m_level,items,
                                          IDsGenerator::instance()->getNewID());
   if(m_bParent)
      static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,false);
   
   return p;
            
}

void GenericGraphicalItemsContainer::setChildren(vector<SmartPtr<GraphicalItem> >&& children)
{
   m_items = children;
}
void GenericGraphicalItemsContainer::setGeometry(GeomCommonProps& )
{
   //defined here to avoid linking problems. This method does not make sense for containers.
}

void GenericGraphicalItemsContainer::deleteAllChildren()
{
   m_items.clear();
}

GraphicalItem* GenericGraphicalItemsContainer::isConnectable(PointF& pt)
{
   for(auto& item:m_items)
      if(GraphicalItem* pItem = item->isConnectable(pt))
         return pItem;
   return nullptr;
}

void GenericGraphicalItemsContainer::setID(ITEM_ID id)
{
   for (auto& item:m_items)
   {
      if(item->getID() == ID_NONE)
         item->setID(IDsGenerator::instance()->getNewID());
   }
   GraphicalItem::setID(id);
}

void GenericGraphicalItemsContainer::getConnectors(set<ITEM_ID>& cons)
{
   for(auto& item:m_items)
      item->getConnectors(cons);
}

void GenericGraphicalItemsContainer::connect(ITEM_ID )
{

}

void GenericGraphicalItemsContainer::disconnect(ITEM_ID )
{

}

ITEM_ID GenericGraphicalItemsContainer::isRectOccupied(PointF& top, PointF& bottom)
{
   for(auto& item:m_items)
   {
      ITEM_ID id = item->isRectOccupied(top,bottom);
      if(id > 0)
         return id;
   }
   return ID_NONE;
}

float GenericGraphicalItemsContainer::getMaxX()
{
   float max = 0;
   for(auto& item:m_items)
   {
      max = std::max(max,item->x());
   }
   return max;
}

float GenericGraphicalItemsContainer::getMaxY()
{
    float max = 0;
    for(auto& item:m_items)
    {
       max = std::max(max,item->y());
    }
    return max;
}

bool GenericGraphicalItemsContainer::isInArea(int x,int y)
{
   for(auto& item:m_items)
   {
      if(item->isInArea(x,y))
         return true;
   }
   return false;
}

PointF GenericGraphicalItemsContainer::getArea()
{
   PointF pt(-1,-1);
   return pt;
}

string GenericGraphicalItemsContainer::getGerberString(GerberGenerator& gen,
                                                       BOARD_LEVEL_ID idLevel)
{
   map<string,vector<ITEM_ID>> mapOfRps;
   map<string,vector<ITEM_ID>> mapOfRects;
   string resTxt;
   enum class ItemType{Round,Rect,RectPckg};
   auto l = [](map<string,vector<ITEM_ID>>&m,
           ITEM_ID id,string&& s)
   {
      if(m.find(s) != m.end())
         m.find(s)->second.push_back(id);
      else
         m[std::move(s)] = {id};
   };

   //create cash of items that have similar sizes and geometry
   for(auto& item:m_items)
   {
      if(!item->isGerberSupported())
         continue;

      if(item->getLevel() != idLevel)
         continue;


      if(RoundPlateGraphicalItem *pRp = dynamic_cast<RoundPlateGraphicalItem*>(item.get()))
      {
         l(mapOfRps,pRp->getID(),
           std::to_string(castFloatToIntPxls(pRp->d())) +
                            ":" + std::to_string(castFloatToIntPxls(pRp->d1())));
      }
      else if(RectGraphicalItem *pRect = dynamic_cast<RectGraphicalItem*>(item.get()))
      {
         auto typeV = pRect->getType() == O_VERTICAL_BOTTOM ||
                  pRect->getType() == O_VERTICAL_TOP;
         l(mapOfRects,pRect->getID(),
            std::to_string(
                castFloatToIntPxls(typeV ? pRect->width() : pRect->height())) +
                ":" + std::to_string(
                castFloatToIntPxls(typeV ? pRect->height() : pRect->width())));
      }
      else if(RectPackageGraphicalItem *pRectP = dynamic_cast<RectPackageGraphicalItem*>(item.get()))
      {
         resTxt += pRectP->getGerberString(gen,idLevel);
      }
      else if(RoundPackageGraphicalItem *pRoundP = dynamic_cast<RoundPackageGraphicalItem*>(item.get()))
      {
         resTxt += pRoundP->getGerberString(gen,idLevel);
      }

   }
   //create appertures' strings
   string txt;
   txt.reserve(mapOfRps.size() > mapOfRects.size() ?
                   mapOfRps.size() * 64 : mapOfRects.size() * 64);

   map<ITEM_ID,ITEM_ID> itemToApperture;
   auto l2 = [&itemToApperture,&txt](map<string,vector<ITEM_ID>>&m,
            ItemType type,GerberGenerator *pGen)
   {
      for(auto& pair:m)
      {
         ITEM_ID id_apperture = pair.second.at(0);
         int d1,d2;
         sscanf(pair.first.c_str(),"%d:%d",&d1,&d2);
         if(type == ItemType::Round)
            txt += pGen->getAppertureForRoundPlate(id_apperture,
                                      static_cast<float>(d1)/PIXELS_PER_MM,
                                      static_cast<float>(d2)/PIXELS_PER_MM);
         else if(type == ItemType::Rect)
            txt += pGen->getAppertureForRect(id_apperture,
                                       static_cast<float>(d1)/PIXELS_PER_MM,
                                       static_cast<float>(d2)/PIXELS_PER_MM);


         for(auto& item:pair.second)
            itemToApperture[item] = id_apperture;
      }
   };

   l2(mapOfRps,ItemType::Round,&gen);
   resTxt += txt;
   txt.clear();

   l2(mapOfRects,ItemType::Rect,&gen);
   resTxt += txt;
   txt.clear();

   //flash items setting current
   //apperture when neccessary
   auto appertureID = ID_NONE;
   for(auto& item:m_items)
   {
      if(!item->isGerberSupported())
         continue;

      if(item->getLevel() != idLevel)
         continue;

      auto iter = itemToApperture.find(item->getID());

      if(iter == itemToApperture.end())
         continue;
      if(iter->second != appertureID)
      {
         txt += gen.getAppertureSetString(itemToApperture.find(item->getID())->second);
         appertureID = iter->second;
      }
      txt += gen.getFlashString(item->abs_x(),item->abs_y());
   }
   resTxt += txt;
   return resTxt;
}
