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

#ifndef FILEREADER_H
#define FILEREADER_H

#include <QXmlStreamReader>
#include <list>
#include "graphicalitem.h"
#include <QXmlStreamAttributes>
#include <QStandardItem>
#include "componentstreemodel.h"
#include <vector>
#include "connectedrulerwrapper.h"
#include "tree_data.h"




using namespace std;

class ElementInfo
{
public:
   enum RESULT_EL {RESULT_NONE,RESULT_ITEM,RESULT_POINT};   
private:   
   QPoint m_point;
   SmartPtr<GraphicalItem> m_pGitem;
   RESULT_EL m_result;
public:
   ElementInfo():m_point(0,0),m_result(RESULT_NONE){}
   void setGraphicalItem(SmartPtr<GraphicalItem>& p)
   {
      m_pGitem = p;
      m_result = RESULT_ITEM;
   }
   void setPoint(QPoint& pt)
   {
      m_point = pt;
      m_result = RESULT_POINT;
   }
   RESULT_EL getResult(){return m_result;}
   void getPoint(QPoint& pt)
   {
      pt = m_point; 
   }
   GraphicalItem *getItem()
   {
      return m_pGitem.get();
   }
};




class FileReader
{
   QXmlStreamReader m_reader;   
public:
    FileReader();
    void setDevice(QIODevice *pDevice);
    SchemData parseItems(vector<SmartPtr<GraphicalItem> >& items,vector<QString>& names);
    SmartPtr<GraphicalItem> parseOneElem(QString& name,QString& type,QString& elemName,
                                         ElementInfo& info,
                                         QXmlStreamReader::TokenType& t,bool stdLib,map<ITEM_ID,SmartPtr<GraphicalItem>> *idToPtr);
//    GraphicalItem* readElement(QXmlStreamAttributes& attributes, string& name,string& type,bool sttLib = false) ;
    void readLibraryFile(QStandardItemModel& model,map<QString,tree_data>& map);
    void findConnectedItem(map<ITEM_ID,SmartPtr<GraphicalItem>>* idToPtr,
                           map<ITEM_ID,int>* ids_connected,
                           ConnectorGraphicalItem *pConnector);
    void checkAndAddAttrToData(QString& name,
                               tree_data& data,
                               QXmlStreamAttributes& attr);
    
    ~FileReader();
};

#endif // FILEREADER_H
