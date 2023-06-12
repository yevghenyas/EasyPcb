#include "multiplategraphicalitem.h"
#include "roundplategraphicalitem.h"

MultiplateGraphicalItem::MultiplateGraphicalItem(float x,float y,BOARD_LEVEL_ID level,vector<SmartPtr<GraphicalItem> >&& items,
                                                 ITEM_ID id):GenericGraphicalItemsContainer(x,y,level,std::move(items),id)
{
   if(getChildren()->size() != 2)
      throw std::logic_error("Multiplate should have 2 plates");
   if(!dynamic_cast<RoundPlateGraphicalItem*>(getChildren()->at(0).get()))
      throw std::logic_error("Multiplate should have 2 RoundPlates");
   if(!dynamic_cast<RoundPlateGraphicalItem*>(getChildren()->at(1).get()))
      throw std::logic_error("Multiplate should have 2 RoundPlates");
}
