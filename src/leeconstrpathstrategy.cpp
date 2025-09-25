#include "leeconstrpathstrategy.h"

LeeConstrPathStrategy::LeeConstrPathStrategy(LEE_STRATEGY s):
       strategy(s)
{

}

vector<UniCoord> LeeConstrPathStrategy::constructLeePath(int ax,int ay,int bx,int by,
                                                         int x_l,int x_h,
                                                         int y_l,int y_h,
                                                         vector<vector<int> >& grid)
{
   if(strategy == LEE_STRATEGY::SHORTEST_PATH)
      return constructLeePathShortest(ax,ay,bx,by,
                                      x_l,x_h,
                                      y_l,y_h,
                                      grid);
   else
      return constructLeePathLowDensity(ax,ay,bx,by,
                                        x_l,x_h,
                                        y_l,y_h,
                                        grid);
}

vector<UniCoord> LeeConstrPathStrategy::constructLeePathShortest(int ax,int ay,int bx,int by,
                                                                 int x_l,int x_h,
                                                                 int y_l,int y_h,
                                                                 vector<vector<int> >& grid)
{

}

vector<UniCoord> LeeConstrPathStrategy::constructLeePathLowDensity(int ax,int ay,int bx,int by,
                                                                   int x_l,int x_h,
                                                                   int y_l,int y_h,
                                                                   vector<vector<int> >& grid)
{

}

int LeeConstrPathStrategy::calculateStepNum(VecIndex ax, VecIndex ay, VecIndex& bx, VecIndex& by,
                                            int x_l, int x_h, int y_l, int y_h,
                                            vector<vector<ITEM_ID> > &grid)
{
   auto step_num_partial = 0;
   auto sizeY = grid.size();
   auto sizeX = grid.at(0).size();
   auto l = std::hypot(sizeX,sizeY);
   if(strategy == LEE_STRATEGY::SHORTEST_PATH)
   {
      //try to construct the part of the shortest path
      auto min_l = std::hypot(abs(static_cast<int>(bx-ax)),abs(static_cast<int>(by-ay)));
      auto min_x = ax;
      auto min_y = ay;
      //it may happen in some cases that the shortest path
      //consists of one cell. i.e. there is no path
      //because other connectors occupy nearest cells
      //for this case we use step_num_partial
      for(auto indexX = x_l; indexX <= x_h; ++indexX)
      {
         for(auto indexY = y_l; indexY <= y_h; ++indexY)
         {
            if(grid[indexY][indexX] > 0)
            {
//               cout<<"grid[indexY][indexX]="<<grid[indexY][indexX]<<endl;
               auto temp = std::hypot(abs(static_cast<int>(bx - indexX)),abs(static_cast<int>(by - indexY)));
               if(temp < min_l)
               {
                  min_l = temp;
                  min_x = indexX;
                  min_y = indexY;
                  step_num_partial = grid[indexY][indexX];
               }
            }
         }
      }
      bx = min_x;
      by = min_y;
   }
   else
   {
      if(pDensity == nullptr)
          throw std::logic_error{"Copper density is null"};
      auto minDensity = (*pDensity)[ay/cellNumber][ax/cellNumber];
      auto min = static_cast<float>(std::hypot(abs(static_cast<int>(bx-ax)),abs(static_cast<int>(by-ay))))/
              static_cast<float>(l) + minDensity;

      auto min_x = ax;
      auto min_y = ay;
      for(auto indexX = x_l; indexX <= x_h; ++indexX)
      {
         for(auto indexY = y_l; indexY <= y_h; ++indexY)
         {
            if(grid[indexY][indexX] > 0)
            {
//               cout<<"grid[indexY][indexX]="<<grid[indexY][indexX]<<endl;
               auto ix = indexX/cellNumber;
               auto iy = indexY/cellNumber;
               auto copperDens = getDensity(iy,ix);
               auto l_ratio = static_cast<float>(std::hypot(abs(static_cast<int>(bx - indexX)),
                                                            abs(static_cast<int>(by - indexY)))
                                                 )/static_cast<float>(l);
               auto temp = l_ratio + copperDens * 3;
               if(temp < min)
               {
                  min = temp;
                  min_x = indexX;
                  min_y = indexY;
                  step_num_partial = grid[indexY][indexX];
                  cout<<"****************min l_ratio="<<l_ratio<<" density="<<copperDens<<endl;
               }
            }
         }
      }
      bx = min_x;
      by = min_y;
   }
   return step_num_partial;
}

float LeeConstrPathStrategy::getDensity(int ix,int iy)
{
   float density = (*pDensity)[iy][ix] * 4;
   if(iy + 1 < static_cast<int>(pDensity->size()))
      density += (*pDensity)[iy + 1][ix] * 2;
   if(iy - 1 >= 0)
      density += (*pDensity)[iy - 1][ix] * 2;
   if(ix + 1 < static_cast<int>(pDensity->at(0).size()))
      density += (*pDensity)[iy][ix + 1] * 2;
   if(ix - 1 >= 0)
      density += (*pDensity)[iy][ix - 1] * 2;
}
