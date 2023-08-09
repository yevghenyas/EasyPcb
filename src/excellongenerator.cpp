#include "excellongenerator.h"
#include <vector>
#include <fstream>

ExcellonGenerator::ExcellonGenerator(BoardLayersWrapper *p):
       pBoardLayers(p)
{

}

void ExcellonGenerator::formatFloat(float val,char *buf ,
                                  int intLen,int fracLen)
{
   auto fractpart = 0.0f;
   auto intpart = 0.0f;
   fractpart  = std::modf(val,&intpart);
   for(int i = 0; i < fracLen; ++i)
      fractpart *= 10;

   char format[64];
   sprintf(format,"%%0%dd%%0%dd",intLen,fracLen);
   sprintf(buf,format,static_cast<int>(intpart),
           static_cast<int>(fractpart));

}


void ExcellonGenerator::generateFile(fstream& out)
{
   vector<BOARD_LEVEL_ID> layerIDs = {LEVEL_A,LEVEL_B,LEVEL_C,LEVEL_D,
                                      LEVEL_E,LEVEL_F};
   map<int,vector<PointF>> data;
   auto l = [&data](SmartPtr<GraphicalItem>& it)
   {
       if(it->isHoleCapable())
       {
          PointF pt(it->abs_x(),it->abs_y());
          auto d = static_cast<int>(it->getHoleDiameter() * PIXELS_PER_MM);
          pushIntoMappedVector(data,
                               d,
                               pt);
       }
   };
   for(const auto layerID:layerIDs)
   {
      //get layer and check if that exists
      auto layer = pBoardLayers->getLayer(layerID);
      if(layer == nullptr)
         continue;
      for(auto& item:*layer->getItemsInLevel())
      {
         //check if this item is a container and get children if needed
         GenericGraphicalItemsContainer *pC =
                  dynamic_cast<GenericGraphicalItemsContainer*>(item.second.get());
         if(pC != nullptr)
         {
            auto children = pC->getChildren();
            for(auto& child:*children)
            {
               //push it into map
               l(child);
            }
         }
         else
         {
            //push it into map
            l(item.second);
         }
      }
   }
   //first header
   out<<"M48"<<endl;
   out<<"METRIC,LZ"<<endl;
   int index = 0;
   map<int,int> m;
   for(const auto& item:data)
   {
      out<<"T"<<std::to_string(index)
        <<"C"<<std::to_string(static_cast<float>(item.first)/PIXELS_PER_MM)
        <<endl;
      m[item.first] = index++;
   }
   out<<"%"<<endl;
   out<<"G05"<<endl;
   out<<"G90"<<endl;
   //print coordinates
   for(const auto& item:data)
   {
      index = m.find(item.first)->second;
      out<<"T"<<index<<endl;
      for(const auto& listItem:item.second)
      {
         char bufX[64];
         char bufY[64];
         formatFloat(listItem.m_x,bufX,3,3);
         formatFloat(listItem.m_y,bufY,3,3);
         out<<"X"<<bufX<<"Y-"<<bufY<<endl;
      }      
   }
   out<<"M30"<<endl;
}
