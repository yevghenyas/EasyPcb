#ifndef PACKAGEWRAPPER_H
#define PACKAGEWRAPPER_H

#include "common.h"
#include <QPainter>
#include "smartptr.h"

enum class PackageType {PackageRound,PackageRect,Cursor,Other};

class PackageWrapper
{
   PointF pt1{-1,-1};
   PointF pt2{-1,-1};
   PackageType type{PackageType::Cursor};
public:
    PackageWrapper();
    void paintPackage(QPainter& p,BOARD_LEVEL_ID level,int zoom);
    SmartPtr<GraphicalItem> commit(BOARD_LEVEL_ID level,bool filled);
    void rollback() noexcept;
    void setType(PackageType t){type = t;}
    void setTypeByMode(MODE_DRAW mode);
    void setPt1(const QPoint& pt,int zoom);
    void setPt1(const PointF& pt) noexcept;
    void setPt2(const PointF& pt) noexcept;
    void setPt2(const QPoint& pt,int zoom);
    bool getArea(QRect& rect,int zoom);
    void clear() noexcept;
};

#endif // PACKAGEWRAPPER_H
