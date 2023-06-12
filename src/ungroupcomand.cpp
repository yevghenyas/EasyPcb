#include "ungroupcomand.h"

UngroupComand::UngroupComand(PcBoard *pBoard,SmartPtr<GraphicalItem>& pContaner):m_pBoard(pBoard),
                                                 m_pContainer(pContaner)
{

   pContaner->setSelected(false);
}

UngroupComand::~UngroupComand()
{

}

void UngroupComand::redo()
{
   if(GenericGraphicalItemsContainer* pC =
           dynamic_cast<GenericGraphicalItemsContainer*>(m_pContainer.get()))
   {
      if(m_Items.size() == 0)
      {
         m_Items =  *pC->getChildren();
      }
      m_pBoard->ungroupCommand(m_pContainer);
/*
      //increment refs to avoid deleting
      if(m_map)
      {
         for(auto& item:*m_map)
         {
            item.second->incRefs();
            item.second->setSelected(false);
         }
      }
*/
      m_pBoard->repaint();

   }
}

void UngroupComand::undo()
{
   if(GenericGraphicalItemsContainer* pCont =
           dynamic_cast<GenericGraphicalItemsContainer*>(m_pContainer.get()))
   {
      shared_ptr<GraphicalItemsMap> pMap = make_shared<GraphicalItemsMap>();
      for(auto item:m_Items)
      {
         char buf[32];
         sprintf(buf,"%d",item->getID());
         (*pMap)[buf] = item;
      }
      m_pBoard->regroupCommand(m_pContainer,*pMap);
      //refresh zoom value
//      pCont->setZoom(m_pBoard->getZoomValue());
/*
      //now we must decrement references
      for(auto& item:*pMap)
      {
        item.second->decRefs();
      }
*/
      //delete the content we do not need it
      //this is important because in case this command deleted
      //the destructor may errouneusly delete the items
      pMap->clear();
   }
}
