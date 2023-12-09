#ifndef PCBAUTOCONSTRUCTOR_H
#define PCBAUTOCONSTRUCTOR_H

#include <vector>
#include <map>
#include "common.h"
#include "smartptr.h"
#include "boardlayerswrapper.h"

using VecLayerInrementer = std::vector<BoardLayer>::size_type;
using PcbLayoutVec = vector<vector<ITEM_ID> >;
using VecIdsInc = std::vector<int>::size_type;
using ConMap = map<BOARD_LEVEL_ID,vector<SmartPtr<GraphicalItem>>>;


#include "boardlayerswrapper.h"

class PcbAutoConstructor
{
    QWidget *m_parent{nullptr};
    const float fConWidth;
    const vector<BOARD_LEVEL_ID> layers;
public:
    PcbAutoConstructor(QWidget *p,const float fConWidth,
                       const vector<BOARD_LEVEL_ID>& l);

    vector<BOARD_LEVEL_ID> prepareLayersForAuto(ConMap& simleVcCons,
                         ConMap& dualVcCons);

    void constructPcbLayout(BoardLayersWrapper& boardLayers,                                 //input
                                                ConMap& simleVcCons,                         //input
                                                ConMap& dualVcCons,                          //input
                                                const int w,const int h,                     //input
                                                map<QString,SmartPtr<GraphicalItem>>& result,//output
                                                map<ITEM_ID,vector<SmartPtr<GraphicalItem>>>& mapOfMultiplates//output
                                                                                );

    map<QString,SmartPtr<GraphicalItem>> constructOneLayer(BoardLayer& boardLayer,
                                                           vector<SmartPtr<GraphicalItem>>& vcCons,
                                                           PcbLayoutVec& m,bool beforeLast = false);

    bool fillPcb(PcbLayoutVec& m,GraphicalItemsMap* items,set<ITEM_ID>& ids,VecIdsInc ln,VecIdsInc cl);
    void createPointsForLineFromLeePath(vector<PointF>& points,
                                                   vector<UniCoord>& pxy);
    void createPointsForLineFromLeePath(vector<PointF>& points,
                                        vector<UniCoord>& px,
                                        vector<UniCoord>& py,
                                        int len);
    SmartPtr<GraphicalItem> createMultiplateForConnector(SmartPtr<GraphicalItem>& con,BOARD_LEVEL_ID baseLayer);
    bool checkCell(vector<vector<int> >& grid,
                   vector<UniCoord> *v,
                   int step_num,
                   VecIndex& j,
                   VecIndex xi, VecIndex yi,
                   VecIndex bx, VecIndex by,
                   const set<ITEM_ID>& ids);
    bool lee(vector<vector<int> >& grid,
             int W,int H, //grid dimemsions
             VecIndex ax, VecIndex ay, VecIndex bx, VecIndex by, //start and end coordinates
             int& len, //resulting length of found way
             vector<UniCoord>& v0,
             vector<UniCoord>& v1,
             set<ITEM_ID> ids //ids of connectors we need to find way from...to
                    );
    //cycles based version of lee algotithm
    vector<UniCoord> lee(vector<vector<int> >& grid,
        VecIndex ax, VecIndex ay, // start and
        VecIndex bx, VecIndex by, // end coordinates
        set<ITEM_ID> ids,bool& bReturnPartial);
};

#endif // PCBAUTOCONSTRUCTOR_H
