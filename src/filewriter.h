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


#ifndef FILEWRITER_H
#define FILEWRITER_H

#include <QXmlStreamWriter>
#include "graphicalitem.h"
#include <map>
#include <set>
#include <vector>
#include <QStandardItem>
#include <QStandardItemModel>
#include "tree_data.h"





using TreeData = map <QString,tree_data>;

class FileWriter
{
    QXmlStreamWriter m_xmlWriter;
    //to avoid processing containers that reside on several layers
    set<QString> m_containers;
public:
    FileWriter();
    void setDevice(QIODevice *pDevice); 
    void writeItems(map<QString,SmartPtr<GraphicalItem> > &items);
    void startDocument(int iBoardWidth,int iBoardHeight);
    void endDocument();
    void parseAndWriteItem(const QString& name,GraphicalItem *p);
    //path is serie of elements for example
    //<custom><ssop><ss16>.... path[0] = "custom" path[1] = "ssop" path[2] = "ss16"
    void writeToLibrary(vector<QString>& path,const QString& name,GraphicalItem *p);
    bool writeToLibrary1(vector<QString>& path,const QString& name,GraphicalItem *p);    
    void writeModelToLibrary(QStandardItemModel *modelint,TreeData& treeData);
    void writeItemToLibrary(QStandardItem *item,TreeData& treeData);
    FileWriter(const FileWriter& other);
    ~FileWriter();
};

#endif // FILEWRITER_H
