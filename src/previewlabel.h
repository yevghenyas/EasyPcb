#ifndef PREVIEWLABEL_H
#define PREVIEWLABEL_H


#include <QLabel>
#include "graphicalitem.h"

class PreviewLabel : public QLabel
{
   GraphicalItem *m_itemInPreview;
   QString m_sItemName;
   int zoom_d{1};
   int sizeHintX{iPreviewSizeX};
   int sizeHintY{iPreviewSizeY};
public:
   PreviewLabel():m_itemInPreview(nullptr){}
   virtual QSize sizeHint() const
   {
      return QSize(sizeHintX,sizeHintY);
   }
   virtual QSize minimumSize() const
   {
      return QSize(sizeHintX,sizeHintY);
   }
   //repaints item in case it was changed
   void refresh();
   //paints item into pixmap
   void paintItem();
   void previewItem(SmartPtr<GraphicalItem>& item,const QString& name);
   void clearPreview();
   void setSizeHintX(int x){sizeHintX = x;}
   void setSizeHintY(int y){sizeHintY = y;}
   bool isDragOfElement(const QPoint& pos);
   GraphicalItem* getPreviewItem(){return m_itemInPreview;}
   QString getPreviewItemName(){return m_sItemName;}
   QPixmap* getPixMap(QPoint& ,int )
   {
      if(m_itemInPreview)
      {
         QPixmap *pix = new QPixmap(*pixmap());
         return pix;
      }
      return nullptr;
   }
   void setDefaultPixmap()
   {
      const QPixmap *p = pixmap();
      if(!p)
      {
         QSize s = size();
         if(s.width() != iPreviewSizeX)
         {
             s.setWidth(sizeHintX);
             s.setHeight(sizeHintY);
         }
         QPixmap buffer(s);
         setPixmap(buffer);
      }
      const_cast<QPixmap*>(pixmap())->fill(Qt::transparent);
   }
};



#endif // PREVIEWLABEL_H
