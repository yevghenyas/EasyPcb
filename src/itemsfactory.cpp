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

#include "itemsfactory.h"
#include "roundplategraphicalitem.h"
#include "connectorgraphicalitem.h"
#include "multiplategraphicalitem.h"
#include "packagegraphicalitem.h"
#include "rectgraphicalitem.h"
#include "roundpackagegraphicalitem.h"
#include "rectpackagegraphicalitem.h"
#include "capgraphicalitem.h"
#include "textgraphicalitem.h"
#include "smartptr.h"


//NOTE!!!!!
//The name of the item in the library file is always descriptive name( DIP8,DIP16 etc...)
//The name of the item in the file of schematics is always digital(which is id of the item in the chematic)
//Before creating the item we need to check if the name can be converted into digital.
//QString.toInt() returns 0 when conversion fails. In this case we set ID_NONE as id of item




ItemsFactory::ItemsFactory()
{
}

void ItemsFactory::resolveZoom(QXmlStreamAttributes& attributes,int& zoom, int& zoom_d)
{
   if(attributes.hasAttribute(ZOOM_DEF))
      zoom = attributes.value(ZOOM_DEF).toString().toInt();
   if(attributes.hasAttribute(ZOOM_D_DEF))
      zoom_d = attributes.value(ZOOM_D_DEF).toString().toInt();   
}

//in case attributes does not have x and y the 
//default values after this function x = 0; y = 0;

bool ItemsFactory::resolveCoordinates(QXmlStreamAttributes& attributes, float& x, float & y)
{
   if(attributes.hasAttribute(X_DEF))
      x = roundFloat(attributes.value(X_DEF).toString().toFloat());
   else
      x = 0;
   if(attributes.hasAttribute(Y_DEF))
      y = roundFloat(attributes.value(Y_DEF).toString().toFloat());
   else
      y = 0;
   
   return true;
}


SmartPtr<GraphicalItem> ItemsFactory::createItem(QXmlStreamAttributes& attributes,QString& name)
{
   SmartPtr<GraphicalItem> p;
   if (attributes.hasAttribute("type"))
   {
      QStringRef ref = attributes.value("type");
      if(ref.compare(QString(PLATE_ROUNd_TYPE_DEF)))
      {
         p = createRoundPlate(attributes,name);
         return p;
      }
   }
   return p;
}


SmartPtr<GraphicalItem> ItemsFactory::createRoundPlate(QXmlStreamAttributes& attributes,QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(X_DEF) ||
      !attributes.hasAttribute(Y_DEF) ||
      !attributes.hasAttribute(D_EX_DEF)  ||
      !attributes.hasAttribute(D_IN_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF))
      return p;

   float x = roundFloat(attributes.value(X_DEF).toFloat());
   float y = roundFloat(attributes.value(Y_DEF).toFloat());
   float d_ex = roundFloat(attributes.value(D_EX_DEF).toFloat());
   float d_in = roundFloat(attributes.value(D_IN_DEF).toFloat());
   int level = attributes.value(LEVEL_DEF).toInt();
   name = attributes.value(NAME_DEF).toString();
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   
   p = createRoundPlate(x,y,d_ex,d_in,static_cast<BOARD_LEVEL_ID>(level),id,iNoZoom,iNoZoom);
   return p;
}

SmartPtr<GraphicalItem>  ItemsFactory::createRoundMultiPlate(float x, float y, float d_ex, float d_in, vector<BOARD_LEVEL_ID> levels,
                                                    ITEM_ID id,int k_zoom, int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   for(auto level:levels)
      items.push_back(createRoundPlate(x,y,d_ex,d_in,level,
                                      id == ID_NONE ? id : IDsGenerator::instance()->getNewID(),k_zoom,zoom_d));
   return SmartPtr<GraphicalItem>::make_smartptr<MultiplateGraphicalItem>(x,y,levels[0],std::move(items),id);
}



SmartPtr<GraphicalItem> ItemsFactory::createConnector(vector< PointF >* points,float w,BOARD_LEVEL_ID level,
                                                      CONNECTOR_TYPE conType,ITEM_ID id,int k_zoom,int zoom_d)
{
   return SmartPtr<GraphicalItem>::make_smartptr<ConnectorGraphicalItem>(std::move(*points),w,level,conType,id);
}


SmartPtr<GraphicalItem> ItemsFactory::createRoundPlate(float x,float y,float d_ex,float d_in,BOARD_LEVEL_ID level,ITEM_ID id,int k_zoom,int zoom_d)
{
   return SmartPtr<GraphicalItem>::make_smartptr<RoundPlateGraphicalItem>(x,y,d_ex,d_in,static_cast<BOARD_LEVEL_ID>(level),id);
}


SmartPtr<GraphicalItem> ItemsFactory::createContainer(float x,float y,BOARD_LEVEL_ID level,std::vector<SmartPtr<GraphicalItem> >& items,ITEM_ID id)
{
   return SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,std::move(items),id);
}

SmartPtr<GraphicalItem> ItemsFactory::createPackage(float x, float y, float w, float h, BOARD_LEVEL_ID level,ITEMS_ORIENTATION o,ITEM_ID id,int k_zoom,int zoom_d)
{
   return SmartPtr<GraphicalItem>::make_smartptr<PackageGraphicalItem>(level,PointF(x,y),w,h,o,id);
}

SmartPtr<GraphicalItem> ItemsFactory::createPackage(QXmlStreamAttributes attributes,QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(HEIGHT_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF))
      return p;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return p;

   float w = roundFloat(attributes.value(WIDTH_DEF).toFloat());
   float h = roundFloat(attributes.value(HEIGHT_DEF).toFloat());
   int level = attributes.value(LEVEL_DEF).toInt();
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o = ITEMS_ORIENTATION::O_VERTICAL_TOP;
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   if(attributes.hasAttribute(ORIENTATION_SHORT_DEF))
       o = static_cast<ITEMS_ORIENTATION>(attributes.value(ORIENTATION_SHORT_DEF).toString().toInt());
   p = createPackage(x,y,w,h,static_cast<BOARD_LEVEL_ID>(level),o,id,iNoZoom,iNoZoom);
   return p;
}

SmartPtr<GraphicalItem> ItemsFactory::createStdDip(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(N_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF) ||
      !attributes.hasAttribute(ORIENTATION_DEF) ||
      !attributes.hasAttribute(NAME_DEF))      
      return p;
   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return p;
   int n = attributes.value(N_DEF).toInt();
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   auto orient = attributes.value(ORIENTATION_DEF).toString();
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o_t = getOrientationStr(orient);
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);   
   p = createStdDip(x * zoom_d/zoom,y * zoom_d/zoom,o_t,n,level,id,zoom,zoom_d);
   return p;
}


SmartPtr<GraphicalItem> ItemsFactory::createStdDip(float x, float y,ITEMS_ORIENTATION type ,int n, BOARD_LEVEL_ID level,
                                          ITEM_ID id,int k_zoom,int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   items.reserve(static_cast<std::vector<GraphicalItem*>::size_type>(n + 1));
   float height = DIP_STEP * n/2;
   float width = getWidthForDipPack(n);
   float width1 = getFullWidthForDipPack(n);
   items.push_back(SmartPtr<GraphicalItem>::make_smartptr<PackageGraphicalItem>(LevelsWrapper::geLevelForDip(level),
                                             PointF(x,y),width1,height,type,
                                             id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE));
   
   for(int j = 0; j < 2; ++j)
   {
      for(int i = 0; i < n/2 ; ++i)
      {
          float xx,yy;
          if(type == O_VERTICAL_BOTTOM || type == O_VERTICAL_TOP)
          {
             yy = y - height/2 + DIP_STEP/2 + i * DIP_STEP;
             if(j > 0)
                xx = x + width/2;
             else
                xx = x - width/2;
          }
          else
          {
             xx = x - height/2 + DIP_STEP/2 + i * DIP_STEP;
             if(j > 0)
                yy = y + width/2;
             else
                yy = y - width/2;
          }

          SmartPtr<GraphicalItem> ptr = SmartPtr<GraphicalItem>::make_smartptr<RoundPlateGraphicalItem>(xx,yy,
                                                               getExternalDForDipPlate(),
                                                               getInternalDForDipPlate(),level,
                                                               id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE);
          items.push_back(ptr);
      }
   }
   
   SmartPtr<GraphicalItem> p = createContainer(x,y,level,items,id);
   static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
   return p;
}


SmartPtr<GraphicalItem> ItemsFactory::createRectPackage(float x, float y, float w, float h, BOARD_LEVEL_ID level,
                                               ITEMS_ORIENTATION o,ITEM_ID id, int k_zoom,int zoom_d,bool filled)
{
   return SmartPtr<GraphicalItem>::make_smartptr<RectPackageGraphicalItem>(level,PointF(x,y),w,h,o,id,filled);
}

//Draws ellipse with d and d1 . When d = d1 the circle will be drawn
SmartPtr<GraphicalItem> ItemsFactory::createRoundPackage(float x, float y, float d, float d1,BOARD_LEVEL_ID level,ITEMS_ORIENTATION o,
                                                ITEM_ID id,int k_zoom,int zoom_d)
{
   return SmartPtr<GraphicalItem>::make_smartptr<RoundPackageGraphicalItem>(level,x,y,d,d1,o,id);
}

SmartPtr<GraphicalItem> ItemsFactory::createElCapacitor(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(TYPE_DEF) ||      
      !attributes.hasAttribute(NAME_DEF))
      return p;
   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return p;
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);   
   
   float d = attributes.value(WIDTH_DEF).toFloat();
   int level = attributes.value(LEVEL_DEF).toInt();
   name = attributes.value(NAME_DEF).toString();
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   p = createElCapacitor(x,y, ITEMS_ORIENTATION::O_HORIZONTAL_LEFT, d,static_cast<BOARD_LEVEL_ID>(level),id,zoom,zoom_d);
   return p;
}


SmartPtr<GraphicalItem> ItemsFactory::createElCapacitor(float x, float y, ITEMS_ORIENTATION type,
                                               float d, BOARD_LEVEL_ID level,ITEM_ID id, int k_zoom,int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   items.reserve(4);
   float f = getFforElCapacitor(static_cast<int>(d) );
   items.push_back(createRoundPackage(x,y,d,d,LevelsWrapper::geLevelForDip(level),type,
                                      id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));
   
   items.push_back(createCapSchematic(x,y,type,f,LevelsWrapper::geLevelForDip(level),id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));
   //horizontal orientation
   items.push_back(createRoundPlate(x - f/2,y,CAP_PLATE_D_EX,
                                         CAP_PLATE_D_IN,
                                         level,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));
   items.push_back(createRoundPlate(x + f/2,y,CAP_PLATE_D_EX,
                                         CAP_PLATE_D_IN,
                                         level,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));


   SmartPtr<GraphicalItem> p = SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,std::move(items),id);
   static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
   return p;
}

SmartPtr<GraphicalItem> ItemsFactory::createResistor(float x, float y, ITEMS_ORIENTATION type,
                                            float l, BOARD_LEVEL_ID level,ITEM_ID id, int k_zoom,int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   items.reserve(3);
   float h = getDforResistor(static_cast<int>(l));
   items.push_back(createRectPackage(x,y,l,h,
                                       LevelsWrapper::geLevelForDip(level),type,
                                        id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));
   //horizontal orientation
   items.push_back(createRoundPlate(x - l/2 - CAP_PLATE_D_EX/2 -
                                         pklnwf,
                                         y,
                                         CAP_PLATE_D_EX,
                                         CAP_PLATE_D_IN,
                                         level,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));
   items.push_back(createRoundPlate(x + l/2 + CAP_PLATE_D_EX/2,
                                         y,
                                         CAP_PLATE_D_EX,
                                         CAP_PLATE_D_IN,
                                         level,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));

   SmartPtr<GraphicalItem> p = SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,
                                                                                                     std::move(items),id);
   static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
   return p;
}

SmartPtr<GraphicalItem> ItemsFactory::createResistor(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(TYPE_DEF) ||      
      !attributes.hasAttribute(NAME_DEF))
      return p;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return p;
   
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);   
   
   float l = roundFloat(attributes.value(WIDTH_DEF).toFloat());
   int level = attributes.value(LEVEL_DEF).toInt();
   name = attributes.value(NAME_DEF).toString();
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   p = createResistor(x,y,O_HORIZONTAL_LEFT,l,static_cast<BOARD_LEVEL_ID>(level),id,zoom,zoom_d);
   return p;

}


//smd passive elements
SmartPtr<GraphicalItem> ItemsFactory::createSmdType(float x, float y, ITEMS_ORIENTATION type, SMD_PACKAGE_TYPE pack_type,
                                           BOARD_LEVEL_ID level,ITEM_ID id, int k_zoom,int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   items.reserve(3);
   PointF pt = getSizeOfSmfPack(pack_type);

   items.push_back(createRectPackage(x,y,pt.x(),pt.y(),
                                       LevelsWrapper::geLevelForSO(level),type,
                                        id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,k_zoom,zoom_d));
   float plate_h = pt.y();
   float plate_w = pt.x()/3;
   //vertical orientation
   items.push_back(createRect(x - pt.x()/2,
                                         y - 0.1f,
                                         plate_w,
                                         plate_h,
                                         level,
                                         type == O_VERTICAL_TOP || type == O_VERTICAL_BOTTOM ? O_HORIZONTAL_RIGHT : O_VERTICAL_TOP,
                                         id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,
                                         k_zoom,zoom_d));
   items.push_back(createRect(x + pt.x()/2,
                                         y - 0.1f,
                                         plate_w,
                                         plate_h,
                                         level,
                                         type == O_VERTICAL_TOP || type == O_VERTICAL_BOTTOM ? O_HORIZONTAL_RIGHT : O_VERTICAL_TOP,
                                         id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,
                                         k_zoom,zoom_d));
   
   SmartPtr<GraphicalItem> p = SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,std::move(items),id);
   static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
   return p;

}

SmartPtr<GraphicalItem> ItemsFactory::createSmdType(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(TYPE_DEF) ||      
      !attributes.hasAttribute(NAME_DEF))
      return p;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return p;
   
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);   
   
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   QString type = attributes.value(TYPE_DEF).toString();
   name = attributes.value(NAME_DEF).toString();
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   p = createSmdType(x * zoom_d/zoom, y * zoom_d/zoom,O_HORIZONTAL_LEFT,getSmdPackForStr(type),level,id,zoom,zoom_d);
   return p;

}

SmartPtr<GraphicalItem> ItemsFactory::createCapSchematic(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> p;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(TYPE_DEF) ||      
      !attributes.hasAttribute(NAME_DEF))
      return p;
   
   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return p;
   
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);   
      
   float width = roundFloat(attributes.value(WIDTH_DEF).toFloat());
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   string type = attributes.value(TYPE_DEF).toString().toStdString();
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o = ITEMS_ORIENTATION::O_VERTICAL_TOP;
   if(attributes.hasAttribute(ORIENTATION_SHORT_DEF))
      o = static_cast<ITEMS_ORIENTATION>(attributes.value(ORIENTATION_SHORT_DEF).toInt());
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   
   p = createCapSchematic(x,y,o,width,level,id,zoom,zoom_d);
   return p;
    
}


SmartPtr<GraphicalItem> ItemsFactory::createCapSchematic(float x, float y, ITEMS_ORIENTATION o, float f, BOARD_LEVEL_ID level,ITEM_ID id,
                                                int k_zoom,int zoom_d)
{
   SmartPtr<GraphicalItem> ptr = SmartPtr<GraphicalItem>::make_smartptr<CapGraphicalItem>(x,y,f,level,id);
   ptr->setType(o);
   return ptr;
}

SmartPtr<GraphicalItem> ItemsFactory::createCap(float x, float y, ITEMS_ORIENTATION type, float f, BOARD_LEVEL_ID level,
                                       ITEM_ID id,int k_zoom,int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   items.reserve(3);

   items.push_back(createCapSchematic(x,y,type,f,LevelsWrapper::geLevelForDip(level),id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE
                                                                           ,k_zoom,zoom_d));
   //horizontal orientation
   items.push_back(createRoundPlate(x - f/2,y,CAP_PLATE_D_EX,
                                         CAP_PLATE_D_IN,
                                         level,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,
                                         k_zoom,zoom_d));
   items.push_back(createRoundPlate(x + f/2,y,CAP_PLATE_D_EX,
                                         CAP_PLATE_D_IN,
                                         level,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,
                                         k_zoom,zoom_d));

   SmartPtr<GraphicalItem> p = SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,std::move(items),id);
   static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
   return p;

}

SmartPtr<GraphicalItem> ItemsFactory::createCap(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> ptr;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(TYPE_DEF) ||      
      !attributes.hasAttribute(NAME_DEF))
      return ptr;
   
   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return ptr;
   
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);   
      
   float width = roundFloat(attributes.value(WIDTH_DEF).toFloat());
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   string type = attributes.value(TYPE_DEF).toString().toStdString();
   name = attributes.value(NAME_DEF).toString();
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   ptr = createCap(x, y, O_HORIZONTAL_LEFT,width,level,id,zoom,zoom_d);
   return ptr;
}


SmartPtr<GraphicalItem> ItemsFactory::createRect(float x, float y, float w, float h, BOARD_LEVEL_ID level, ITEMS_ORIENTATION o,ITEM_ID id, int k_zoom,int zoom_d)
{
   return SmartPtr<GraphicalItem>::make_smartptr<RectGraphicalItem>(x,y,w,h,level,o,id);
}

SmartPtr<GraphicalItem> ItemsFactory::createStdSO(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> ptr;
   if(!attributes.hasAttribute(N_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF) ||
      !attributes.hasAttribute(ORIENTATION_DEF) ||
      !attributes.hasAttribute(NAME_DEF))      
      return ptr;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return ptr;
   int n = attributes.value(N_DEF).toString().toInt();
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   auto orient = attributes.value(ORIENTATION_DEF).toString();
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o_t = getOrientationStr(orient);
   int zoom = 1;
   int zoom_d = 1;
   resolveZoom(attributes,zoom,zoom_d);
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;

   ptr = createStdSO(x * zoom_d/zoom,y * zoom_d/zoom,o_t,n,level,id,zoom,zoom_d);
   return ptr;

}

//x,y - coordinates of center, n - number of contacts,level - PCB layer,k_zoom - the value of zoom to multiply,
//zoom_d - the value of zoom to divide with.
//the resulting x_coord = x * k_zoom/zoom_d
SmartPtr<GraphicalItem> ItemsFactory::createStdSO(float x, float y, ITEMS_ORIENTATION type,int n, BOARD_LEVEL_ID level,
                                         ITEM_ID id,int k_zoom,int zoom_d)
{
   vector<SmartPtr<GraphicalItem> > items;
   items.reserve(static_cast<std::vector<GraphicalItem*>::size_type>(n + 1));
   float height = getHeightForSOPack(n);
   float width = (getWidthForSOPack(n) - 0.5f);
   items.push_back(SmartPtr<GraphicalItem>::make_smartptr<PackageGraphicalItem>(LevelsWrapper::geLevelForSO(level),
                                            PointF(x,y),width,height,type,
                                            id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE));

   for(int j = 0; j < 2; ++j)
   {
      for(int i = 0; i < n/2 ; ++i)
      {
          float xx,yy,w,h;
          if(type == O_VERTICAL_BOTTOM || type == O_VERTICAL_TOP)
          {
             yy = y - height/2 + DIP_STEP/4 + i * DIP_STEP/2;
             if(j > 0)
                xx = x + width/2 + (DIP_STEP - 0.5f)/2 + pklnwf;
             else
                xx = x - width/2 - (DIP_STEP - 0.5f)/2 - pklnwf;
             w = DIP_STEP - 0.5f;
             h = SO_PLATE_HEIGHT;
          }
          else
          {
             xx = x - height/2 + DIP_STEP/4 + i * DIP_STEP/2;
             if(j > 0)
                yy = y + width/2 + (DIP_STEP - 0.5f)/2 + pklnwf;
             else
                yy = y + width/2 - (DIP_STEP - 0.5f)/2 - pklnwf;;
             h = DIP_STEP - 0.5f;
             w = SO_PLATE_HEIGHT;
          }
          items.push_back(createRect(xx,yy,w,h,level,type,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,
                                     k_zoom,zoom_d));
      }
   }

   SmartPtr<GraphicalItem> p = SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,
                                                                                                      std::move(items),id);

   static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
   return p;
}

SmartPtr<GraphicalItem> ItemsFactory::createRect(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> ptr;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(HEIGHT_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF))
      return ptr;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return ptr;
   
   float w = roundFloat(attributes.value(WIDTH_DEF).toFloat());
   float h = roundFloat(attributes.value(HEIGHT_DEF).toFloat());

   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o = ITEMS_ORIENTATION::O_VERTICAL_TOP;
   if(attributes.hasAttribute(ORIENTATION_SHORT_DEF))
      o = static_cast<ITEMS_ORIENTATION>(attributes.value(ORIENTATION_SHORT_DEF).toInt());
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;

   ptr = SmartPtr<GraphicalItem>::make_smartptr<RectGraphicalItem>(x,y,w,h,level,o,id);

   return ptr;
}

SmartPtr<GraphicalItem> ItemsFactory::createRectPackage(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> ptr;
   bool filled = false;
   if(!attributes.hasAttribute(WIDTH_DEF)  ||
      !attributes.hasAttribute(HEIGHT_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF))
      return ptr;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return ptr;
   
   float w = roundFloat(attributes.value(WIDTH_DEF).toFloat());
   float h = roundFloat(attributes.value(HEIGHT_DEF).toFloat());
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o = ITEMS_ORIENTATION::O_VERTICAL_TOP;
   if(attributes.hasAttribute(ORIENTATION_SHORT_DEF))
      o = static_cast<ITEMS_ORIENTATION>(attributes.value(ORIENTATION_SHORT_DEF).toInt());
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   if(attributes.hasAttribute(FILLED_SHORT_DEF))
      filled = (attributes.value(FILLED_SHORT_DEF).toInt() == 1 ? true : false);
   
   ptr = createRectPackage(x,y,w,h,level,o,id,iNoZoom,iNoZoom,filled);
   return ptr;
}

SmartPtr<GraphicalItem> ItemsFactory::createRoundPackage(QXmlStreamAttributes attributes, QString& name)
{
   SmartPtr<GraphicalItem> ptr;
   if(!attributes.hasAttribute(D_EX_DEF)  ||
      !attributes.hasAttribute(LEVEL_DEF) ||
      !attributes.hasAttribute(NAME_DEF))
      return ptr;

   float x,y;
   if(!resolveCoordinates(attributes,x,y))
      return ptr;
   
   bool bCircle = false;
   float d_in = 0.0f;
   float d_ex = roundFloat(attributes.value(D_EX_DEF).toFloat());
   if (!attributes.hasAttribute(D_IN_DEF))
       bCircle = true;
   else
       d_in = roundFloat(attributes.value(D_IN_DEF).toFloat());
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
   name = attributes.value(NAME_DEF).toString();
   ITEMS_ORIENTATION o = ITEMS_ORIENTATION::O_VERTICAL_TOP;
   if(attributes.hasAttribute(ORIENTATION_SHORT_DEF))
      o = static_cast<ITEMS_ORIENTATION>(attributes.value(ORIENTATION_SHORT_DEF).toInt());
   ITEM_ID id = name.toInt();
   if(!id)
      id = ID_NONE;
   // when d_in is 0 this means that this was not written in attributes then this is circle and d_in = d_ex
   ptr = createRoundPackage(x,y,d_ex,bCircle ? d_ex : d_in,level,o,id,iNoZoom,iNoZoom);
   if(attributes.hasAttribute(ANGLE_ST_DEF))
       static_cast<RoundPackageGraphicalItem*>(ptr.get())->setStartAngle(
               attributes.value(ANGLE_ST_DEF).toInt());
   if(attributes.hasAttribute(ANGLE_SP_DEF))
       static_cast<RoundPackageGraphicalItem*>(ptr.get())->setSpanAngle(
               attributes.value(ANGLE_SP_DEF).toInt());
   if(attributes.hasAttribute(CHORD))
       static_cast<RoundPackageGraphicalItem*>(ptr.get())->setClosed(
               attributes.value(CHORD).toInt());

   return ptr;

}


SmartPtr<GraphicalItem> ItemsFactory::duplicateItem(GraphicalItem *p, float x,float y,bool bIgnoreCoord)
{
   SmartPtr<GraphicalItem> p1 = p->clone();
   if(!bIgnoreCoord)
   {
      if(GenericGraphicalItemsContainer *p2 = dynamic_cast<GenericGraphicalItemsContainer*>(p1.get()))
      {
         //if the content of container which is cloned has absolute coordinates we need
         //to recalculate them for new container, because we set new coordinates
         //for new container and absolute coordinates of content of cloned object do not
         //make sense for the content of the new container
         if(!p2->isParent())
            p2->setAsParent(true,true);
      }
      else if(ConnectorGraphicalItem *p2 = dynamic_cast<ConnectorGraphicalItem*>(p1.get()))
      {
         PointF pt = p2->getArea();
         PointF dif = PointF(x- pt.x(),y - pt.y());
         p2->moveFor(p2->getLevel(),dif);
      }
      p1->setX(x);
      p1->setY(y);
   }
   return p1;
}

SmartPtr<GraphicalItem> ItemsFactory::createStdTSOP(float x, float y, ITEMS_ORIENTATION type, int n, BOARD_LEVEL_ID level,
                                           ITEM_ID id,int k_zoom, int zoom_d)
{
    vector<SmartPtr<GraphicalItem> > items;
    float height = getHeightForTSOPPack(n);
    float width = getWidthForTSOPPack(n);
    items.push_back(SmartPtr<GraphicalItem>::make_smartptr<PackageGraphicalItem>(LevelsWrapper::geLevelForSO(level),
                                             PointF(x,y),width,height,type,
                                             id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE));

    float D = 1.0f;
    float  c = .0f;
    if(n == 8)
        c = 0.33f;
    else
        c = 0.2f;

    for(int j = 0; j < 2; ++j)
    {
       float w,h;
       for(int i = 0; i < n/2 ; ++i)
       {
           float xx,yy;
           if(type == O_VERTICAL_BOTTOM || type == O_VERTICAL_TOP)
           {
              w = D;
              h = TSSOP_PLATE_HEIGHT;
              yy = y - height/2 + c + 0.65f * i;
              if(j > 0)
                 xx = x + width/2 + D/2 + pklnwf;
              else
                 xx = x - width/2 - D/2 - pklnwf;
           }
           else
           {
              w = TSSOP_PLATE_HEIGHT;
              h = D;
              xx = x - height/2 + c + 0.65f * i;
              if(j > 0)
                 yy = y + width/2 + D/2 + pklnwf;
              else
                 yy = y - width/2 - D/2 - pklnwf ;
           }

           items.push_back(createRect(xx,yy,w,h,level,type,id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE,
                                      k_zoom,zoom_d));
       }
    }
    SmartPtr<GraphicalItem> p = SmartPtr<GraphicalItem>::make_smartptr<GenericGraphicalItemsContainer>(x,y,level,std::move(items),id);
    static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
    return p;
}

SmartPtr<GraphicalItem> ItemsFactory::createStdTSOP(QXmlStreamAttributes attributes, QString& name)
{
    SmartPtr<GraphicalItem> ptr;
    if(!attributes.hasAttribute(N_DEF)  ||
       !attributes.hasAttribute(LEVEL_DEF) ||
       !attributes.hasAttribute(NAME_DEF) ||
       !attributes.hasAttribute(ORIENTATION_DEF) ||
       !attributes.hasAttribute(NAME_DEF))
       return ptr;

    float x,y;
    if(!resolveCoordinates(attributes,x,y))
       return ptr;
    int n = attributes.value(N_DEF).toInt();
    BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toInt());
    auto orient = attributes.value(ORIENTATION_DEF).toString();
    name = attributes.value(NAME_DEF).toString();
    ITEMS_ORIENTATION o_t = getOrientationStr(orient);
    int zoom = 1;
    int zoom_d = 1;
    resolveZoom(attributes,zoom,zoom_d);
    ITEM_ID id = name.toInt();
    if(!id)
       id = ID_NONE;
    ptr = createStdTSOP(x * zoom_d/zoom,y * zoom_d/zoom,o_t,n,level,id,zoom,zoom_d);
    return ptr;

}

SmartPtr<GraphicalItem> ItemsFactory::createText(float x, float y, QString& text, int fntSize,ITEMS_ORIENTATION o,
                                                 BOARD_LEVEL_ID level, ITEM_ID id,int k_zoom,int k_zoom_d)
{
     return SmartPtr<GraphicalItem>::make_smartptr<TextGraphicalItem>(x,y,text,fntSize,o,level,id);

}
SmartPtr<GraphicalItem> ItemsFactory::createText(QXmlStreamAttributes attributes,QString& name)
{
    SmartPtr<GraphicalItem> p;
    if(!attributes.hasAttribute(X_DEF) ||
       !attributes.hasAttribute(Y_DEF) ||
       !attributes.hasAttribute(TXT_CONT_DEF)  ||
       !attributes.hasAttribute(TXT_FNT_SZ_DEF)  ||
       !attributes.hasAttribute(LEVEL_DEF) ||
       !attributes.hasAttribute(NAME_DEF))
       return p;

    float x = roundFloat(attributes.value(X_DEF).toFloat());
    float y = roundFloat(attributes.value(Y_DEF).toFloat());
    int level = attributes.value(LEVEL_DEF).toInt();
    name = attributes.value(NAME_DEF).toString();
    QString text = attributes.value(TXT_CONT_DEF).toString();
    int fontSize = attributes.value(TXT_FNT_SZ_DEF).toInt();
    ITEM_ID id = name.toInt();
    if(!id)
       id = ID_NONE;

   ITEMS_ORIENTATION o_t = ITEMS_ORIENTATION::O_HORIZONTAL_LEFT;
    if(attributes.hasAttribute(ORIENTATION_SHORT_DEF))
       o_t = static_cast<ITEMS_ORIENTATION>(attributes.value(ORIENTATION_SHORT_DEF).toString().toInt());

    p = createText(x,y,text,fontSize, o_t, static_cast<BOARD_LEVEL_ID>(level),id,iNoZoom,iNoZoom);
    return p;
}

SmartPtr<GraphicalItem> ItemsFactory::createGenericChip(bool dip,          //is the dip type?
                                                 double x,
                                                 double y,
                                                 double pinsDist, // distance between pins
                                                 double pinPackDist, //distance between pin and package
                                                 double width, //package width
                                                 double height, //package height
                                                 BOARD_LEVEL_ID idLevel, // layer
                                                 ITEM_ID id,  // ID of item if any
                                                 ITEMS_ORIENTATION o, //orientation
                                                 array<int,4>& n //pins number for each side
)
{
    auto num = 0;
    for(auto it:n)
       num += it;

    auto pinHeight = abs(pinPackDist) * 2;
    auto pinWidth = pinsDist * 0.7;

    vector<SmartPtr<GraphicalItem>> itemsVector;
    //anticlockwise
    ITEMS_ORIENTATION po[4] = {O_HORIZONTAL_LEFT,O_VERTICAL_BOTTOM,O_HORIZONTAL_RIGHT,O_VERTICAL_TOP};
    if(o == ITEMS_ORIENTATION::O_HORIZONTAL_LEFT|| o == ITEMS_ORIENTATION::O_HORIZONTAL_RIGHT )
    {
       po[0] = O_VERTICAL_BOTTOM;
       po[1] = O_HORIZONTAL_RIGHT;
       po[2] = O_VERTICAL_TOP;
       po[3] = O_HORIZONTAL_LEFT;
    }
    itemsVector.push_back(SmartPtr<GraphicalItem>::make_smartptr<PackageGraphicalItem>(dip? LevelsWrapper::geLevelForDip(idLevel) :
                                                                                            LevelsWrapper::geLevelForSO(idLevel),
                                              PointF(x,y),width,height,o,
                                              id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE));


    for (size_t i = 0; i < n.size(); ++i)
    {
       if(n[i] > 0)
       {
          list<PointF> points;
          calculatePointsForPins(points,po[i],n[i],pinsDist,pinPackDist,width,height);
          for(auto& coord:points)
          {

             if(dip)
             {
                 itemsVector.push_back(SmartPtr<GraphicalItem>::make_smartptr<RoundPlateGraphicalItem>(coord.x(),coord.y(),
                                                                  pinsDist*0.7,pinsDist*0.7/3,idLevel,
                                                                  id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE));
                 itemsVector.back()->setType(po[i]);
             }
             else
             {
                 itemsVector.push_back(SmartPtr<GraphicalItem>::make_smartptr<RectGraphicalItem>(coord.x(),coord.y(),pinWidth,pinHeight,idLevel,
                                                               po[i],
                                                               id != ID_NONE ? IDsGenerator::instance()->getNewID() : ID_NONE));
             }
          }
       }
    }
    SmartPtr<GraphicalItem> p = createContainer(x,y,idLevel,itemsVector,id);
    static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(true,true);
    return p;
}
ItemsFactory::~ItemsFactory()
{

}
