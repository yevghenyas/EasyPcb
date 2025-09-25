#ifndef LEECONSTRPATHSTRATEGY_H
#define LEECONSTRPATHSTRATEGY_H

#include "common.h"




class LeeConstrPathStrategy
{
   LEE_STRATEGY strategy{LEE_STRATEGY::SHORTEST_PATH};
   int cellNumber{15};
   vector<vector<float>> *pDensity{nullptr};
   vector<UniCoord> constructLeePathShortest(int ax,int ay,int bx,int by,
                                             int x_l,int x_h,
                                             int y_l,int y_h,
                                             vector<vector<ITEM_ID>>& grid);
   vector<UniCoord> constructLeePathLowDensity(int ax,int ay,int bx,int by,
                                               int x_l,int x_h,
                                               int y_l,int y_h,
                                               vector<vector<ITEM_ID>>& grid);
public:
   LeeConstrPathStrategy(LEE_STRATEGY s);
   vector<UniCoord> constructLeePath(int ax,int ay,int bx,int by,
                                     int x_l,int x_h,
                                     int y_l,int y_h,
                                     vector<vector<ITEM_ID>>& grid);

   int calculateStepNum(VecIndex ax, VecIndex ay, VecIndex& bx, VecIndex& by,
                        int x_l,int x_h,
                        int y_l,int y_h,
                        vector<vector<ITEM_ID>>& grid);

   void setDensity(vector<vector<float>> *dens){pDensity = dens;}
   void setStrategy(LEE_STRATEGY s){strategy = s;}
   float getDensity(int ix,int iy);
};

#endif // LEECONSTRPATHSTRATEGY_H
