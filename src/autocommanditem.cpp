#include "autocommanditem.h"

AutoCommandItem::AutoCommandItem(ConstructedLayer&& cMap,MultiMap&& mMap,
                                 set<ITEM_ID>&& toDelete,PcBoard *p)
                                 :conMap(std::move(cMap)),multiMap(std::move(mMap)),

                                  m_pBoard(p)
{
    for(auto vcConID:toDelete)
    {
       auto delItem = m_pBoard->getBoardLayers()->findItem(QString::number(vcConID),LEVEL_VC);
       if(delItem.get() != nullptr)
          itemsToDelete.push_back(delItem);
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
   for(auto vcCon:itemsToDelete)
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

   for(auto vcCon:itemsToDelete)
      m_pBoard->addGraphicalItemToLevels(vcCon);

   m_pBoard->repaint();
}
