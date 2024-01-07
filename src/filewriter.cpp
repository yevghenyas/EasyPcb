/*
 * Copyright 2018 <copyright holder> <email>
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
#include "common.h"
#include "filewriter.h"
#include "graphicalitem.h"
#include "graphicalitemscontainer.h"
#include "genericgraphicalitemscontainer.h"
#include <vector>
#include "connectorgraphicalitem.h"
#include "roundpackagegraphicalitem.h"
#include "rectpackagegraphicalitem.h"
#include "roundplategraphicalitem.h"
#include "packagegraphicalitem.h"
#include "rectgraphicalitem.h"
#include "capgraphicalitem.h"
#include "textgraphicalitem.h"
#include <QFile>
#include <iostream>

FileWriter::FileWriter()
{

}

void FileWriter::setDevice(QIODevice* pDevice)
{
   m_xmlWriter.setDevice(pDevice);
}


FileWriter::FileWriter(const FileWriter& other)
{

}

void FileWriter::startDocument(int iBoardWidth,int iBoardHeight)
{
   m_xmlWriter.setAutoFormatting(true);  // Устанавливаем автоформатирование текста
   m_xmlWriter.writeStartDocument();     // Запускаем запись в документ
   m_xmlWriter.writeStartElement(BD_STRT_EL);
   //write board width and height
   m_xmlWriter.writeAttribute(WIDTH_DEF,QString::number(iBoardWidth));
   m_xmlWriter.writeAttribute(HEIGHT_DEF,QString::number(iBoardHeight));
   m_xmlWriter.writeStartElement(ITEMS_DEF);
}


void FileWriter::writeItems(std::map<QString,SmartPtr<GraphicalItem> >& items)
{
   int i = 0;
   for(auto item:items)
   {
      cout<<"writing:"<<item.first.toStdString()<<" "<<std::to_string(i)<<endl;
      parseAndWriteItem(item.first,item.second.get());
   }
}

void FileWriter::parseAndWriteItem(const QString& name,GraphicalItem* p)
{
   GenericGraphicalItemsContainer *pC;
   if((pC = dynamic_cast<GenericGraphicalItemsContainer*> (p)))
   {
      //check if we already processed this container for another layer
      if(m_containers.find(name) == m_containers.end())
         m_containers.insert(name);
      else
         return;
      m_xmlWriter.writeStartElement(CONTAINER_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,GENERIC_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pC->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pC->y()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pC->getLevel()));
      if(pC->isParent())
         m_xmlWriter.writeAttribute(PARENT_DEF,"y");
      std::vector<SmartPtr<GraphicalItem> > *vecChildren = pC->getChildren();
      for(auto& child :*vecChildren)
      {
         QString name = QString::number(child->getID());
         parseAndWriteItem(name,child.get());
      }
      m_xmlWriter.writeEndElement();
      return;
   }
   ConnectorGraphicalItem* pL;
   if((pL = dynamic_cast<ConnectorGraphicalItem*> (p)))
   {
      m_xmlWriter.writeStartElement(CONNECTOR_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,CONNECTOR_TYPE_DEF);
      m_xmlWriter.writeAttribute(WIDTH_DEF,QString::number(pL->width()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pL->getLevel()));
      m_xmlWriter.writeAttribute(STYLE_DEF,QString::number(static_cast<int>(pL->getStyle())));
      m_xmlWriter.writeAttribute(EL_OR_BOARD_DEF,QString::number(static_cast<int>(pL->getConnectorType())));
      vector<PointF> *pPts = pL->getPoints();
      for(auto point:*pPts)
      {
         m_xmlWriter.writeStartElement(POINT_DEF);
         m_xmlWriter.writeAttribute(X_DEF,QString::number(point.x()));
         m_xmlWriter.writeAttribute(Y_DEF,QString::number(point.y()));
         m_xmlWriter.writeEndElement();
      }
      auto connItems = pL->getConnectedItems();
      if(connItems != nullptr)
      {
         for(auto& item:*connItems)
         {
            m_xmlWriter.writeStartElement(PLATE_CNT);
            m_xmlWriter.writeAttribute(NAME_DEF,QString::number(item.second->getID()));
            m_xmlWriter.writeAttribute("Ind",QString::number(item.first));
//         m_xmlWriter.writeAttribute(X_DEF,std::to_string(item.second->x()).c_str());
//         m_xmlWriter.writeAttribute(Y_DEF,std::to_string(item.second->y()).c_str());
            m_xmlWriter.writeEndElement();
         }
      }
      m_xmlWriter.writeEndElement();
      return;
   }
   RoundPlateGraphicalItem* pRp;
   if((pRp = dynamic_cast<RoundPlateGraphicalItem*> (p)) )
   {
      auto xx = QString::number(pRp->x());
      m_xmlWriter.writeStartElement(PLATE_RND_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,PLATE_ROUNd_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pRp->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pRp->y()));
      m_xmlWriter.writeAttribute(D_EX_DEF,QString::number(pRp->d()));
      m_xmlWriter.writeAttribute(D_IN_DEF,QString::number(pRp->d1()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pRp->getLevel()));
      m_xmlWriter.writeEndElement();
      return;
   }
   RectPackageGraphicalItem* pRpkg;
   if((pRpkg = dynamic_cast<RectPackageGraphicalItem*> (p)))
   {
      m_xmlWriter.writeStartElement(PACKAGE_RECT_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,RECT_PKG_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pRpkg->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pRpkg->y()));
      m_xmlWriter.writeAttribute(WIDTH_DEF,QString::number(pRpkg->width()));
      m_xmlWriter.writeAttribute(HEIGHT_DEF,QString::number(pRpkg->height()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pRpkg->getLevel()));
      m_xmlWriter.writeAttribute(ORIENTATION_SHORT_DEF,QString::number(pRpkg->getType()));
      m_xmlWriter.writeAttribute(FILLED_SHORT_DEF,QString::number(pRpkg->isFilled() ? 1 : 0));
      m_xmlWriter.writeEndElement();
      return;
   }
   PackageGraphicalItem* pPkg;
   if((pPkg = dynamic_cast<PackageGraphicalItem*> (p)))
   {
      m_xmlWriter.writeStartElement(PACKAGE_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,PKG_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pPkg->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pPkg->y()));
      m_xmlWriter.writeAttribute(WIDTH_DEF,QString::number(pPkg->width()));
      m_xmlWriter.writeAttribute(HEIGHT_DEF,QString::number(pPkg->height()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pPkg->getLevel()));
      m_xmlWriter.writeAttribute(ORIENTATION_SHORT_DEF,QString::number(pPkg->getType()));
      m_xmlWriter.writeEndElement();
      return;
   }
   RoundPackageGraphicalItem* pRndPkg;
   if((pRndPkg = dynamic_cast<RoundPackageGraphicalItem*> (p)))
   {
      m_xmlWriter.writeStartElement(PACKAGE_RND_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,RND_PKG_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pRndPkg->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pRndPkg->y()));
      m_xmlWriter.writeAttribute(D_EX_DEF,QString::number(pRndPkg->d()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pRndPkg->getLevel()));
      m_xmlWriter.writeAttribute(ORIENTATION_SHORT_DEF,QString::number(pRndPkg->getType()));
      m_xmlWriter.writeAttribute(ANGLE_ST_DEF,QString::number(pRndPkg->getStartAngle()));
      m_xmlWriter.writeAttribute(ANGLE_SP_DEF,QString::number(pRndPkg->getSpanAngle()));
      m_xmlWriter.writeAttribute(CHORD,QString::number(pRndPkg->isClosed()));
      m_xmlWriter.writeEndElement();
      return;
   }
   RectGraphicalItem* pRect;
   if((pRect = dynamic_cast<RectGraphicalItem*> (p)))
   {
      m_xmlWriter.writeStartElement(PLATE_RECT_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,RECT_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pRect->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pRect->y()));
      m_xmlWriter.writeAttribute(WIDTH_DEF,QString::number(pRect->width()));
      m_xmlWriter.writeAttribute(HEIGHT_DEF,QString::number(pRect->height()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pRect->getLevel()));
      m_xmlWriter.writeAttribute(ORIENTATION_SHORT_DEF,QString::number(pRect->getType()));
      m_xmlWriter.writeEndElement();
      return;
   }
   CapGraphicalItem* pCap;
   if((pCap = dynamic_cast<CapGraphicalItem*> (p)))
   {
      m_xmlWriter.writeStartElement(CAP_SCEMATIC_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,CAP_SCHEM_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pCap->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pCap->y()));
      m_xmlWriter.writeAttribute(WIDTH_DEF,QString::number(pCap->f()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pCap->getLevel()));
      m_xmlWriter.writeAttribute(ORIENTATION_SHORT_DEF,QString::number(pCap->getType()));
      m_xmlWriter.writeEndElement();
      return;
   }
   if(TextGraphicalItem *pText = dynamic_cast<TextGraphicalItem*> (p))
   {

      m_xmlWriter.writeStartElement(TEXT_DEF);
      m_xmlWriter.writeAttribute(NAME_DEF,name);
      m_xmlWriter.writeAttribute(TYPE_DEF,TEXT_TYPE_DEF);
      m_xmlWriter.writeAttribute(X_DEF,QString::number(pText->x()));
      m_xmlWriter.writeAttribute(Y_DEF,QString::number(pText->y()));
      m_xmlWriter.writeAttribute(LEVEL_DEF,QString::number(pText->getLevel()));
      m_xmlWriter.writeAttribute(TXT_CONT_DEF,pText->getText());
      m_xmlWriter.writeAttribute(TXT_FNT_SZ_DEF,QString::number(pText->getFontSize()));
      m_xmlWriter.writeAttribute(ORIENTATION_SHORT_DEF,QString::number(pText->getType()));
      m_xmlWriter.writeEndElement();
      return;
   }

}

void FileWriter::endDocument()
{
   m_xmlWriter.writeEndElement();
   m_xmlWriter.writeEndElement();
   m_xmlWriter.writeEndDocument();
   m_containers.clear();
}

// adds component to library hierarchy according to the specified path.
// The path is a vector, for example path = {"BD_LIBRARY","CUSTOM","SSOPS","SSOP_CHIP"}
// in this case in the library will be
//</BD_LIBRARY>
// ............
//    <CUSTOM>
//        <DIP>
//            <SSOPS>
//                <SSOP_CHIP name="..." x="..." ......   />
//            </SSOPS>
//        </DIP>
//    </CUSTOM>
//</BD_LIBRARY>
void FileWriter::writeToLibrary(vector< QString >& path, const QString& name, GraphicalItem* p)
{
   QXmlStreamReader reader;
   //for comparing path in the file and our path
   short counter = 0;
   //flag for update operation
   bool bAdded = false;
   QFile file("bd_lib_std.bd_xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

   reader.setDevice(&file);

   QFile file_out("bd_lib_std_copy.bd_xml");
   if (!file_out.open(QIODevice::WriteOnly))
      return;

   m_xmlWriter.setDevice(&file_out);
   m_xmlWriter.setAutoFormatting(true);

   while (!reader.atEnd())
   {
      reader.readNext();
      QString str = reader.name().toString();

      if (reader.isStartDocument())
         m_xmlWriter.writeStartDocument();

      if(reader.isStartElement())
      {
         //write start element
         m_xmlWriter.writeStartElement(reader.name().toString());
         //compare the element with a part of our path
         //if they are equal increment counter
         if(path[counter].compare(str) == 0)
         {
            ++counter;
         }
         //this means that the our path and current path in the file are equal
         //and we can insert our element
         if(counter == path.size() && !bAdded)
         {
            parseAndWriteItem(name,p);
            counter = 0;
            bAdded = true;
         }
      }
      //write attributes comments and other characters
      if(reader.attributes().size() > 0)
         m_xmlWriter.writeAttributes(reader.attributes());

      if(reader.isCharacters())
         m_xmlWriter.writeCharacters(reader.text().toString());

      if(reader.isComment())
         m_xmlWriter.writeComment(reader.text().toString());

      if(reader.isEndElement())
      {
         //the current path in the file is equal to ours only partially
         //we need to extend it according to our path and add our element
         if(counter && path[counter - 1].compare(str) == 0)
         {
            //
            int counter_tmp = counter; // we need to remember starting point
            for(int i = counter; i < path.size();++i)
            {
               m_xmlWriter.writeStartElement(path[i]);
               if(i == path.size() - 1)
               {
                  parseAndWriteItem(name,p);
//                  m_xmlWriter.writeStartElement("TEST");
//                  m_xmlWriter.writeEndElement();
                  bAdded = true;
               }
            }
            for(int i = counter; i > counter_tmp;--i)
            {
               m_xmlWriter.writeEndElement();
            }
            counter--;
         }
         m_xmlWriter.writeEndElement();
      }


      if(reader.isEndDocument())
         m_xmlWriter.writeEndDocument();
   }
   m_containers.clear();   
}


bool FileWriter::writeToLibrary1(vector<QString>& path, const QString& name, GraphicalItem* p)
{
   QXmlStreamReader reader;
   //for comparing path in the file and our path
   short counter = 0;
   //flag for update operation
   bool bAdded = false;
   bool bInElementWithTheSameName = false;
   QString strUpdatedElement;
   QFile file("bd_lib_std.bd_xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

   reader.setDevice(&file);

   QFile file_out("bd_lib_std_copy.bd_xml");
   if (!file_out.open(QIODevice::WriteOnly))
      return false;

   m_xmlWriter.setDevice(&file_out);
   m_xmlWriter.setAutoFormatting(true);

   while (!reader.atEnd())
   {
      reader.readNext();
      QString str = reader.name().toString();

      if (reader.isStartDocument())
         m_xmlWriter.writeStartDocument();
      if(!bInElementWithTheSameName)
      {
         if(reader.isStartElement())
         {
            if(reader.attributes().size() > 0 &&
               reader.attributes().hasAttribute(NAME_DEF) &&
               (name.compare(reader.attributes().value(NAME_DEF)) == 0))
            {
               bInElementWithTheSameName = true;
               strUpdatedElement = str;
               continue;
            }
            m_xmlWriter.writeStartElement(reader.name().toString());
            //compare the element with a part of our path
            //if they are equal increment counter
            if(path[counter].compare(str) == 0 && !bAdded)
            {
               //increment when the path is not completely equal
               //and continue reading
               ++counter;
               //the path is equal add element in case it is not added
               if(counter == path.size())
               {
                  cout<<"adding item"<<endl;
                  parseAndWriteItem(name,p);
                  bAdded = true;
                  counter--;
               }
            }
         }
         //write attributes comments and other characters
         if(reader.attributes().size() > 0)
            m_xmlWriter.writeAttributes(reader.attributes());

         if(reader.isCharacters())
            m_xmlWriter.writeCharacters(reader.text().toString());

         if(reader.isComment())
            m_xmlWriter.writeComment(reader.text().toString());

      }
      if(reader.isEndElement())
      {
         if(bInElementWithTheSameName)
         {
            if(str.compare(strUpdatedElement) == 0)
               bInElementWithTheSameName = false;
         }
         else
         {
            if(bAdded)
            {
               m_xmlWriter.writeEndElement();
               if(counter && path[counter].compare(str) == 0)
               {
                  counter--;
               }
               continue;
            }
            //the current path in the file is equal to ours only partially
            //we need to extend it according to our path and add our element
            if(counter && path[counter - 1].compare(str) == 0)
            {
               //
               int counter_tmp = counter; // we need to remember starting point
               for(; counter < path.size();++counter)
               {
                  m_xmlWriter.writeStartElement(path[counter]);
                  if(counter == path.size() - 1)
                  {
                     cout<<"adding item with path"<<endl;
                     parseAndWriteItem(name,p);
                     bAdded = true;
                  }
               }
               for(counter = path.size() - 1 ;counter >= counter_tmp;--counter)
               {
                  m_xmlWriter.writeEndElement();
               }
            }
            m_xmlWriter.writeEndElement();
         }
      }
      if(reader.isEndDocument())
         m_xmlWriter.writeEndDocument();
   }
   m_containers.clear();
   //copy previous file and set new
   std::time_t t;
   struct tm *timeinfo;
   std::time(&t);
   timeinfo = std::localtime(&t);
   char buf[64];
   memset(buf,0,64);
   sprintf(buf,"bd_lib_std_%d%d%d%d%d%d.bd_xml",1900 + timeinfo->tm_year,timeinfo->tm_mon,timeinfo->tm_mday,
                              timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec);
   bool b1 = file.rename(buf);
   bool b2 = file_out.rename("bd_lib_std.bd_xml");
   
   return b1 && b2;
   
}

void FileWriter::writeItemToLibrary(QStandardItem *item,TreeData& treeData)
{
   auto data = treeData.find(item->text());
   if(data != treeData.end())
   {
      if(data->second.getAttributes() == nullptr)
      {
         parseAndWriteItem(data->first,data->second.getGraphicalItem().get());
      }
      else
      {
         m_xmlWriter.writeStartElement(*data->second.getXmlElementName());
         m_xmlWriter.writeAttributes(*data->second.getAttributes());
         m_xmlWriter.writeEndElement();
      }
   }
   else
   {
      m_xmlWriter.writeStartElement(item->text());
      for (int row = 0; row < item->rowCount(); row++ )
      {
         for (int col = 0; col < item->columnCount(); col++)
         {
            cout << item->child(row,col)->text().toStdString()<<endl;
            writeItemToLibrary(item->child(row,col),treeData);
         }
      }
      m_xmlWriter.writeEndElement();
   }
}

void FileWriter::writeModelToLibrary(QStandardItemModel *model,TreeData& treeData)
{
    QFile file("bd_lib_std.bd_xml");
     if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
         return;

    m_xmlWriter.setDevice(&file);
    m_xmlWriter.setAutoFormatting(true);
    m_xmlWriter.writeStartDocument();
    for (int row = 0; row < model->rowCount(); row++ ) {
      for (int col = 0; col < model->columnCount(); col++) {
        cout << model->item(row,col)->text().toStdString()<<endl;
        writeItemToLibrary(model->item(row,col),treeData);
      }
    }
    m_xmlWriter.writeEndDocument();
}

FileWriter::~FileWriter()
{

}
