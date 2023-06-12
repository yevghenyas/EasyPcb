#ifndef BOARDLAYER_H
#define BOARDLAYER_H

#include<QString>
#include "common.h"
#include "smartptr.h"
#include "genericgraphicalitemscontainer.h"
#include "connectorgraphicalitem.h"

using GraphicalItemsMap = map<QString,SmartPtr<GraphicalItem> >;



class BoardLayer
{
    BOARD_LEVEL_ID idLevel;
    Qt::GlobalColor color;
    char desc[16];
    char colorDesc[24];
    bool bVisible;
    GraphicalItemsMap m_itemsInLevel;
    GraphicalItemsMap m_connectorsInLevel;

public:
    BoardLayer(BoardLevel& level);
    ~BoardLayer(){clear();}
    BOARD_LEVEL_ID getLevel(){return idLevel;}
    void setVisible(bool b){bVisible = b;}
    bool isItemInLevel(const QString& name);
    bool getItem(const QString& name,SmartPtr<GraphicalItem>& ptr);
    void addGraphicalItemToLevel(const QString& name,SmartPtr<GraphicalItem> item);
    void addGraphicalItemToLevel(SmartPtr<GraphicalItem> p);
    void deleteGraphicalItemFromLevel(const QString& name);
    void deleteGraphicalItemFromLevel(SmartPtr<GraphicalItem>& ptr);
    void deleteConnectorFromLevel(ConnectorGraphicalItem *p);
    void clear();
    bool isPointIn(int x,int y,QString &name_out,SmartPtr<GraphicalItem>& ptr);
    //chacks if the item connectable and paints properly
    bool checkPropertiesAndPaint(QPainter& p,int mode,PointF& pt,SmartPtr<GraphicalItem>& ptr,int zoom);
    void paintItems(QPainter& p,int mode,int zoom);
    GraphicalItemsMap* getItemsInLevel();
    GraphicalItemsMap* getConnectItemsInLevel();
    void copyItems(GraphicalItemsMap& dstMap) const;
};

#endif // BOARDLAYER_H
