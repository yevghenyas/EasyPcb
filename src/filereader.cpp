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

#include "filereader.h"
#include <iostream>
#include "itemsfactory.h"
#include "connectorgraphicalitem.h"



FileReader::FileReader()
{

}

void FileReader::setDevice(QIODevice *pDevice)
{
   m_reader.setDevice(pDevice);
}

SchemData FileReader::parseItems(vector< SmartPtr<GraphicalItem> >& items, vector< QString >& names)
{
   SchemData data;
   //set default board width and height
   data.m_width = NEW_SCHEM_DEF_WIDTH * PIXELS_PER_MM;
   data.m_height = NEW_SCHEM_DEF_HEIGHT * PIXELS_PER_MM;
   map<ITEM_ID,SmartPtr<GraphicalItem>> idToPtrMap;
   while (!m_reader.atEnd())
   {
      QXmlStreamReader::TokenType token = m_reader.readNext();
      if (token == QXmlStreamReader::StartDocument)
         continue;

      if (token == QXmlStreamReader::StartElement)
      {
         if(m_reader.attributes().size() > 0)
         {
            QXmlStreamAttributes attributes = m_reader.attributes();
            if(m_reader.name().compare(BD_STRT_EL) == 0)
            {
               if(m_reader.attributes().hasAttribute(WIDTH_DEF)
                   && m_reader.attributes().hasAttribute(HEIGHT_DEF))
               {
                 //read width and height of board
                 data.m_width = m_reader.attributes().value(WIDTH_DEF).toString().toInt();
                 data.m_height = m_reader.attributes().value(HEIGHT_DEF).toString().toInt();
               }
               continue;
            }
            QString name;
            QString type;
//            GraphicalItem* p = readElement(attributes,name,type);
            ElementInfo info;
            QString elemName;
            auto p = parseOneElem(name,type,elemName,info,token,false,&idToPtrMap);
            if(p.get())
            {
               items.push_back(p);
               names.push_back(name);
               GenericGraphicalItemsContainer *pCon = nullptr;
               if((pCon = dynamic_cast<GenericGraphicalItemsContainer*>(p.get())) != nullptr)
               {
                  auto pChildren = pCon->getChildren();
                  for(auto& item:*pChildren)
                     idToPtrMap[item->getID()] = item;
               }
               else
                  idToPtrMap[p->getID()] = p;
            }
         }
      }
   }
   return data;
}

void FileReader::findConnectedItem(map<ITEM_ID,SmartPtr<GraphicalItem>>* idToPtr,
                                   map<ITEM_ID,int>* ids_connected,
                                   ConnectorGraphicalItem *pConnector)
{
   for(auto& item:*ids_connected)
   {
      auto indToItem = idToPtr->find(item.first);
      if(indToItem != idToPtr->end())
      {
         pConnector->insertConnectedNode(item.second,indToItem->second);
         indToItem->second->connect(pConnector->getID());
      }
   }
}


//The name of the item in the library file is always descriptive name( DIP8,DIP16 etc...)
//The name of the item in the file of schematics is always digital(which is id of the item in the chematic)
//Before creating the item we need to check if the name can be converted into digital.
//This is needed when creating container
SmartPtr<GraphicalItem> FileReader::parseOneElem(QString& name,QString& type,
                                                 QString& xmlElemName,ElementInfo& info,
                                        QXmlStreamReader::TokenType& t,bool stdLib,map<ITEM_ID,SmartPtr<GraphicalItem>> *idToPtr)
{
   vector<PointF> m_points;
   map<ITEM_ID,int> ids_connected; //ids of electrically connected items
   ITEM_ID conn_id = ID_NONE; //individual connector id to be added to ids_connected
   int conn_ind; // index of connected node of schematic connector
   vector<SmartPtr<GraphicalItem> > m_items;
   QString itemName;
   float conn_w = 0.0f;
   LINE_STYLE conn_st = LINE_STYLE::LINE_ROUNDED;
   float x_container = 0.0f;
   float y_container = 0.0f;
   bool container_is_parent = false;
   SmartPtr<GraphicalItem> p;
   PointF pt;
   QXmlStreamAttributes attributes = m_reader.attributes();
   if(!attributes.hasAttribute(NAME_DEF) ||
      !attributes.hasAttribute(TYPE_DEF) ||
      !attributes.hasAttribute(LEVEL_DEF))
      return p;
   name = attributes.value(NAME_DEF).toString();
   type = attributes.value(TYPE_DEF).toString();
   BOARD_LEVEL_ID level = static_cast<BOARD_LEVEL_ID>(attributes.value(LEVEL_DEF).toString().toInt());
   xmlElemName = m_reader.name().toString();
   QXmlStreamReader::TokenType token = t;
   CONNECTOR_TYPE conType = CONNECTOR_TYPE::BOARD;
   cout <<"xmlElemName:"<<xmlElemName.toStdString()<<endl;
   while(true)
   {
      if(token == QXmlStreamReader::StartElement )
      {
         QString str = m_reader.name().toString();
         cout <<"Start element:"<<str.toStdString()<<endl;         
         if(str.compare(PLATE_RND_DEF) == 0)
         {
            p = ItemsFactory::createRoundPlate(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;
         }
         else if(str.compare(PLATE_RECT_DEF) == 0)
         {
            p = ItemsFactory::createRect(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;            
         }
         else if(str.compare(PLATE_CNT) == 0)
         {
            if(!attributes.hasAttribute(NAME_DEF))
            {
               p.reset(nullptr);
               return p;
            }
            conn_id = attributes.value(NAME_DEF).toInt();
            conn_ind = attributes.value("Ind").toInt();
         }
         else if(str.compare(PACKAGE_DEF) == 0)
         {            
            p = ItemsFactory::createPackage(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;
         }
         else if(str.compare(PACKAGE_RECT_DEF) == 0)
         {
            p = ItemsFactory::createRectPackage(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;            
         }
         else if(str.compare(PACKAGE_RND_DEF) == 0)
         {
            p = ItemsFactory::createRoundPackage(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;            
         }
         else if(str.compare(TEXT_DEF) == 0)
         {
            p = ItemsFactory::createText(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;
         }

         else if(str.compare(CAPACITOR_DEF) == 0)
         {
            p = ItemsFactory::createCap(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;
            
         }
         else if(str.compare(CAP_SCEMATIC_DEF) == 0)
         {
            p = ItemsFactory::createCapSchematic(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;
            
         }         
         else if(str.compare(RESISTOR_DEF) == 0)
         {
            p = ItemsFactory::createResistor(attributes,itemName);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;
            static_cast<GenericGraphicalItemsContainer*>(p.get())->setAsParent(p.get(),false);
         }
         else if(str.compare(EL_CAPACITOR_DEF) == 0)
         {
            p = ItemsFactory::createElCapacitor(attributes,name);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;            
         }
         else if(str.compare(SMD_PASSIVE_DEF) == 0)
         {
            p = ItemsFactory::createSmdType(attributes,name);
            info.setGraphicalItem(p);
            if(!p.get())
               return p;            
         }         
         else if(str.compare(DIP_CHIP_DEF) == 0)
         {
            info.setGraphicalItem(p);
            if(stdLib)
               p = ItemsFactory::createStdDip(attributes,name);//iPreviewSizeX/2,iPreviewSizeY/2);
            else
               p = ItemsFactory::createStdDip(attributes,name);
            
            info.setGraphicalItem(p);
         }
         else if(str.compare(SO_CHIP_DEF) == 0)
         {
            if(stdLib)          
               p = ItemsFactory::createStdSO(attributes,name);//iPreviewSizeX/2,iPreviewSizeY/2);
            else
               p = ItemsFactory::createStdSO(attributes,name);
            
            info.setGraphicalItem(p);
         }         
         else if(str.compare(MSSOP_CHIP_DEF) == 0 ||
                 str.compare(TSSOP_CHIP_DEF) == 0)
         {
            if(stdLib)
               p = ItemsFactory::createStdTSOP(attributes,name);//iPreviewSizeX/2,iPreviewSizeY/2);
            else
               p = ItemsFactory::createStdTSOP(attributes,name);

            info.setGraphicalItem(p);
         }
         else if(str.compare(POINT_DEF) == 0)
         {
            if(!attributes.hasAttribute(X_DEF) ||      
               !attributes.hasAttribute(Y_DEF))
            {
               p.reset(nullptr);
               return p;
            }
            //no need to check if parent element is connector
            //because point can only be a part of connector
            pt.setX(attributes.value(X_DEF).toString().toFloat());
            pt.setY(attributes.value(Y_DEF).toString().toFloat());
         }         
         else if(str.compare(CONTAINER_DEF) == 0)
         {
            if(!attributes.hasAttribute(X_DEF))
               x_container = 0;
            else
               x_container = attributes.value(X_DEF).toString().toFloat();
            
            if(!attributes.hasAttribute(Y_DEF))
               y_container = 0;
            else
               y_container = attributes.value(Y_DEF).toString().toFloat();
            
            if(attributes.hasAttribute(PARENT_DEF))
               container_is_parent = true;


            //continue accumulating the content of container
            
         }
         else if(str.compare(CONNECTOR_DEF) == 0)
         {
            if(!attributes.hasAttribute(WIDTH_DEF))
            {
               p.reset(nullptr);
               return p;
            }
            conn_w = attributes.value(WIDTH_DEF).toString().toFloat();
            if(attributes.hasAttribute(STYLE_DEF))
                conn_st = static_cast<LINE_STYLE>(attributes.value(STYLE_DEF).toInt());
            if(attributes.hasAttribute(EL_OR_BOARD_DEF))
                conType = static_cast<CONNECTOR_TYPE>(attributes.value(EL_OR_BOARD_DEF).toInt());
            //continue accumulating the content of connector;
         }         
      }
      else if (token == QXmlStreamReader::EndElement )
      {
         QString str = m_reader.name().toString();
         cout <<"End element:"<<str.toStdString()<<endl;
         if(m_reader.name().compare(CONNECTOR_DEF) == 0)
         {
            ITEM_ID id = name.toInt();
            if(!id)
                id = ID_NONE;
            if(m_points.size() > 0)
            {
               auto pCon = ItemsFactory::createConnector(&m_points,conn_w,(BOARD_LEVEL_ID)level,conType,
                                                         id,iNoZoom,iNoZoom);
               ConnectorGraphicalItem *pConnector = static_cast<ConnectorGraphicalItem*>(pCon.get());
               //connected items
               if(idToPtr != nullptr)
               {
                  findConnectedItem(idToPtr,&ids_connected,pConnector);
/*
                  for(auto item:ids_connected)
                  {
                     auto indToItem = idToPtr->find(item.first);
                     if(indToItem != idToPtr->end())
                     {
                        pConnector->insertConnectedNode(item.second,indToItem->second);
                        indToItem->second->connect(pConnector->getID());
                     }
                  }
*/
               }
               return pCon;
            }
            else
            {
               p.reset(nullptr);
               return p;
            }
         }
         else if(m_reader.name().compare(CONTAINER_DEF) == 0)
         {
            if(m_items.size() > 0)
            {
                ITEM_ID id = name.toInt();
                if(!id)
                    id = ID_NONE;
                auto pC =
                        ItemsFactory::createContainer(x_container,y_container,
                                                                     level,m_items,id);
               static_cast<GenericGraphicalItemsContainer*>(pC.get())->setAsParent(container_is_parent,false);
               return pC;
            }
            else
            {
               p.reset(nullptr);
               return p;
            }
         }
         else if(xmlElemName.compare(CONNECTOR_DEF) == 0)
         {
            if(m_reader.name().compare(POINT_DEF) == 0)
               m_points.push_back(pt);
            if(m_reader.name().compare(PLATE_CNT) == 0)
               ids_connected[conn_id] = conn_ind;
         }
         else if(xmlElemName.compare(CONTAINER_DEF) == 0)         
         {
            if(p.get())
            {
               m_items.push_back(p);
               p.reset(nullptr);
            }               
         }
         else if(!m_reader.name().isEmpty())
         {
            cout <<"End element returning:"<<str.toStdString()<<endl;            
            return p;
         }
      }
      else
      {
         QString str = m_reader.name().toString();
         cout <<"End element:"<<str.toStdString()<<endl;         
      }         
      token = m_reader.readNext();      
      attributes = m_reader.attributes();
   }
}

/*
GraphicalItem* FileReader::readElement(QXmlStreamAttributes& attributes, string& name,string& t,bool stdLib)
{
    if(attributes.empty())
       return NULL;
    if(attributes.hasAttribute(TYPE_DEF))
    {
       QStringRef type = attributes.value(TYPE_DEF);
       name = attributes.value(NAME_DEF).toString().toStdString();
       QString el_name = m_reader.name().toString();
       if(el_name.compare(CONNECTOR_DEF) == 0)
       {
          if(!m_reader.attributes().hasAttribute(WIDTH_DEF)||
             !m_reader.attributes().hasAttribute(NAME_DEF) ||
             !m_reader.attributes().hasAttribute(LEVEL_DEF))
             return NULL;
          int w = attributes.value(WIDTH_DEF).toString().toInt();
          int level = attributes.value(LEVEL_DEF).toString().toInt();
          vector<QPoint> m_points;
          while(true)
          {
             QXmlStreamReader::TokenType token = m_reader.readNext();
             if(token == QXmlStreamReader::StartElement )
             {
                QString str = m_reader.name().toString();
                if(str.compare(POINT_DEF) == 0)
                {
                    if(!m_reader.attributes().hasAttribute(X_DEF)||
                       !m_reader.attributes().hasAttribute(Y_DEF))
                        continue;
                    int x = m_reader.attributes().value(X_DEF).toString().toInt();
                    int y = m_reader.attributes().value(Y_DEF).toString().toInt();
                    m_points.push_back(QPoint(x,y));
                }
                cout<<"start element "<<endl;
             }
             else if (token == QXmlStreamReader::EndElement && m_reader.name().compare(CONNECTOR_DEF) == 0)
             {
                QString str = m_reader.name().toString();
                cout<<"end element "<<endl;
                cout<<"connector"<<endl;
                return ItemsFactory::createConnector(&m_points,w,(BOARD_LEVEL_ID)level);
             }
             else
             {
                QString str = m_reader.name().toString();
                cout<<"other element "<<endl;
             }
          }
       }
       else if(el_name.compare(CONTAINER_DEF) == 0)
       {
          vector<GraphicalItem*> items;
          vector<string> names;
          if(!m_reader.attributes().hasAttribute(NAME_DEF)
              || !m_reader.attributes().hasAttribute(X_DEF) 
              || !m_reader.attributes().hasAttribute(Y_DEF) 
              || !m_reader.attributes().hasAttribute(LEVEL_DEF)
            )
              return NULL;
          int xx = m_reader.attributes().value(X_DEF).toString().toInt();
          int yy = m_reader.attributes().value(Y_DEF).toString().toInt();
          BOARD_LEVEL_ID level = (BOARD_LEVEL_ID)m_reader.attributes().value(LEVEL_DEF).toString().toInt();
          while(true)
          {
             QXmlStreamReader::TokenType token = m_reader.readNext();
             if(token == QXmlStreamReader::StartElement )
             {
                QString str = m_reader.name().toString();
                if(str.compare(PLATE_DEF) == 0 && m_reader.attributes().size() > 0)
                {
                   QXmlStreamAttributes att = m_reader.attributes();
                   GraphicalItem *p = ItemsFactory::createItem(att,name);
                   if(p != NULL)
                   {
                      items.push_back(p);
                      names.push_back(name);
                   }
                }
                else if(str.compare(PACKAGE_DEF) == 0 && m_reader.attributes().size() > 0)
                {
                   GraphicalItem *p = ItemsFactory::createPackage(m_reader.attributes(),name);
                   if(p != NULL)
                   {
                      items.push_back(p);
                      names.push_back(name);
                   }                   
                }
                cout<<"start element "<<endl;
             }
             else if (token == QXmlStreamReader::EndElement && m_reader.name().compare(CONTAINER_DEF) == 0)
             {
                QString str = m_reader.name().toString();
                cout<<"end element "<<endl;
                return ItemsFactory::createContainer(xx,yy,level,items);
             }
             else
             {
                QString str = m_reader.name().toString();
                cout<<"other element "<<endl;
             }
          }          
       }   
       else if(el_name.compare(RESISTOR_DEF) == 0)
       {
          GraphicalItem *p = ItemsFactory::createResistor(attributes,name);
          t = type.toString().toStdString();
          return p;
       }
       else if(el_name.compare(EL_CAPACITOR_DEF) == 0)
       {
          GraphicalItem *p = ItemsFactory::createElCapacitor(attributes,name);
          t = type.toString().toStdString();
          return p;
       }
       else if(el_name.compare(SMD_PASSIVE_DEF) == 0)
       {
          GraphicalItem *p = ItemsFactory::createSmdType(attributes,name);
          t = type.toString().toStdString();
          return p;
       }
       else if(el_name.compare(CAPACITOR_DEF) == 0)
       {
          GraphicalItem *p = ItemsFactory::createCap(attributes,name);
          t = type.toString().toStdString();
          return p;
       }              
       else if(el_name.compare(DIP_CHIP_DEF) == 0)
       {
          t = type.toString().toStdString();
          if(stdLib)
             return ItemsFactory::createStdDip(attributes,name,iPreviewSizeX/2,iPreviewSizeY/2);
          else
             return ItemsFactory::createStdDip(attributes,name);
       }              
       else if(el_name.compare(SO_CHIP_DEF) == 0)
       {
          t = type.toString().toStdString();          
          if(stdLib)          
             return ItemsFactory::createStdSO(attributes,name,iPreviewSizeX/2,iPreviewSizeY/2);
          else
             return ItemsFactory::createStdSO(attributes,name);
          
       }                     
       else
       {
          cout<<"plate"<<endl;          
          return ItemsFactory::createItem(attributes,name);
       }
    }
    return NULL;
}
*/
void FileReader::readLibraryFile(QStandardItemModel& model,map<QString,tree_data>& map)
{
    QStandardItem *item = model.invisibleRootItem();
    QStandardItem *curItem = nullptr;
    while (!m_reader.atEnd())
    {
       QXmlStreamReader::TokenType token = m_reader.readNext();
       if (token == QXmlStreamReader::StartDocument)
          continue;
       

       if (token == QXmlStreamReader::StartElement)
       {
          if(m_reader.attributes().size() > 0)
          {
             QXmlStreamAttributes attributes = m_reader.attributes();
             QString name;
             QString type;
             if(curItem != nullptr)
             {
                
//                GraphicalItem *p = readElement(attributes,name,type,true);
                ElementInfo info;
                QString elemName;
                auto p = parseOneElem(name,type,elemName,info,token,true,nullptr);
                if(p.get() != nullptr)
                {
                   QStandardItem *pItem = new QStandardItem(name);
                   curItem->appendRow(pItem);
                   tree_data data;
                   data.setGraphicalItem(p);
                   data.assignTypeFromQString(type);
//                  for(int i = 0; i < lItemTypeLength - 1; ++i)
//                     data.type[i] = type.at(i);
//                  data.type[lItemTypeLength - 1] = 0;     
//                   
                   map[name] = data;
//                   curItem = pItem;
                   cout<<name.toStdString()<<endl;
                   checkAndAddAttrToData(elemName,map.find(name)->second,attributes);
                }
             }   
          }
          else
          {
             QStandardItem *pItem = new QStandardItem(m_reader.name().toString());
             cout<<m_reader.name().toString().toStdString()<<endl;
             if(!curItem)
                item->appendRow(pItem);
             else
                curItem->appendRow(pItem);
          
             curItem = pItem;
          }          
       }
       else if(token == QXmlStreamReader::EndElement)
       {
          curItem = curItem->parent();
          if(curItem)
             cout<<curItem->text().toStdString()<<endl;
       }
    }

}

void FileReader::checkAndAddAttrToData(QString& name,
                                       tree_data& data,QXmlStreamAttributes& attr)
{
   if(name.compare(RESISTOR_DEF) == 0 ||
      name.compare(EL_CAPACITOR_DEF) == 0 ||
      name.compare(SMD_PASSIVE_DEF) == 0 ||
      name.compare(DIP_CHIP_DEF) == 0 ||
      name.compare(SO_CHIP_DEF) == 0 ||
      name.compare(MSSOP_CHIP_DEF) == 0 ||
      name.compare(TSSOP_CHIP_DEF) == 0 ||
      name.compare(CAP_SCEMATIC_DEF) == 0 ||
      name.compare(CAPACITOR_DEF) == 0)
   {
      data.setAttributes(attr);
      data.setXmlElemName(name);
   }
}



FileReader::~FileReader()
{

}
