#include "autocommanditem.h"

AutoCommandItem::AutoCommandItem(ConstructedLayer&& cMap,MultiMap&& mMap,PcBoard *p)
                                 :conMap(cMap),multiMap(mMap),m_pBoard(p)
{
   for(auto& vcToPoints:conMap)
   {
      if(conMap.count(vcToPoints.first) == 1)
      {
         SmartPtr<GraphicalItem> vcCon = m_pBoard->getBoardLayers()->findItem(vcToPoints.first,
                                                                           BOARD_LEVEL_ID::LEVEL_VC);
         if(vcCon.get() != nullptr)
            vcCons.push_back(vcCon);
      }
   }
}

AutoCommandItem::~AutoCommandItem()=default;

void AutoCommandItem::redo()
{
   //first update zoom
// m_pItem->setZoom(m_pBoard->getZoomValue());
   for(auto& vcToMulti:multiMap)
   {
      for(auto& multi:vcToMulti.second)
      {
         multi->setVisible(true);
         m_pBoard->addGraphicalItemToLevels(multi);
      }
   }
   for(auto& vcToPoints:conMap)
   {
      vcToPoints.second->setVisible(true);
      m_pBoard->addGraphicalItemToLevels(vcToPoints.second);
   }
   for(auto& vcCon:vcCons)
      m_pBoard->deleteGraphicalItem(vcCon);
   m_pBoard->repaint();
}

void AutoCommandItem::undo()
{
   for(auto& vcToMulti:multiMap)
   {
      for(auto& multi:vcToMulti.second)
         m_pBoard->deleteGraphicalItem(multi);
   }
   for(auto& vcToPoints:conMap)
      m_pBoard->deleteGraphicalItem(vcToPoints.second);
   for(auto& vcCon:vcCons)
      m_pBoard->addGraphicalItemToLevels(vcCon);

   m_pBoard->repaint();
}
