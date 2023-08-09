#ifndef CONNECTORWRAPPER_H
#define CONNECTORWRAPPER_H

#include "common.h"
#include "smartptr.h"
#include "connectorgraphicalitem.h"
#include "drawwrapper.h"


class ConnectorWrapper
{
   //basic connector properties
   CONNECTOR_TYPE m_type{CONNECTOR_TYPE::NONE};
   float width{fDefVirtConWidth};
   BOARD_LEVEL_ID boardLevel{LEVEL_VC};
   //managed connector item;
   SmartPtr<ConnectorGraphicalItem> m_tempItem;
   //The last point properties
   //in case the point connected
   SmartPtr<GraphicalItem> m_tempConItem;
   //for non connected point coordinates
   PointF m_tempPoint;
   static constexpr int textSize{12};
   //private methods
   int getConnectedItemsSize();
   void paintForConnector(QPainter& p, QColor& c,int zoom_plus);
   void paintForConnectedRuler(QPainter& p, QColor& c,int zoom_plus);
   void paintForSimpleRuler(QPainter& p, QColor& c,int zoom_plus);
   void paintText(QPainter& p,QColor &c,PointF& pt1,
                  PointF& pt2,int zoom_plus);
public:
   ConnectorWrapper();
   virtual ~ConnectorWrapper(){}
   void setProperties(CONNECTOR_TYPE type,float w,BOARD_LEVEL_ID l);
   //add last point to tempItem
   void addPoint();
   //set and get data methods
   void setConnector(SmartPtr<GraphicalItem>& item){m_tempConItem = item;}
   void setPoint(PointF& pt);
   PointF& getPoint(){return m_tempPoint;}
   void clearConnector(){m_tempConItem.reset(nullptr);}
   void clearPoint();
   //returns built item and reset all temp properties
   SmartPtr<ConnectorGraphicalItem> commit();
   //cancels building of connector
   void rollback();
   //paint temposrary connector
   void paintItem(QPainter& p,int zoom_plus);
   bool isConnecting();
   CONNECTOR_TYPE getConnectorType() {return m_type;}
};

#endif // CONNECTORWRAPPER_H
