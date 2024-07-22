#ifndef PAGEDATA_H
#define PAGEDATA_H

#include "smartptr.h"
#include "graphicalitem.h"

struct PageData
{
   SmartPtr<GraphicalItem> item;
   bool bValidated{false};
   double pinDist;
   double pinPackDist;
};

#endif // PAGEDATA_H
