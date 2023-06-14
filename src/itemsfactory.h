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

#ifndef ITEMSFACTORY_H
#define ITEMSFACTORY_H

#include "common.h"
#include "genericgraphicalitemscontainer.h"
#include <QXmlStreamAttributes>
#include <smartptr.h>

class ItemsFactory
{
    static bool resolveCoordinates(QXmlStreamAttributes& attributes,float& x,float& y);
    static void resolveZoom(QXmlStreamAttributes& attributes,int& zoom,int& zoom_d);
public:
    ItemsFactory();
    static SmartPtr<GraphicalItem> createRoundPlate(QXmlStreamAttributes& attributes,QString& name);
    static SmartPtr<GraphicalItem> createRoundPlate(float x,float y,float d_ex,float d_in,BOARD_LEVEL_ID level,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createRoundMultiPlate(float x,float y,float d_ex,float d_in,vector<BOARD_LEVEL_ID> levels,
                                                ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createItem(QXmlStreamAttributes& attributes,QString& name);
    static SmartPtr<GraphicalItem> createConnector(vector<PointF>* points,float w,BOARD_LEVEL_ID level,CONNECTOR_TYPE conType,
                                                   ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createContainer(float x,float y,BOARD_LEVEL_ID level, std::vector<SmartPtr<GraphicalItem> >& items,ITEM_ID id);
    static SmartPtr<GraphicalItem> createPackage(float x,float y,float w,float h,BOARD_LEVEL_ID level,ITEMS_ORIENTATION o,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createPackage(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createStdDip(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createStdDip(float x,float y,ITEMS_ORIENTATION type,int n,BOARD_LEVEL_ID level,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createStdTSOP(float x,float y,ITEMS_ORIENTATION type,int n,BOARD_LEVEL_ID level,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createStdTSOP(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createRoundPackage(float x,float y,float d,float d1,BOARD_LEVEL_ID level,ITEMS_ORIENTATION o,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createRoundPackage(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createRectPackage(float x,float y,float w,float h,BOARD_LEVEL_ID level,
                                                     ITEMS_ORIENTATION o,ITEM_ID id,int k_zoom,int zoom_d,bool filled = false);
    static SmartPtr<GraphicalItem> createRectPackage(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createElCapacitor(float x, float y, ITEMS_ORIENTATION type, float d, BOARD_LEVEL_ID level,ITEM_ID id,
                                            int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createElCapacitor(QXmlStreamAttributes attributes, QString& name);
    static SmartPtr<GraphicalItem> createResistor(float x, float y, ITEMS_ORIENTATION type, float l, BOARD_LEVEL_ID level,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createResistor(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createSmdType(float x, float y, ITEMS_ORIENTATION type,SMD_PACKAGE_TYPE pack_type, BOARD_LEVEL_ID level,
                                        ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createSmdType(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createCapSchematic(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createCapSchematic(float x, float y, ITEMS_ORIENTATION type, float f, BOARD_LEVEL_ID level,
                                             ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createCap(float x, float y, ITEMS_ORIENTATION type, float f, BOARD_LEVEL_ID level,ITEM_ID id, int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createCap(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createStdSO(float x,float y,ITEMS_ORIENTATION type,int n,BOARD_LEVEL_ID level,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createStdSO(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createRect(float x, float y, float w, float h, BOARD_LEVEL_ID level,ITEMS_ORIENTATION o,ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createRect(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> duplicateItem(GraphicalItem *p,float x,float y,bool bIgnoreCoord);
    static SmartPtr<GraphicalItem> createText(float x, float y, QString& text, int fntSize,ITEMS_ORIENTATION o, BOARD_LEVEL_ID level, ITEM_ID id,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createText(QXmlStreamAttributes attributes,QString& name);
    static SmartPtr<GraphicalItem> createQfp(float x, float y,int n,BOARD_LEVEL_ID level,int k_zoom,int zoom_d);
    static SmartPtr<GraphicalItem> createQfp(QXmlStreamAttributes attributes,QString& name);
    ~ItemsFactory();
};

#endif // ITEMSFACTORY_H
