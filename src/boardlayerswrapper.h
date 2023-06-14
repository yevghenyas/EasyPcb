#ifndef BOARDLAYERSWRAPPER_H
#define BOARDLAYERSWRAPPER_H

#include "common.h"
#include <map>
#include <vector>
#include "graphicalitem.h"
#include "boardlayer.h"
#include "filewriter.h"

using GraphicalItemsMap = map<QString,SmartPtr<GraphicalItem> >;
using VecLayerInrementer = vector<BoardLayer>::size_type;
using VecLevelsInrementer = vector<BoardLevel>::size_type;

class BoardLayersWrapper
{
   map<BOARD_LEVEL_ID,int> levelIdToInd;
   vector<BoardLayer> boardLayers;
   list<VecLayerInrementer> m_selectionOrder;
public:
   BoardLayersWrapper();
   void initLayers(vector<BoardLevel>& levels);
   BoardLayer* getLayer(BOARD_LEVEL_ID levelId);
   void paintLayers(QPainter& p,MODE_DRAW mode,int zoom_plus);
   void checkIfHoveringAndPaint(QPainter& p,MODE_DRAW mode,PointF& pt,
                                SmartPtr<GraphicalItem>& ptr,int zoom_plus);
   SmartPtr<GraphicalItem> findItem(const QString& name,BOARD_LEVEL_ID idLevel = LEVEL_ALL);
   SmartPtr<GraphicalItem> findSchematicConnectorItem(QString& name);
   bool addGraphicalItemToLayers(SmartPtr<GraphicalItem>& ptr);
   bool deleteGraphicalItemFromLayers(SmartPtr<GraphicalItem>& ptr);
   bool isPointIn(int x,int y,QString &name_out,SmartPtr<GraphicalItem>& ptr);
   void copyItems(GraphicalItemsMap& dstMap) const;
   void removeAll();
   void writeLayersToFile(FileWriter& writer);
   void checkSelectForArea(const QPoint& top,const QPoint& bottom,
                           GraphicalItemsMap& mapOfSelection);
   //returns layers with items
   void getNonEmptyLayersIds(set<BOARD_LEVEL_ID>& layersSet,
                             bool bActiveLayersOnly = false);
};

#endif // BOARDLAYERSWRAPPER_H
