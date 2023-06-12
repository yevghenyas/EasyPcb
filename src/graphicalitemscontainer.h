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

#ifndef GRAPHICALITEMSCONTAINER_H
#define GRAPHICALITEMSCONTAINER_H

#include <vector>
#include "graphicalitem.h"
#include "smartptr.h"

using namespace std;

class GraphicalItemsContainer : public GraphicalItem
{
public:
    GraphicalItemsContainer(BOARD_LEVEL_ID level,float x,float y,ITEM_ID id);
    virtual void groupItems() = 0;
    virtual bool unGroupItems() = 0;
    virtual vector<SmartPtr<GraphicalItem> >* getChildren() = 0;
    virtual void setChildren(vector<SmartPtr<GraphicalItem> >&& children) = 0;
    virtual void deleteAllChildren() = 0;
    //return max coordinate of children
    virtual float getMaxX() = 0;
    virtual float getMaxY() = 0;
    virtual ~GraphicalItemsContainer();
};

#endif // GRAPHICALITEMSCONTAINER_H
