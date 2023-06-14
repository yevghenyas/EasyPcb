#include "packagewrapper.h"
#include "drawwrapper.h"
#include "itemsfactory.h"

PackageWrapper::PackageWrapper()
{

}

void PackageWrapper::setTypeByMode(MODE_DRAW mode)
{
   switch(mode)
   {
      case MODE_CURSOR:
         type = PackageType::Cursor;
         break;
      case MODE_PACK_RECT:
         type = PackageType::PackageRect;
         break;
      case MODE_PACK_ROUND:
         type = PackageType::PackageRound;
         break;
      default:
         type = PackageType::Other;
         rollback();
         break;
   }
}

void PackageWrapper::paintPackage(QPainter& p,BOARD_LEVEL_ID level,
                                  int zoom)
{
   if(pt1.x() < 0 || pt2.x() < 0 || type == PackageType::Other)
      return;

   QColor c = type == PackageType::Cursor ?
              selectColor :
              LevelsWrapper::getColorForLevel(level);
   if(type == PackageType::PackageRect ||
      type == PackageType::Cursor)
   {
      DrawWrapper::drawRectPackage(p,c,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                                    castFloatToIntPxls((pt1.x() + pt2.x())/2,zoom),
                                    castFloatToIntPxls((pt1.y() + pt2.y())/2,zoom),
                                    castFloatToIntPxls(pt2.x() - pt1.x(),zoom),
                                    castFloatToIntPxls(pt2.y() - pt1.y(),zoom));

   }
   else
   {
       float radius = std::hypotf(pt2.x() - pt1.x(),pt2.y() - pt1.y());
       DrawWrapper::drawRoundPackageF(p,c,
                                     pt1.x(),
                                     pt1.y(),
                                     radius,
                                     radius,zoom,1);
   }
}

SmartPtr<GraphicalItem> PackageWrapper::commit(BOARD_LEVEL_ID level,bool filled)
{
   //create package
   if(type == PackageType::PackageRect)
   {
      return ItemsFactory::createRectPackage((pt1.x() + pt2.x())/2,
                                             (pt1.y() + pt2.y())/2,
                                             pt2.x() - pt1.x(),
                                             pt2.y() - pt1.y(),
                                             level,
                                             ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,ID_NONE,
                                             iNoZoom,iNoZoom,filled);
   }
   else if(type == PackageType::PackageRound)
   {
      float radius = std::hypotf(pt2.x() - pt1.x(),pt2.y() - pt1.y());
      return ItemsFactory::createRoundPackage(pt1.x(),
                                              pt1.y(),
                                              radius * 2,
                                              radius * 2,
                                              level,
                                              ITEMS_ORIENTATION::O_VERTICAL_TOP,
                                              ID_NONE,
                                              iNoZoom,iNoZoom);
   }
}

void PackageWrapper::setPt1(const PointF& pt) noexcept
{
   if(type == PackageType::Other)
      return;
   pt1 = pt;
}

void PackageWrapper::setPt2(const PointF& pt) noexcept
{
   if(type == PackageType::Other)
      return;
   pt2 = pt;
}


void PackageWrapper::setPt1(const QPoint& pt,int zoom)
{
   if(type == PackageType::Other)
      return;
   pt1.setX(pt.x()/PIXELS_PER_MM/zoom);
   pt1.setY(pt.y()/PIXELS_PER_MM/zoom);
}

void PackageWrapper::setPt2(const QPoint& pt,int zoom)
{
   if(type == PackageType::Other)
      return;
   pt2.setX(pt.x()/PIXELS_PER_MM/zoom);
   pt2.setY(pt.y()/PIXELS_PER_MM/zoom);
}

void PackageWrapper::clear() noexcept
{
   pt1.setX(-1);
   pt1.setY(-1);
   pt2.setX(-1);
   pt2.setY(-1);
}


void PackageWrapper::rollback() noexcept
{
   clear();
}

bool PackageWrapper::getArea(QRect& rect,int zoom)
{
   if(type == PackageType::Other ||
      type == PackageType::PackageRound)
      return false;
   if(pt1.x() < 0 || pt2.x() < 0)
      return false;
   rect.setCoords(castFloatToIntPxls(pt1.x(),zoom),
                  castFloatToIntPxls(pt1.y(),zoom),
                  castFloatToIntPxls(pt2.x(),zoom),
                  castFloatToIntPxls(pt2.y(),zoom));
   return true;
}

