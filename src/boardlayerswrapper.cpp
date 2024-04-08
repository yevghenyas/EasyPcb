#include "boardlayerswrapper.h"
#include <QMessageBox>

BoardLayersWrapper::BoardLayersWrapper()
{

}

void BoardLayersWrapper::initLayers(vector<BoardLevel>& levels)
{
   boardLayers.reserve(levels.size());
   for(VecLevelsInrementer i = 0; i < levels.size() ; ++i)
   {
      //we need it in reverse order since they declared
      //in the order reversed to that in which we need
      //to draw it
      boardLayers.push_back(BoardLayer(levels[levels.size() - 1 - i]));
      if(levels[levels.size() - 1 - i].idLevel == BOARD_LEVEL_ID::LEVEL_A1 ||
         levels[levels.size() - 1 - i].idLevel == BOARD_LEVEL_ID::LEVEL_F1)
          m_selectionOrder.push_back(i);
      else
          m_selectionOrder.push_front(i);
      levelIdToInd[(boardLayers[boardLayers.size() - 1]).getLevel()] = static_cast<int>(i);
   }
}

BoardLayer* BoardLayersWrapper::getLayer(BOARD_LEVEL_ID idLevel)
{
   auto item = levelIdToInd.find(idLevel);
   if(item != levelIdToInd.end())
      return &boardLayers[static_cast<VecLevelsInrementer>(item->second)];
   return nullptr;
}

void BoardLayersWrapper::paintLayers(QPainter& p,MODE_DRAW mode,int zoom_plus)
{
   for(VecLevelsInrementer i = 0; i < boardLayers.size() ; ++i)
   {
      boardLayers[i].paintItems(p,mode,zoom_plus);
   }
}

void BoardLayersWrapper::checkIfHoveringAndPaint(QPainter& p,MODE_DRAW mode,PointF& pt,
                                                                    SmartPtr<GraphicalItem>& ptr,int zoom_plus)
{
   for(VecLevelsInrementer i = 0; i < boardLayers.size() ; ++i)
   {
      boardLayers[i].checkPropertiesAndPaint(p,mode,pt,ptr,zoom_plus);
   }
}

SmartPtr<GraphicalItem> BoardLayersWrapper::findItem(const QString& name,BOARD_LEVEL_ID idLevel)
{
   SmartPtr<GraphicalItem> ptr;
   if(idLevel != LEVEL_NONE &&
      idLevel != LEVEL_ALL)
   {
      auto item = levelIdToInd.find(idLevel);
      if(item != levelIdToInd.end())
         boardLayers[static_cast<VecLevelsInrementer>(item->second)].getItem(name,ptr);
      else
         throw invalid_argument("findItem:idLevel is not correct");
   }
   else
   {
      for(VecLevelsInrementer i = 0; i < boardLayers.size() ; ++i)
      {
         if(boardLayers[i].getItem(name,ptr))
            break;
      }
   }
   return ptr;
}

SmartPtr<GraphicalItem> BoardLayersWrapper::findSchematicConnectorItem(QString& name)
{
   SmartPtr<GraphicalItem> res;
   auto item = levelIdToInd.find(BOARD_LEVEL_ID::LEVEL_VC);
   if(item != levelIdToInd.end())
   {
      auto connector = boardLayers[static_cast<VecLevelsInrementer>(item->second)]
              .getConnectItemsInLevel()->find(name);
      if(connector != boardLayers[static_cast<VecLevelsInrementer>(item->second)]
              .getConnectItemsInLevel()->end())
         res = connector->second;
   }
   else
      throw invalid_argument("findSchematicConnectorItem:idLevel is not correct");
   return res;
}

bool BoardLayersWrapper::addGraphicalItemToLayers(SmartPtr<GraphicalItem>& p)
{
   if(p->getID() == ID_NONE)
   {
      p->setID(IDsGenerator::instance()->getNewID());
   }
   else
   {
      IDsGenerator::instance()->adjustCounter(p->getID());
   }
   set<BOARD_LEVEL_ID> layersSet;
   p->getLevels(layersSet);
   for(auto& layer:layersSet)
   {
      auto pLayer = getLayer(layer);
      if(pLayer != nullptr)
      {
         QString name = QString::number(p->getID());
         pLayer->addGraphicalItemToLevel(name,p);
         //check if the item is a connector
         if(layer == BOARD_LEVEL_ID::LEVEL_VC)
         {
            ConnectorGraphicalItem* ptr = dynamic_cast<ConnectorGraphicalItem*>(p.get());
            if(ptr != nullptr)
            {
               for(auto& item:*ptr->getConnectedItems())
                  item.second->connect(ptr->getID());
            }
         }
      }
      else
      {
         char buf[96];
         sprintf(buf,"addGraphicalItemToLayers: layerId=% is not found",layer);
         throw invalid_argument(buf);
      }
   }
}

bool BoardLayersWrapper::deleteGraphicalItemFromLayers(SmartPtr<GraphicalItem>& p)
{
   QString name = QString::number(p->getID());
   //delete from all layers
   set<ITEM_ID> connectorsSet;
   p->getConnectors(connectorsSet);
   if(connectorsSet.size() > 0)
   {
      QMessageBox msgBox(QMessageBox::Critical,"Deleting item",
                         "The item has schematic connectors! First delete connectors!",
                         QMessageBox::Ok);
      msgBox.exec();
      return false;
   }
//   BoardLayer* pLayer = getLayer(BOARD_LEVEL_ID::LEVEL_VC);
/*
   for(auto id:connectorsSet)
   {
      QString conName = QString::number(id);
      SmartPtr<GraphicalItem> ptr = findSchematicConnectorItem(conName);
      //disconnect and delete
      if(ConnectorGraphicalItem *p = dynamic_cast<ConnectorGraphicalItem*>(ptr.get()))
      {
         auto items = p->getConnectedItems();
         for(auto& item:*items)
         {
            item.second->disconnect(id);
         }
         pLayer->deleteConnectorFromLevel(p);
      }
   }
*/ ConnectorGraphicalItem *pC = nullptr;
   if((pC = dynamic_cast<ConnectorGraphicalItem*>(p.get())) != nullptr)
   {
      auto pConItems =  pC->getConnectedItems();
      if(pConItems != nullptr)
      {
         for(auto& conItem :*pConItems)
            conItem.second->disconnect(p->getID());
      }
   }
   for(auto& layer:boardLayers)
      layer.deleteGraphicalItemFromLevel(name);
}

bool BoardLayersWrapper::isPointIn(int x,int y,QString &name_out,SmartPtr<GraphicalItem>& ptr)
{
   for(list<VecLayerInrementer>::iterator iter = m_selectionOrder.begin();
       iter != m_selectionOrder.end(); ++iter)
   {
      if(boardLayers[*iter].isPointIn(x,y,name_out,ptr))
         return true;
   }
   return false;
}

void BoardLayersWrapper::removeAll()
{
   for(auto& layer:boardLayers)
      layer.clear();
}

void BoardLayersWrapper::copyItems(GraphicalItemsMap& dstMap) const
{
   for(VecLayerInrementer i = 0 ; i < boardLayers.size(); ++i)
      boardLayers[i].copyItems(dstMap);
}

void BoardLayersWrapper::writeLayersToFile(FileWriter& fileWriter)
{
   for(VecLayerInrementer i = 0 ; i < boardLayers.size(); ++i)
   {
      fileWriter.writeItems(*(boardLayers[i].getItemsInLevel()));
      fileWriter.writeItems(*(boardLayers[i].getConnectItemsInLevel()));
   }

}

void BoardLayersWrapper::checkSelectForArea(const QPoint& top,const QPoint& bottom,
                                            GraphicalItemsMap& mapOfSelection)
{
   PointF topf(top.x()/PIXELS_PER_MM,top.y()/PIXELS_PER_MM);
   PointF botf(bottom.x()/PIXELS_PER_MM,bottom.y()/PIXELS_PER_MM);
   auto lambda = [&topf,&botf](SmartPtr<GraphicalItem>& it)
   {
      auto x = it->abs_x();
      auto y = it->abs_y();
      if(x > topf.x() && x < botf.x() &&
         y > topf.y() && y < botf.y())
         return true;
      else
         return false;
   };

   for(VecLayerInrementer i = 0; i < boardLayers.size(); ++i)
   {
      for (int j = 0; j < 2; ++j)
      {
         auto items = (j == 0 ?
                           boardLayers[i].getItemsInLevel() :
                                boardLayers[i].getConnectItemsInLevel());
         for (auto& item:*items)  //*boardLayers[i].getItemsInLevel())
         {
            bool bShouldSelect = false;
            GenericGraphicalItemsContainer *p =
                 dynamic_cast<GenericGraphicalItemsContainer*>(item.second.get());
            if(p != nullptr)
            {
               for(auto&ch:*p->getChildren())
               {
                  bShouldSelect = lambda(ch);
                  if(bShouldSelect)
                     break;
               }
            }
            else
               bShouldSelect = lambda(item.second);
            if(bShouldSelect)
            {
               item.second->setSelected(true);
               mapOfSelection.insert(item);
            }
         }
      }
   }
}

void BoardLayersWrapper::getNonEmptyLayersIds(set<BOARD_LEVEL_ID>& layersSet,
                         bool bActiveLayersOnly)
{
   for(auto& item:boardLayers)
      if(item.getItemsInLevel()->size() > 0 ||
         item.getConnectItemsInLevel()->size() > 0)
      {
         if(!bActiveLayersOnly || (bActiveLayersOnly &&
             !LevelsWrapper::isPackageLevel(item.getLevel())))
            layersSet.insert(item.getLevel());
      }
}
