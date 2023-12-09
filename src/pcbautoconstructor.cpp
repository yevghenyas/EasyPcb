#include "pcbautoconstructor.h"
#include "autodlg.h"
#include "qprogressdialog.h"
#include "multiplategraphicalitem.h"
#include "itemsfactory.h"






PcbAutoConstructor::PcbAutoConstructor(QWidget *p,const float fWidth,
                                       const vector<BOARD_LEVEL_ID>& l)
                                       : m_parent(p),fConWidth(fWidth),layers(l)
{

}


vector<BOARD_LEVEL_ID> PcbAutoConstructor::prepareLayersForAuto(ConMap& simpleVcCons,
                     ConMap& dualVcCons)
{
   vector<BOARD_LEVEL_ID> levels;

   if(simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_A) != simpleVcCons.end())
      levels.push_back(BOARD_LEVEL_ID::LEVEL_A);

   if(simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_F) != simpleVcCons.end())
      levels.push_back(BOARD_LEVEL_ID::LEVEL_F);

   if(simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_B) != simpleVcCons.end())
      levels.push_back(BOARD_LEVEL_ID::LEVEL_B);

   if(simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_C) != simpleVcCons.end())
      levels.push_back(BOARD_LEVEL_ID::LEVEL_C);

   if(simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_D) != simpleVcCons.end())
      levels.push_back(BOARD_LEVEL_ID::LEVEL_D);

   if(simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_E) != simpleVcCons.end())
      levels.push_back(BOARD_LEVEL_ID::LEVEL_E);

   auto comp = [&simpleVcCons](const BOARD_LEVEL_ID& a,
           const BOARD_LEVEL_ID& b)
           {
              if ( (a == BOARD_LEVEL_ID::LEVEL_A && b == BOARD_LEVEL_ID::LEVEL_F) ||
                   (b == BOARD_LEVEL_ID::LEVEL_A && a == BOARD_LEVEL_ID::LEVEL_F))
              {
                 auto sizeA = simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_A)->second.size();
                 auto sizeF = simpleVcCons.find(BOARD_LEVEL_ID::LEVEL_F)->second.size();
                 return a == BOARD_LEVEL_ID::LEVEL_A ? sizeA > sizeF : sizeF > sizeA;
              }
              else if(a == BOARD_LEVEL_ID::LEVEL_A || a == BOARD_LEVEL_ID::LEVEL_F)
              {
                 return true;
              }
              else if(b == BOARD_LEVEL_ID::LEVEL_A || b == BOARD_LEVEL_ID::LEVEL_F)
              {
                 return false;
              }
              else
              {
                 auto sizeA = simpleVcCons.find(a)->second.size();
                 auto sizeB = simpleVcCons.find(b)->second.size();
                 return sizeA > sizeB;
              }
           };


   std::sort(levels.begin(),levels.end(),comp);

}

bool PcbAutoConstructor::fillPcb(PcbLayoutVec& m,GraphicalItemsMap* items,set<ITEM_ID>& ids,
                         VecIdsInc ln,VecIdsInc cl)
{
    using PcbVectorIndex = std::vector<ITEM_ID>::size_type;
    auto layerId = BOARD_LEVEL_ID::LEVEL_A;
    for(auto& item:*items)
    {
//       if(ids.find(item.second->getID()) != ids.end())
//          continue;
       PointF top;
       PointF bottom;
       auto& p = item.second;
       GenericGraphicalItemsContainer *pc = nullptr;
       float fWidth = fConWidth;
       auto lambda = [fWidth,&top,&bottom,ids](GraphicalItem *item,PcbLayoutVec& pcb)
       {
          PointF pt = item->getArea();
          float x = item->abs_x();
          float y = item->abs_y();
          PointF topCoord{x - pt.x()/2,y - pt.y()/2};
          PointF bottCoord{x + pt.x()/2,y + pt.y()/2};
          QPoint topCell = getCellCoordForPoint(topCoord,fWidth);
          QPoint bottomCell = getCellCoordForPoint(bottCoord,fWidth);
          auto topIndexX = static_cast<PcbVectorIndex>(topCell.x());
          auto topIndexY = static_cast<PcbVectorIndex>(topCell.y());
          auto botIndexX = static_cast<PcbVectorIndex>(bottomCell.x());
          auto botIndexY = static_cast<PcbVectorIndex>(bottomCell.y());
          for(PcbVectorIndex i = topIndexX; i <= botIndexX; ++i)
          {
             for(PcbVectorIndex j = topIndexY; j <= botIndexY; ++j)
             {
                top.setX(i * fWidth);
                top.setY(j * fWidth);
                bottom.setX(top.x() + fWidth);
                bottom.setY(top.y() + fWidth);
                ITEM_ID id = item->isRectOccupied(top,bottom);
                //in case the cell is already occupied see if it has connectors connected
                if(id != ID_NONE)
                {
                   if(pcb[j][i] < WALL)
                   {
                      auto iter = ids.find(WALL - pcb[j][i]);
                      if(iter == ids.end())
                      {
                         iter = ids.find(id);
                         if(iter != ids.end())
                         {
                            pcb[j][i] = WALL - *iter;
                         }
                      }
                  }
                  else
                  {
                     pcb[j][i] = WALL - id;
                  }
               }
               else if(!(pcb[j][i] < WALL))
                  pcb[j][i] = ID_NONE;
             }
          }
       };
       if((pc = dynamic_cast<GenericGraphicalItemsContainer*>(p.get())) != nullptr)
       {
          auto children = pc->getChildren();
          for(auto& ch:*children)
             if(ch->getLevel() == layerId)
                lambda(ch.get(),m);
       }
       else
       {
          lambda(item.second.get(),m);
       }
   }
   return false;
}

void PcbAutoConstructor::createPointsForLineFromLeePath(vector<PointF>& points,
                                               vector<UniCoord>& px,
                                               vector<UniCoord>& py,
                                               int length)
{
    unsigned len = static_cast<unsigned>(length);
    points.push_back(PointF(px[0].pos * fConWidth + fConWidth/2,
                     py[0].pos * fConWidth + fConWidth/2));
    int dif = abs((px[1].pos - px[0].pos) * 2) + abs(py[1].pos - py[0].pos);
    for(VecIdsInc index = 1;
        index < static_cast<unsigned int>(len); ++index)
    {
       int dif_part1 = px[index].pos - px[index - 1].pos;
       int dif_part2 = py[index].pos - py[index - 1].pos;
       if(((abs(dif_part1 * 2) + abs(dif_part2)) != dif)
               || index ==static_cast<unsigned int>(len - 1))
       {
          dif = (abs(dif_part1 * 2) + abs(dif_part2));
          points.push_back(PointF(px[index - 1].pos * fConWidth + fConWidth/2,
                           py[index - 1].pos * fConWidth + fConWidth/2));

          px[index - 1] = -1;
          py[index - 1] = -1;
       }

    }
    points.push_back(PointF(px[len - 1].pos * fConWidth + fConWidth/2,
                     py[len - 1].pos * fConWidth + fConWidth/2));
}

void PcbAutoConstructor::createPointsForLineFromLeePath(vector<PointF>& points,
                                               vector<UniCoord>& pxy)
{
    auto len = pxy.size();
    points.push_back(PointF(pxy[0].coord.x * fConWidth + fConWidth/2,
                     pxy[0].coord.y * fConWidth + fConWidth/2));
    int dif = abs((pxy[1].coord.x - pxy[0].coord.x) * 2) + abs(pxy[1].coord.y - pxy[0].coord.y);
    for(VecIdsInc index = 1;
        index < static_cast<unsigned int>(len); ++index)
    {
       int dif_part1 = pxy[index].coord.x - pxy[index - 1].coord.x;
       int dif_part2 = pxy[index].coord.y - pxy[index - 1].coord.y;
       if(((abs(dif_part1 * 2) + abs(dif_part2)) != dif)
               || index ==static_cast<unsigned int>(len - 1))
       {
          dif = (abs(dif_part1 * 2) + abs(dif_part2));
          points.push_back(PointF(pxy[index - 1].coord.x * fConWidth + fConWidth/2,
                           pxy[index - 1].coord.y * fConWidth + fConWidth/2));

       }

    }
    points.push_back(PointF(pxy[len - 1].coord.x * fConWidth + fConWidth/2,
                     pxy[len - 1].coord.y * fConWidth + fConWidth/2));
}


//checks the cells if it matches the criteria and marks it apropriately
bool PcbAutoConstructor::checkCell(vector<vector<int> >& grid,
               vector<UniCoord>* v,
               int step_num,
               VecIndex& j,
               VecIndex xi, VecIndex yi,
               VecIndex bx, VecIndex by,
               const set<ITEM_ID>& ids)
{
   bool bFound = false;

   if(grid[yi][xi] == WALL)
       return bFound;

   if(grid[yi][xi] == EMPTY || (grid[yi][xi] < WALL &&
       ids.find(WALL - grid[yi][xi]) != ids.end()))
   {

      //check if close to this cell is not occupied by other items
      if(xi > 0)
         if(grid[yi][xi - 1] == WALL || (grid[yi][xi - 1] < WALL &&
              ids.find(WALL - grid[yi][xi - 1]) == ids.end()))
         {
//         cout << "grid[yi][xi - 1]=" << grid[yi][xi - 1]<<endl;
           return false;
         }
      //check if close to this cell is not occupied by other items
      if(yi > 0)
         if(grid[yi - 1][xi] == WALL || (grid[yi - 1][xi] < WALL &&
              ids.find(WALL - grid[yi - 1][xi]) == ids.end()))
         {
//           cout << "grid[yi - 1][xi]=" << grid[yi - 1][xi]<<endl;
            return false;
         }

      auto sizeY = grid.size();
      auto sizeX = grid.at(0).size();

      //check if close to this cell is not occupied by other items
      if(xi < sizeX - 1)
         if(grid[yi][xi + 1] == WALL || (grid[yi][xi + 1] < WALL &&
              ids.find(WALL - grid[yi][xi + 1]) == ids.end()))
         {
//         cout << "grid[yi][xi + 1]=" << grid[yi][xi + 1]<<endl;
            return false;
         }

      //check if close to this cell is not occupied by other items
      if(yi < sizeY - 1)
         if(grid[yi + 1][xi] == WALL || (grid[yi + 1][xi] < WALL &&
              ids.find(WALL - grid[yi + 1][xi]) == ids.end()))
         {
//            cout << "grid[yi + 1][xi]=" << grid[yi + 1][xi]<<endl;
            return false;
         }

   //now add the cell to list and mark the pcb appropriately
   if(j == v->size() - 1)
      throw logic_error("the memory for vector is exceeded");
   grid[yi][xi] = step_num;
   v->at(j++) = UniCoord(static_cast<char16_t>(xi),
                         static_cast<char16_t>(yi));
   if(xi == bx && yi == by)
      bFound = true;
   }
   return bFound;
}

vector<UniCoord> PcbAutoConstructor::lee(vector<vector<int> >& grid,
    VecIndex ax, VecIndex ay, // start and
    VecIndex bx, VecIndex by, // end coordinates
    set<ITEM_ID> ids,//ids of items already placed on pcb
    bool& bReturnPartial) //indicates that even not completed path should be returned
{
   bool bFound = false;
   vector<UniCoord> result;
   VecIndex x_l = ax;
   VecIndex x_h = ax;
   VecIndex y_l = ay;
   VecIndex y_h = ay;
   auto sizeY = grid.size();
   auto sizeX = grid.at(0).size();
   size_t  step_num = 0;
   auto bContinue = true;
   grid[ay][ax] = static_cast<int>(step_num);
   while (true)
   {
      for(auto indexX = x_l; indexX <= x_h; ++indexX)
      {
         for(auto indexY = y_l; indexY <= y_h; ++indexY)
         {  
            if(grid[indexY][indexX] == static_cast<int>(step_num) - 1)
            {
               if(indexX == bx && indexY == by)
               {
                  step_num = static_cast<size_t>(grid[by][bx]);
                  bFound = true;
               }
               auto l = [&grid,&ids,sizeX,sizeY](size_t ix,size_t iy){
                  //check if close to this cell is not occupied by other items
                  if(ix > 1)
                     if(grid[iy][ix - 1] < WALL && ids.find(WALL - grid[iy][ix - 1]) == ids.end())
                        return false;
                  if(iy > 1)
                     if(grid[iy - 1][ix] < WALL && ids.find(WALL - grid[iy - 1][ix]) == ids.end())
                        return false;

                  if(ix < sizeX - 1)
                     if(grid[iy][ix + 1] < WALL && ids.find(WALL - grid[iy][ix + 1]) == ids.end())
                        return false;

                  if(iy < sizeY - 1)
                     if(grid[iy + 1][ix] < WALL && ids.find(WALL - grid[iy + 1][ix]) == ids.end())
                        return false;

                  return true;
               };
               //mark cells after checks with step_num
               if(indexX > 0)
               {
                  bool bInConItem = (grid[indexY][indexX - 1] < WALL &&
                          ids.find(WALL - grid[indexY][indexX - 1]) != ids.end());
                  if(bInConItem || (grid[indexY][indexX - 1] == EMPTY &&
                                    l(indexX - 1,indexY)))
                  {
//                   cout << "grid[yi][xi - 1]=" << grid[yi][xi - 1]<<endl;
                     grid[indexY][indexX - 1] = static_cast<int>(step_num);
                     bContinue = true;
                  }
               }
               if(indexY > 0)
               {
                  bool bInConItem = (grid[indexY - 1][indexX] < WALL &&
                           ids.find(WALL - grid[indexY - 1][indexX]) != ids.end());

                  if(bInConItem || (grid[indexY - 1][indexX] == EMPTY &&
                       l(indexX,indexY - 1)))
                  {
//                     cout << "grid[yi - 1][xi]=" << grid[yi - 1][xi]<<endl;
                     grid[indexY - 1][indexX] = static_cast<int>(step_num);
                     bContinue = true;
                  }
               }
               if(indexX < sizeX - 1)
               {
                  bool bInConItem = (grid[indexY][indexX + 1] < WALL &&
                           ids.find(WALL - grid[indexY][indexX + 1]) != ids.end());

                  if( bInConItem || (grid[indexY][indexX + 1] == EMPTY &&
                                       l(indexX + 1,indexY)))
                  {
           //        cout << "grid[yi][xi + 1]=" << grid[yi][xi + 1]<<endl;
                     grid[indexY][indexX + 1] = static_cast<int>(step_num);
                     bContinue = true;
                  }
               }

               if(indexY < sizeY - 1)
               {
                  bool bInConItem = (grid[indexY + 1][indexX] < WALL &&
                          ids.find(WALL - grid[indexY + 1][indexX]) != ids.end());
                  if(bInConItem || (grid[indexY + 1][indexX] == EMPTY && l(indexX,indexY + 1)))
                  {
          //           cout << "grid[yi + 1][xi]=" << grid[yi + 1][xi]<<endl;
                     //now check the items around
                     grid[indexY + 1][indexX] = static_cast<int>(step_num);
                     bContinue = true;
                  }
               }
            }
         }         
      }

      if(bFound)
         break;

      if(!bContinue)
      {
         if(bReturnPartial)
            break;
         else
            return result;
      }

      if(x_l > 0)
         --x_l;
      if(y_l > 0)
         --y_l;
      if(x_h < sizeX - 1)
         ++x_h;
      if(y_h < sizeY - 1)
         ++y_h;


      bContinue = false;
      ++step_num;
      cout<<"step="<<step_num<<endl;
      if(step_num > 200)
         break;
   }
//   AutoDlg dlg(&grid,w,h,ax,ay,bx,by);
//   dlg.exec();

   auto construct_path = [&step_num,&grid,&result,sizeX,sizeY](int xx,int yy)
   {
      result.assign(step_num,-1);
      result[step_num - 1] = UniCoord(static_cast<char16_t>(xx),static_cast<char16_t>(yy));
      --step_num;
      for(; step_num > 0; --step_num)
      {
         UniCoord cur_point = result[step_num];
         cout<<"grid x - 1="<<grid[cur_point.coord.y][cur_point.coord.x - 1]<<endl;
         cout<<"grid x + 1="<<grid[cur_point.coord.y][cur_point.coord.x + 1]<<endl;
         cout<<"grid y - 1="<<grid[cur_point.coord.y - 1][cur_point.coord.x]<<endl;
         cout<<"grid y + 1="<<grid[cur_point.coord.y + 1][cur_point.coord.x]<<endl;
         if(cur_point.coord.x - 1 > 0 && grid[cur_point.coord.y][cur_point.coord.x - 1] == static_cast<int>(step_num))
         {
            result[step_num - 1] = UniCoord(cur_point.coord.x - 1,cur_point.coord.y);
            continue;
         }
         else if(cur_point.coord.x + 1 < sizeX && grid[cur_point.coord.y][cur_point.coord.x + 1] == static_cast<int>(step_num))
         {
            result[step_num - 1] = UniCoord(cur_point.coord.x + 1,cur_point.coord.y);
            continue;
         }
         else if(cur_point.coord.y - 1 > 0 && grid[cur_point.coord.y - 1][cur_point.coord.x] == static_cast<int>(step_num))
         {
            result[step_num - 1] = UniCoord(cur_point.coord.x,cur_point.coord.y - 1);
            continue;
         }
         else if(cur_point.coord.y + 1 < sizeY && grid[cur_point.coord.y + 1][cur_point.coord.x] == static_cast<int>(step_num))
         {
            result[step_num - 1] = UniCoord(cur_point.coord.x,cur_point.coord.y + 1);
            continue;
         }
      }
   };
   if(bFound)
   {
      construct_path(static_cast<int>(bx),static_cast<int>(by));
      bReturnPartial = false; //the path is completed
   }
   else if(bReturnPartial)
   {
      //try to construct the part of the shortest path
      auto min_l = std::hypot(bx-ax,by-ay);
      auto min_x = ax;
      auto min_y = ay;
      for(auto indexX = x_l; indexX <= x_h; ++indexX)
      {
         for(auto indexY = y_l; indexY <= y_h; ++indexY)
         {
            if(grid[indexY][indexX] > 0 && grid[indexY][indexX] <= step_num)
            {
               auto temp = std::hypot(bx - indexX,by - indexY);
               if(temp < min_l)
               {
                  min_l = temp;
                  min_x = indexX;
                  min_y = indexY;
                  step_num = grid[indexY][indexX];
               }
            }
         }
      }
      construct_path(static_cast<int>(min_x),static_cast<int>(min_y));
      bReturnPartial = true;//the path is not completed;
   }
   return result;
}

bool PcbAutoConstructor::lee(vector<vector<int> >& grid,
         int W,int H, //grid dimemsions
         VecIndex ax, VecIndex ay, VecIndex bx, VecIndex by, //start and end coordinates
         int& len, //resulting length of found way
         vector<UniCoord>& v0, //arrays for storing coordinates for analyzing
         vector<UniCoord>& v1, //one for current and the other for next interation
         set<ITEM_ID> ids //ids of connectors we need to find way from...to
                )
{
   grid[ay][ax] = 0;
   char16_t k = 1; //initial step number
   //initial coordinate = starting point
   v1[0] = UniCoord(static_cast<char16_t>(ax),
                   static_cast<char16_t>(ay));
   v1[1] = -1; //break marker after the last coordinate
   bool bFound = false; //successful result marker
   VecIndex j = 0;
   len = 0;
   do
   {
      auto looping = false;
      vector<UniCoord> *v_cur = k&1 ? &v1:&v0; //current array with coordinates to be analyzed
      vector<UniCoord> *v_next = k&1 ? &v0:&v1; //here we store next coordinates to analyze
      j = 0; //counter for array of next coordinates.
      int counter = 0;
      for(VecIndex i = 0; v_cur->at(i).pos != -1;++i, ++counter)
      {
         //take coordinate and analyze cells around them
         //add cells to the list of cells to be analyzed in case
         // they match criteria
         char16_t xx = v_cur->at(i).coord.x;
         char16_t yy = v_cur->at(i).coord.y;
         if((xx - 1) >= 0)
         {
            if((bFound = checkCell(grid,v_next,k,j,xx - 1,yy,bx,by,ids)))
               break;
         }
         if((xx + 1) < W)
         {
            if((bFound = checkCell(grid,v_next,k,j,xx + 1,yy,bx,by,ids)))
               break;
         }
         if((yy - 1) >= 0)
         {
            if((bFound = checkCell(grid,v_next,k,j,xx,yy - 1,bx,by,ids)))
               break;
         }
         if((yy + 1) < H)
         {
            if((bFound = checkCell(grid,v_next,k,j,xx,yy + 1,bx,by,ids)))
               break;
         }
      }
//      cout<<"counter"<<counter<<endl;
//      cout<<"k="<<k<<" j="<<j<<" found="<<bFound<<" looping="<<looping<<endl;
      if(bFound)
         break;
      v_next->at(j) = -1;
      ++k;
   }while(j);
   //restore the shortest path
   if(bFound)
   {
      //restore the calculated way
      auto& vx = v0;
      auto& vy = v1;
      len = k;
      vx[k] = UniCoord(static_cast<int>(bx));
      vy[k] = UniCoord(static_cast<int>(by));
      --k;

      while(true)
      {
         auto xx = static_cast<VecIndex>(vx[k + 1].pos);
         auto yy = static_cast<VecIndex>(vy[k + 1].pos);
         if(vy[k + 1].pos - 1 >= 0 &&
            grid[yy - 1][xx] == k)
         {
            vx[k] = vx[k + 1].pos;
            vy[k] = vy[k + 1].pos - 1;
         }
         else if(vy[k + 1].pos + 1 < H &&
                 grid[yy + 1][xx] == k)
         {
            vx[k] = vx[k + 1].pos;
            vy[k] = vy[k + 1].pos + 1;
         }
         else if(vx[k + 1].pos - 1 >= 0 &&
                 grid[yy][xx - 1] == k)
         {
            vx[k] = vx[k + 1].pos - 1;
            vy[k] = vy[k + 1].pos;
         }
         else if(vx[k + 1].pos + 1 < W &&
                grid[yy][xx + 1] == k)
         {
            vx[k] = vx[k + 1].pos + 1;
            vy[k] = vy[k + 1].pos;
         }
         else
         {
            len = 0;
            return false;
         }
         if(k == 0)
            break;
         --k;
      }
      return true;
   }
   else
      return false;
}


map<QString,SmartPtr<GraphicalItem>> PcbAutoConstructor::constructOneLayer(BoardLayer& boardLayer,
                                                       vector<SmartPtr<GraphicalItem>>& vcCons,
                                                       PcbLayoutVec& m,bool beforeLast)
{
    auto sizeY = m.size();
    auto sizeX = m.at(0).size();
    const int w = 25;
    const int h = 25;
    auto clearL = [&m,sizeX,sizeY](){
       for(VecIdsInc ln = 0; ln < sizeY; ++ln)
          for(VecIdsInc cl = 0; cl < sizeX; ++cl)
             m[ln][cl] = ID_NONE;
    };

    AUTO_SORT index = TOP_LEFT;
    map<AUTO_SORT,map<QString,SmartPtr<GraphicalItem>>> mapOfConnectors;
    while(true)
    {
       int value = 0;
       QProgressDialog progress("Processing connectors...",
                                "Abort processing", 0, 4,m_parent);
       progress.setWindowModality(Qt::WindowModal);

       set<QString> toDeleteSet;


       std::sort(vcCons.begin(),vcCons.end(),
                 [&index](SmartPtr<GraphicalItem> rp,SmartPtr<GraphicalItem> lp)
       {
          ConnectorGraphicalItem* connector1 = static_cast<ConnectorGraphicalItem*>(rp.get());
          ConnectorGraphicalItem* connector2 = static_cast<ConnectorGraphicalItem*>(lp.get());
          PointF start1 = connector1->getPoints()->at(0);
          PointF end1 = connector1->getPoints()->at(connector1->getPoints()->size() - 1);
          PointF start2 = connector2->getPoints()->at(0);
          PointF end2 = connector2->getPoints()->at(connector2->getPoints()->size() - 1);
          PointF rpt((start1.x() + end1.x())/2,(start1.y() + end1.y())/2);
          PointF lpt((start2.x() + end2.x())/2,(start2.y() + end2.y())/2);
          //return start1.y() < start2.y() && end1.y() < end2.y();
          if(index == TOP_LEFT)
             return rpt.y() < lpt.y() && rpt.x() < lpt.x();
          else if(index == TOP_RIGHT)
             return rpt.y() < lpt.y() && rpt.x() > lpt.x();
          else if(index == BOTTOM_LEFT)
             return rpt.y() > lpt.y() && rpt.x() < lpt.x();
          else
             return rpt.y() > lpt.y() && rpt.x() > lpt.x();
       });

       auto connectors = boardLayer.getConnectItemsInLevel();
       auto non_connector_items = boardLayer.getItemsInLevel();
       int len = 0;
       map<QString,SmartPtr<GraphicalItem>> vcIdToConPoints;
       for(VecIndex i = 0; i < vcCons.size();++i)
       {
           //clear board before start
           clearL();
           ConnectorGraphicalItem* connector = static_cast<ConnectorGraphicalItem*>(vcCons[i].get());
           cout<<"ID of connector:"<<connector->getID()<<endl;
           //now we get items that this connector connects to
           set<ITEM_ID> ids;
           auto pMap = connector->getConnectedItems();
           for(auto& item:*pMap)
              ids.insert(item.second->getID());


           fillPcb(m,non_connector_items,ids,0,0);
           fillPcb(m,connectors,ids,0,0);
           fillPcb(m,&vcIdToConPoints,ids,0,0);


           PointF startPoint = connector->getPoints()->at(0);
           PointF endPoint = connector->getPoints()->at(connector->getPoints()->size() - 1);
           int ax = static_cast<int>(startPoint.x()/fConWidth);
           int ay = static_cast<int>(startPoint.y()/fConWidth);
           int bx = static_cast<int>(endPoint.x()/fConWidth);
           int by = static_cast<int>(endPoint.y()/fConWidth);
           len = 0;


           //******** lee based on vectors call. commented
                         //if(PcbAutoConstructor::lee(m,static_cast<int>(sizeX),
                         //       static_cast<int>(sizeY),
                         //       static_cast<VecLayerInrementer>(ax),
                         //       static_cast<VecLayerInrementer>(ay),
                         //       static_cast<VecLayerInrementer>(bx),
                         //       static_cast<VecLayerInrementer>(by),len,px,py,ids))
           //********
           auto bReturnPartial = beforeLast;
           auto result = PcbAutoConstructor::lee(m,
                  static_cast<VecLayerInrementer>(ax),
                  static_cast<VecLayerInrementer>(ay),
                  static_cast<VecLayerInrementer>(bx),
                  static_cast<VecLayerInrementer>(by),ids,bReturnPartial);
           if(result.size() > 0)
           {
              vector<PointF> points;
//                 createPointsForLineFromLeePath(points,px,py,len);
              createPointsForLineFromLeePath(points,result);

              auto p = SmartPtr<GraphicalItem>::make_smartptr<ConnectorGraphicalItem>(std::move(points),
                                                fConWidth,
                                                boardLayer.getLevel(),
                                                CONNECTOR_TYPE::SCHEMATIC,
                                                IDsGenerator::instance()->getNewID());
              QString str_id = QString::number(connector->getID());
              vcIdToConPoints[str_id] = p;
              if(bReturnPartial)
              {

              }
           }
              //AutoDlg dlg(&m,w,h,ax,ay,bx,by);
              //dlg.exec();

       }

       if(vcIdToConPoints.size() == vcCons.size())
       {
          //we are done.create schematic connectors from points
          return vcIdToConPoints;
//             connectorsFromVcs(vcIdToConPoints,this);
          break;
       }
       else
       {
          //try differents sorting
          auto old_index = index;
          index = getNextAutoSort(index);
          if(index == SORT_NONE)
          {
             //we've tried all sortings
             //try to find out the best processing
             AUTO_SORT sortMax = index;
             size_t max = 0;
             for(auto oneProcessing:mapOfConnectors)
             {
                if(oneProcessing.second.size() > max)
                {
                   max = oneProcessing.second.size();
                   sortMax = oneProcessing.first;
                }
             }
             if(sortMax != SORT_NONE)
             {
                auto bestProcessing = mapOfConnectors.find(sortMax);
                return bestProcessing->second;
//                   connectorsFromVcs(bestProcessing->second,this);

             }
             break;
          }
          else
          {
             //store results
             mapOfConnectors[old_index] = std::move(vcIdToConPoints);
             //in case the map was copied(not moved) clear it
             vcIdToConPoints.clear();
          }
       }
       progress.setValue(++value);
       if (progress.wasCanceled())
          return map<QString,SmartPtr<GraphicalItem>>();

    }

}

SmartPtr<GraphicalItem> PcbAutoConstructor::createMultiplateForConnector(SmartPtr<GraphicalItem>& con,
                                                                         BOARD_LEVEL_ID baseLayer)
{
   auto level1 = con->getLevel();
   if(level1 == baseLayer) //in case the layers are equal we do not create anything
      return SmartPtr<GraphicalItem>();
   vector<BOARD_LEVEL_ID> levels = {level1,baseLayer};
   return ItemsFactory::createRoundMultiPlate(con->abs_x(),con->abs_y(),
                                                                  MULPLATE_D_EX,MULPLATE_D_IN,
                                                                  levels,ID_NONE,1,1);
}

void PcbAutoConstructor::constructPcbLayout(BoardLayersWrapper& boardLayers,
                                            ConMap& simleVcCons,                         //input
                                            ConMap& dualVcCons,                          //input
                                            const int w,const int h,                     //input
                                            map<QString,SmartPtr<GraphicalItem>>& result,//output
                                            map<ITEM_ID,vector<SmartPtr<GraphicalItem>>>& mapOfMultiplates//output
                                                                            )


{
    VecLayerInrementer sizeX = static_cast<VecLayerInrementer>(w/fConWidth);
    VecLayerInrementer sizeY = static_cast<VecLayerInrementer>(h/fConWidth);
    PcbLayoutVec m(sizeY,vector<ITEM_ID>(sizeX,ID_NONE));
    vector<UniCoord> px((sizeX + sizeY) * 2,ID_NONE);
    vector<UniCoord> py((sizeX + sizeY) * 2,ID_NONE);
    map<QString,vector<PointF> > mapOfLinesData;
    BOARD_LEVEL_ID prevLayerId = BOARD_LEVEL_ID::LEVEL_NONE;

    for(auto& oneLayer:simleVcCons)
    {
       if(prevLayerId != BOARD_LEVEL_ID::LEVEL_NONE &&
               simleVcCons.find(prevLayerId)->second.size() > 0)
       {
          //add items from previous layer with multiplates
          //and try to construct this layer using them
          for(auto& vcCon:simleVcCons.find(prevLayerId)->second)
          {
             auto connectors = static_cast<ConnectorGraphicalItem*>(vcCon.get())->getConnectedItems();
             vector<SmartPtr<GraphicalItem>> multiplates;
             for(auto& oneCon:*connectors)
             {
                auto mp = createMultiplateForConnector(oneCon.second,oneLayer.first);
                if(mp.get() != nullptr)//check for empty object
                   multiplates.push_back(mp);
             }
             oneLayer.second.push_back(vcCon);
             mapOfMultiplates[vcCon->getID()] = multiplates;
          }
       }
       auto boardLayer = boardLayers.getLayer(oneLayer.first);
       auto res = constructOneLayer(*boardLayer,oneLayer.second,m);
       //remove schematic connectors from input vector in case there were
       //consructed new pcb connectors for them
       for(auto& itemNew:res)
       {
          for(auto iter = oneLayer.second.begin(); iter != oneLayer.second.end(); ++iter)
             if((*iter)->getID() == itemNew.first.toInt())
             {
                oneLayer.second.erase(iter);
                break;
             }
       }
       result.insert(res.begin(),res.end());
       //delete multiplates we were not able to construct connector for
       for(auto& item:oneLayer.second)
       {
          mapOfMultiplates.erase(item->getID());
       }
       prevLayerId = oneLayer.first;
    }
}
