#ifndef TREE_DATA_H
#define TREE_DATA_H
#include "smartptr.h"
#include "graphicalitem.h"
#include <QXmlStreamAttributes>

class tree_data
{
   QChar type[lItemTypeLength];
   SmartPtr<GraphicalItem> pItem;
   SmartPtr<QXmlStreamAttributes> pAttr; //for standard containers
   QString xmlElemName;
public:
   void assignTypeFromQString(QString& str)
   {
      for(int i = 0; i < lItemTypeLength - 1 && i < str.size(); ++i)
         type[i] = str.at(i);
      type[lItemTypeLength - 1] = 0;
   }
   SmartPtr<GraphicalItem> getGraphicalItem()
   {
      return pItem;
   }
   void setGraphicalItem(SmartPtr<GraphicalItem>& p)
   {
      pItem = p;
   }
   void setAttributes(QXmlStreamAttributes& attr)
   {
      pAttr.reset(new QXmlStreamAttributes(attr));
   }
   QXmlStreamAttributes *getAttributes()
   {
      return pAttr.get();
   }
   void setXmlElemName(const QString& str)
   {
      xmlElemName = str;
   }
   QString* getXmlElementName()
   {
      return &xmlElemName;
   }
   QChar* getType()
   {
      return type;
   }
   static tree_data createContainerData(SmartPtr<GraphicalItem>& p)
   {
      tree_data data;
      data.setGraphicalItem(p);
      char temp[] = "generic";
      for(size_t i = 0 ; i < strlen(temp); ++i)
          data.type[i] = QChar(temp[i]);
      data.setXmlElemName(CONTAINER_DEF);
      return data;
   }
};



#endif // TREE_DATA_H
