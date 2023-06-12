#include "connectorwrapper.h"
#include "itemsfactory.h"

ConnectorWrapper::ConnectorWrapper()
{
}
void ConnectorWrapper::setProperties(CONNECTOR_TYPE type,float w,BOARD_LEVEL_ID l)
{
   m_type = type;
   width = w;
   boardLevel = l;
}

void ConnectorWrapper::addPoint()
{
   if(m_type == CONNECTOR_TYPE::CONNECTED_RULER
      || m_type == CONNECTOR_TYPE::SCHEMATIC )
   {
      //first added node must be connected for these types
      if(m_tempConItem.get() == nullptr
              && getConnectedItemsSize() == 0)
         return;
      //in this case the connector is ready
      if(getConnectedItemsSize() > 1)
         return;
   }
   if(m_tempItem.get() == nullptr)
   {
      //no points in the beginning
      vector<PointF> v;
      //zoom values set to 1
      m_tempItem = SmartPtr<ConnectorGraphicalItem>::make_smartptr
              <ConnectorGraphicalItem>(std::move(v),width,boardLevel,m_type,ID_NONE);
   }
   if(m_tempConItem.get() != nullptr)
      m_tempItem->addConnectedNode(m_tempConItem);
   else
      m_tempItem->addNode(m_tempPoint);

}

void ConnectorWrapper::setPoint(PointF& pt)
{
   m_tempPoint = pt;
}

void ConnectorWrapper::clearPoint()
{
   m_tempPoint.setX(-1);
   m_tempPoint.setY(-1);
}

SmartPtr<ConnectorGraphicalItem> ConnectorWrapper::commit()
{
   //reset all data and return built connector
   SmartPtr<ConnectorGraphicalItem> conItem;
   if(m_type == CONNECTOR_TYPE::SCHEMATIC)
   {
      if(getConnectedItemsSize() > 1)
         conItem = m_tempItem;      
   }
   else
   {
      if(m_tempItem.get() != nullptr &&
           m_tempItem->getPoints()->size() > 1)
         conItem = m_tempItem;
   }
   rollback();
   return conItem;
}

void ConnectorWrapper::rollback()
{
   //reset all data
   m_tempItem.reset(nullptr);
   m_tempConItem.reset(nullptr);
   m_tempPoint.setX(-1);
   m_tempPoint.setY(-1);
   m_type = CONNECTOR_TYPE::NONE;
}

void ConnectorWrapper::paintItem(QPainter& p, int zoom_plus)
{
   //return immediately in case we are not in active mode
   QColor c = LevelsWrapper::getColorForLevel(boardLevel);
   switch(m_type)
   {
      case CONNECTOR_TYPE::NONE:
         return;
      case CONNECTOR_TYPE::SCHEMATIC:
         c = virtConColor;
         paintForConnector(p,c,zoom_plus);
         break;
      case CONNECTOR_TYPE::CONNECTED_RULER:
         c = rulerColor;
         paintForConnectedRuler(p,c,zoom_plus);
         break;
      case CONNECTOR_TYPE::SIMPLE_RULER:
         c = rulerColor;
         paintForSimpleRuler(p,c,zoom_plus);
         break;
      default:
         paintForConnector(p,c,zoom_plus);
         break;
   }

}

void ConnectorWrapper::paintForConnector(QPainter& p,QColor& c,int zoom_plus)
{
   //return immediately in case we are not in active mode
   if(m_tempItem.get() != nullptr)
   {
      m_tempItem->paintItem(p,c,zoom_plus,1,boardLevel);
      if(m_tempPoint.x() > 0 && m_tempPoint.y() > 0)
      {
         //draw line here
         PointF pt1 = m_tempItem->getPoints()->at(m_tempItem->getPoints()->size() - 1);
         PointF pt2 = m_tempConItem.get() != nullptr
                 ? PointF(m_tempConItem->abs_x(),m_tempConItem->abs_y()) : m_tempPoint;
         DrawWrapper::drawLine(p,c,pt1.x(),pt1.y(),pt2.x(),pt2.y(),
                            false,width,LINE_STYLE::LINE_SQUARED,zoom_plus,1);
      }
   }
   if(m_tempConItem.get() != nullptr)
   {
      //draw connected point here
      DrawWrapper::drawConnectorPlate(p,c,m_tempConItem->abs_x(),m_tempConItem->abs_y(),zoom_plus);
   }
}

void ConnectorWrapper::paintForConnectedRuler(QPainter& p,QColor& c,int zoom_plus)
{
   PointF pt1(-1,-1);
   PointF pt2(-1,-1);
   int size = getConnectedItemsSize();
   if (size > 0)
   {
      m_tempItem->paintItem(p,c,zoom_plus,1,boardLevel);
   }
   if(size > 1)
   {
      pt1 = m_tempItem->getPoints()->at(0);
      pt2 = m_tempItem->getPoints()->at(1);
   }
   else if(size == 1)
   {
      pt1 = m_tempItem->getPoints()->at(0);
      pt2 = m_tempPoint;
      DrawWrapper::drawLine(p,c,pt1.x(),pt1.y(),pt2.x(),pt2.y(),
                         false,width,LINE_STYLE::LINE_SQUARED,zoom_plus,1);
   }
   if(m_tempConItem.get() != nullptr)
   {
      //draw connected point here
      DrawWrapper::drawConnectorPlate(p,c,m_tempConItem->abs_x(),m_tempConItem->abs_y(),zoom_plus);
   }

   if(size > 0)
   {
      paintText(p,c,pt1,pt2);
   }
}


int ConnectorWrapper::getConnectedItemsSize()
{
  if(m_tempItem.get() != nullptr &&
     m_tempItem->getConnectedItems() != nullptr)
     return static_cast<int> (m_tempItem->getConnectedItems()->size());
  else
     return 0;

}

bool ConnectorWrapper::isConnecting()
{
   switch(m_type)
   {
      case CONNECTOR_TYPE::CONNECTED_RULER:
         if(getConnectedItemsSize() > 1)
            return false;
      case CONNECTOR_TYPE::SCHEMATIC:
         return true;
      case CONNECTOR_TYPE::SIMPLE_RULER:
         return (m_tempItem.get() == nullptr);
      default:
         return false;
   }
}

void ConnectorWrapper::paintText(QPainter& p,QColor &c,PointF &pt1,PointF& pt2)
{
   float difX = abs(pt1.x() - pt2.x());
   float difY = abs(pt1.y() - pt2.y());
   float l = hypotf(difX,difY);
   char buf[64];
   sprintf(buf,"x=%.3f,y=%.3f,l=%.3f",
            static_cast<double>(difX),
            static_cast<double>(difY),
            static_cast<double>(l));
   QString text = buf;
   float x = (pt1.x() + pt2.x())/2;
   float y = (pt1.y() + pt2.y())/2;
   DrawWrapper::drawText(p,c,text,textSize,-1,-1,x,y,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,1,1);
}

void ConnectorWrapper::paintForSimpleRuler(QPainter& p, QColor& c,int zoom_plus)
{
   if(m_tempItem.get() == nullptr)
      return;
   paintForConnector(p,c,zoom_plus);
   PointF pt1 = m_tempItem->getPoints()->at(0);
   PointF pt2 = m_tempPoint;
   paintText(p,c,pt1,pt2);
}
