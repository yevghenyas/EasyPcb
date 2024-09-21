#include "previewlabel.h"
#include "genericgraphicalitemscontainer.h"


void PreviewLabel::previewItem(SmartPtr<GraphicalItem>& item,const QString& name)
{
   m_itemInPreview = item.get();
   m_sItemName = name;
   refresh();
}

void PreviewLabel::paintItem()
{
   GenericGraphicalItemsContainer *pG = nullptr;
   auto zoom = 1;
   zoom_d = 1;
   //all containers should be with (0,0) coordinates after creation
   if((pG = dynamic_cast<GenericGraphicalItemsContainer*>(m_itemInPreview)))
   {
      auto maxY = static_cast<int>(pG->getMaxY() * PIXELS_PER_MM);
      auto maxX = static_cast<int>(pG->getMaxX() * PIXELS_PER_MM);
      auto maxYPr = sizeHintY/2;
      auto maxXPr = sizeHintX/2;
      auto zoomX = maxX > maxXPr ? maxX/maxXPr * 2 : 1;
      auto zoomY = maxY > maxYPr ? maxY/maxYPr * 2 : 1;
      zoom_d = std::max(zoomX,zoomY);
      //set them temporarily for the center of label
      pG->setX(static_cast<float>(sizeHintX) /PIXELS_PER_MM/2 * zoom_d / zoom);
      pG->setY(static_cast<float>(sizeHintY) /PIXELS_PER_MM/2 * zoom_d / zoom);
   }
   //paint
   QPixmap *p = const_cast<QPixmap*>(pixmap());
   QColor c(Qt::black);
   QPainter pnt(p);
   //paint it
   m_itemInPreview->paintItemIntoBuffer(0,0,pnt,c,zoom,zoom_d);
   repaint();
   //set them back to (0.0)
   if(pG != nullptr)
   {
      pG->setX(0);
      pG->setY(0);
   }
}

void PreviewLabel::refresh()
{
   //paint
   QPixmap *p = const_cast<QPixmap*>(pixmap());
   p->fill(Qt::transparent);
   paintItem();
}


void PreviewLabel::clearPreview()
{
   m_itemInPreview = nullptr;
   m_sItemName.clear();
//   clear();
}

bool PreviewLabel::isDragOfElement(const QPoint& pos)
{
   if(m_itemInPreview)
   {
      GenericGraphicalItemsContainer *pG = nullptr;

      int zoom = 1;
      //all containers should be with (0,0) coordinates after creation
      //set real coordinates of parent(center of preview panel
      if((pG = dynamic_cast<GenericGraphicalItemsContainer*>(m_itemInPreview)))
      {
         //set them temporarily for the center of label
         pG->setX(static_cast<float>(sizeHintX)/2/PIXELS_PER_MM);
         pG->setY(static_cast<float>(sizeHintY)/2/PIXELS_PER_MM);
      }

      if(m_itemInPreview->isInArea(pos.x(),pos.y()*zoom_d/zoom))
         return true;

      if(pG != nullptr)
      {
         pG->setX(0);
         pG->setY(0);
      }
   }
   return false;
}
