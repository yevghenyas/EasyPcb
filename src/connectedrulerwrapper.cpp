#include "connectedrulerwrapper.h"

ConnectedRulerWrapper::ConnectedRulerWrapper()
{

}

/*

void ConnectorWrapper::addPoint()
{
   //added node must be connected
   if(m_tempConItem.get() == nullptr)
      return;
   //in this case the connector is ready
   if(getConnectedItemsSize() > 1)
      return;

   if(m_tempItem.get() == nullptr)
   {
      //no points in the beginning
      vector<PointF> v;
      //zoom values set to 1
      m_tempItem = SmartPtr<ConnectorGraphicalItem>::make_smartptr
              <ConnectorGraphicalItem>(std::move(v),width,boardLevel,m_type,1,1,ID_NONE);
   }
   m_tempItem->addConnectedNode(m_tempConItem);
}

SmartPtr<ConnectorGraphicalItem> ConnectorWrapper::commit()
{
   return SmartPtr<ConnectorGraphicalItem>{};
}


void ConnectorWrapper::paintItem(QPainter& p, int zoom_plus)
{
   //return immediately in case we are not in active mode
   if(m_type == CONNECTOR_TYPE::NONE)
      return;

   QColor c = rulerColor;

   PointF pt1 = m_tempItem->getPoints()->at(0);
   PointF pt2 = getConnectedItemsSize() > 1 ? m_tempItem->getPoints()->at(0)
                                            :m_tempPoint;

   DrawWrapper::drawLine(p,c,pt1.x(),pt1.y(),pt2.x(),pt2.y(),
                      false,width,LINE_STYLE::LINE_SQUARED,zoom_plus,1);



   if(getConnectedItemsSize() > 1)
   {

   }

   if(m_tempItem.get() != nullptr)
   {
      m_tempItem->setZoom(zoom_plus);
      m_tempItem->paintItem(p,c,boardLevel);
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
   if(m_type == CONNECTOR_TYPE::RULER)
   {
      int size = getConnectedItemsSize();
      if(size > 0)
      {
         PointF pt1 = m_tempItem->getPoints()->at(0);
         PointF pt2 = size > 1 ? m_tempItem->getPoints()->at(1)
                               : m_tempPoint;
         float difX = abs(pt1.x() - pt2.x()) * PIXELS_PER_MM * zoom_plus;
         float difY = abs(pt1.y() - pt2.y()) * PIXELS_PER_MM * zoom_plus;
         float l = hypotf(difX,difY);
         char buf[64];
         sprintf(buf,"x=%.3,y=%.3f,l=%.3f",
                 static_cast<double>(difX),
                 static_cast<double>(difY),
                 static_cast<double>(l));
         QString text = buf;
         float x = (pt1.x() + pt2.x())/2;
         float y = (pt1.y() + pt2.y())/2;
         DrawWrapper::drawText(p,c,text,14,-1,-1,x,y,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,zoom_plus,1);
      }
   }

}

bool ConnectorWrapper::isConnecting()
{
   switch(m_type)
   {
      case CONNECTOR_TYPE::RULER:
      case CONNECTOR_TYPE::SCHEMATIC:
         return true;
      default:
         return false;
   }
}

*/
