#include "boardlayer.h"

BoardLayer::BoardLayer(BoardLevel& level)
{
   idLevel = level.idLevel;
   color = level.color;
   strcpy(desc,level.desc);
   strcpy(colorDesc,level.colorDesc);
   bVisible = true;
}

bool BoardLayer::isItemInLevel(const QString& name)
{
   return m_itemsInLevel.find(name) != m_itemsInLevel.end() ||
          m_connectorsInLevel.find(name) != m_connectorsInLevel.end();
}


bool BoardLayer::getItem(const QString& name,SmartPtr<GraphicalItem>& ptr)
{
   auto item = m_itemsInLevel.find(name);
   if (item != m_itemsInLevel.end())
   {
      ptr = item->second;
      return true;
   }
   item = m_connectorsInLevel.find(name);
   if (item != m_connectorsInLevel.end())
   {
      ptr = item->second;
      return true;
   }

   return false;
}
void BoardLayer::addGraphicalItemToLevel(const QString& name,SmartPtr<GraphicalItem> item)
{
   cout<<"adding to level="<<idLevel<<" name="<<name.toStdString()<<endl;
   if(dynamic_cast<ConnectorGraphicalItem*>(item.get()))
      m_connectorsInLevel[name] = item;
   else
      m_itemsInLevel[name] = item;
}
void BoardLayer::addGraphicalItemToLevel(SmartPtr<GraphicalItem> item)
{
   char buf[64];
   sprintf(buf,"%d",item->getID());
//   cout<<"adding to level="<<idLevel<<" name="<<buf<<"refs "<<p->getRefs()<<endl;
   if(dynamic_cast<ConnectorGraphicalItem*>(item.get()))
   {
      if(m_connectorsInLevel.find(buf) == m_connectorsInLevel.end())
         m_connectorsInLevel[buf] = item;
   }
   else
   {
      if(m_itemsInLevel.find(buf) == m_itemsInLevel.end())
         m_itemsInLevel[buf] = item;
   }
}
void BoardLayer::deleteGraphicalItemFromLevel(const QString& name)
{
   if(!m_itemsInLevel.erase(name))
      m_connectorsInLevel.erase(name);
}
void BoardLayer::deleteGraphicalItemFromLevel(SmartPtr<GraphicalItem>& ptr)
{
   QString name = QString::number(ptr->getID());
   deleteGraphicalItemFromLevel(name);
}
void BoardLayer::clear()
{
   m_itemsInLevel.clear();
   m_connectorsInLevel.clear();
}
bool BoardLayer::isPointIn(int x,int y,QString &name_out,SmartPtr<GraphicalItem>& ptr)
{
   for(auto& item:m_itemsInLevel)
   {
      if(item.second->isPointIn(x,y,idLevel))
      {
         name_out = item.first;
         ptr = item.second;
         return true;
      }
   }
   for(auto& item:m_connectorsInLevel)
   {
      if(item.second->isPointIn(x,y,idLevel))
      {
         name_out = item.first;
         ptr = item.second;
         return true;
      }
   }
   return false ;
}
//chacks if the item connectable and paints properly
bool BoardLayer::checkPropertiesAndPaint(QPainter& p,int mode,PointF& pt,SmartPtr<GraphicalItem>& ptr,int zoom)
{
   if(!bVisible)
      return false;
   QColor c1(LevelsWrapper::getColorForSelection());
   QColor c2(LevelsWrapper::getColorForLevel(idLevel));
   QColor c3(Qt::black);
   auto chkPrp = [](int mode,
                    GraphicalItem *item,
                    PointF& pt)
   {
      switch(mode)
      {
        case MODE_VC_CON:
        {
           if(item->isConnectable(pt))
              return true;
           else
              return false;
        }
        default:
           return item->isAboveTheCenter(pt);
      }
   };
   for(auto& item:m_connectorsInLevel)
   {

      if(!ptr.get())
      {
         if(chkPrp(mode,item.second.get(),pt))
            ptr = item.second;
      }

      if(mode == MODE_PRINT)
      {
         item.second->paintItem(p,c3,zoom,1,idLevel);
      }
      else if(item.second->isSelected())
      {
         item.second->paintItem(p,c1,zoom,1,idLevel);
      }
      else
      {
         item.second->paintItem(p,c2,zoom,1,idLevel);
      }
   }
   for(auto& item:m_itemsInLevel)
   {
      if (GenericGraphicalItemsContainer *pc =
               dynamic_cast<GenericGraphicalItemsContainer*>(item.second.get()))
      {
         //container,check children
         vector<SmartPtr<GraphicalItem> > *vChildren = pc->getChildren();
         for(auto& child:*vChildren)
         {
            if(chkPrp(mode,child.get(),pt))
               ptr = child;
         }
      }
      else if(chkPrp(mode,item.second.get(),pt))
         ptr = item.second;

      if(mode == MODE_PRINT)
      {
         item.second->paintItem(p,c3,zoom,1,idLevel);
      }
      else if(item.second->isSelected())
      {
         item.second->paintItem(p,c1,zoom,1,idLevel);
      }
      else
      {
         item.second->paintItem(p,c2,zoom,1,idLevel);
      }
   }
   return ptr.get();
}
void BoardLayer::paintItems(QPainter& p,int mode,int zoom)
{
   if(!bVisible)
      return;
   QColor c1(LevelsWrapper::getColorForSelection());
   QColor c2(LevelsWrapper::getColorForLevel(idLevel));
   QColor c3(Qt::black);
   for(auto& item:m_connectorsInLevel)
   {
      if(mode == MODE_PRINT)
      {
         item.second->paintItem(p,c3,zoom,1,idLevel);
      }
      else if(item.second->isSelected())
      {
         item.second->paintItem(p,c1,zoom,1,idLevel);
      }
      else
      {
         item.second->paintItem(p,c2,zoom,1,idLevel);
      }
   }

   for(auto& item:m_itemsInLevel)
   {
//         cout<<"paint ("<<std::to_string(item.second->x())<<","<<item.second->y()<<")"<<endl;
      if(mode == MODE_PRINT)
      {
         item.second->paintItem(p,c3,zoom,1,idLevel);
      }
      else if(item.second->isSelected())
      {
         item.second->paintItem(p,c1,zoom,1,idLevel);
      }
      else
      {
         item.second->paintItem(p,c2,zoom,1,idLevel);
      }
   }
}
GraphicalItemsMap* BoardLayer::getItemsInLevel()
{
   return &m_itemsInLevel;
}
GraphicalItemsMap* BoardLayer::getConnectItemsInLevel()
{
   return &m_connectorsInLevel;
}
void BoardLayer::copyItems(GraphicalItemsMap& dstMap) const
{
   for(auto& item:m_itemsInLevel)
      dstMap[item.first] = item.second;
   for(auto& item:m_connectorsInLevel)
      dstMap[item.first] = item.second;
}

void BoardLayer::deleteConnectorFromLevel(ConnectorGraphicalItem *p)
{
   if(p == nullptr)
      return;
   const QString name = QString::number(p->getID());
   if(m_connectorsInLevel.find(name) != m_connectorsInLevel.end())
   {
      auto conItems = p->getConnectedItems();
      for(auto& item:*conItems)
      {
         item.second->disconnect(p->getID());
      }
      p->getConnectedItems()->clear();
   }
   m_connectorsInLevel.erase(name);
}
