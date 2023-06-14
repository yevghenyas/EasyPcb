#ifndef MULTIPLATEGRAPHICALITEM_H
#define MULTIPLATEGRAPHICALITEM_H

#include "genericgraphicalitemscontainer.h"


class MultiplateGraphicalItem : public GenericGraphicalItemsContainer
{
public:
    MultiplateGraphicalItem(float x,float y,BOARD_LEVEL_ID level,vector<SmartPtr<GraphicalItem> >&& items,
                            ITEM_ID id);
    SmartPtr<GraphicalItem> getFirstPlate(){return getChildren()->at(0);}
    SmartPtr<GraphicalItem> getSecondPlate(){return getChildren()->at(1);}
};

#endif // MULTIPLATEGRAPHICALITEM_H
