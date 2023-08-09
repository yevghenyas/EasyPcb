/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef PCBOARD_H
#define PCBOARD_H

#include <qwidget.h>
#include "roundplategraphicalitem.h"
#include "linegraphicalitem.h"
#include <map>
#include <vector>
#include <list>
#include <string>
#include <QAction>
#include "common.h"
#include <iostream>
#include "genericgraphicalitemscontainer.h"
#include "smartptr.h"
//#include "boardlayer.h"
#include "boardlayerswrapper.h"
#include <QPlainTextEdit>
#include "mousetrace.h"
#include "connectorwrapper.h"
#include "packagewrapper.h"



class MyWidget;

enum class ContainerType;


using namespace std;


//#define OLD_STYLE


class PcBoard;

class TextEditField : public QPlainTextEdit
{
public:
   TextEditField(QWidget *parent) :
       QPlainTextEdit(parent)
   {
   }
   virtual void keyPressEvent( QKeyEvent * event) override;
   void showText(int x,int y)
   {
      setGeometry(x,y,this->width(),this->height());
      setAutoFillBackground(false);
      setBackgroundVisible(true);
      setVisible(true);
   }
};


class ActionEx : public QAction
{
   QPoint m_pos;
public:
   ActionEx(const QString& text,QObject *parent):QAction(text,parent){}
   virtual ~ActionEx();
   void setPos(const QPoint& pos);
   QPoint getPos();
};

class NodeChangeItem
{
public:
    SmartPtr<GraphicalItem> m_pItem;
    int m_index{-1};
    PointF m_startPoint;
    PointF m_finishPoint;
    NodeChangeItem()
    {
       clear();
    }
    void clear()
    {
       m_index = -1;
       m_startPoint.setX(-1);
       m_startPoint.setY(-1);
       m_finishPoint.setX(-1);
       m_finishPoint.setY(-1);
    }
    void setItem(SmartPtr<GraphicalItem>& p,int index,const PointF& pt_orig,const PointF& pt)
    {
       m_pItem = p;
       m_index = index;
       m_startPoint = pt_orig;
       m_finishPoint = pt;
    }
    void finishMovement(PointF& )
    {
       clear();
    }
    void moveNode(PointF& pt)
    {
        m_finishPoint = pt;
        m_pItem->moveNode(pt,m_index);
    }
    bool isClear()
    {
       return (m_pItem.get());
    }
};




using VecLayerInrementer = std::vector<BoardLayer>::size_type;
using VecIdsInc = std::vector<int>::size_type;
using PcbLayoutVec = vector<vector<ITEM_ID> >;
using MapOfOccupCoord = map<ITEM_ID,list<QPoint> >;
using MapOfCollisions = map<string,ITEM_ID >;

class PcBoard :  public QWidget
{
    Q_OBJECT
    friend class AddItemCommand;
    friend class DeleteItemCommand;
    friend class MoveBetweenLayersCommand;
    friend class SetPropsCommand;
    friend class GroupCommand;
    friend class UngroupComand;
    friend class MoveNodeCommand;
    friend class AddNodeCommand;
    friend class SetBoardProps;
    friend class MoveItemCommand;
    friend class AutoCommandItem;
    //all visible levels in order of drawing;
//    vector<BoardLayer> m_layers;
//    list<VecLayerInrementer> m_selectionOrder;
    BoardLayersWrapper boardLayers;
    //visible level -> items for this level
    map<string,float> m_mapOfDragData;
    GraphicalItemsMap m_mapOfSelItems;
    GraphicalItemsMap m_mapofItemsToCopy;
    int m_currMode;
    //connector item used to construct connectors
    SmartPtr<ConnectorWrapper> m_tempConnector;
    //Connectable ruler
    SmartPtr<ConnectorWrapper> m_ConnectedRuler;
    SmartPtr<ConnectorWrapper> m_SimpleRuler;
    SmartPtr<PackageWrapper> m_packWrapper;
    //current zoom value
    unsigned short m_scaleFactor;
    float m_dInt,m_dExt,m_wLine;
    //current layer the user places items(except text) on
    BOARD_LEVEL_ID m_currentLevel;
    //for drawing rect and circle in MODE_PACK_RECT,MODE_PACK_ROUND
//    QPoint m_RectAndRndCoord[2];
    //used for adjusting connectors and rulers end points during dragging.
    QPoint m_RulerCoord[2];
    ActionEx *newNodeAct;
    ActionEx *delNodeAct;
    QAction *groupAct;
    QAction *ungroupAct;
    QAction *addToLib;
    QAction *propAct;
    QAction *copyAct;
    QAction *cutAct;
    QAction *pasteAct;
    QAction *rotateAct;
    void unselect();
//    int m_width,m_height;
    SchemData m_schemData;
    MyWidget *m_myWidget;
    QImage *m_pTemplateImage;
    NodeChangeItem m_nodeChangeItem;
    TextEditField *m_pText{nullptr};
    QString m_text;
    int text_x_pos{-1};
    int text_y_pos{-1};
    int textFontSize{-1};
    bool filledPack{false};
private slots:
    void newNodeAction();
    void delNodeAction();
    void groupAction();
    void ungroupAction();
    void addToLibAction();    
    void editPropsAction();
public:
    PcBoard(SchemData& data,MyWidget *parent = nullptr);
    virtual ~PcBoard();
    void setMode(int mode);
    void checkResetMode(MODE_DRAW mode);
    //d1 external D d2 internal d
    void preProcessPlate(float dOut,float dIn);
    void processSimpleGraphicalItems(QMouseEvent* e);
    void processGerber(QString& name);
    void processExcellon(QString& name);
    void processPlate(int x,int y);
    void preProcessMultiPlate(float dOut,float dIn);
    void processMultiPlate(int x,int y);
    void preProcessPlateRect(float w, float h);
    void processPlateRect(int x,int y);
    void processLine(float w);
    void processVcCon();
    void processPackRect(float wl,bool filled = false);
    void processPackRound(float wl);
    void processContainer(const QPoint& pos,const QString& type,
                          const QString& name,int n,SmartPtr<GraphicalItem>& pItem);
    void rotate();
    unsigned short getZoomValue(){return m_scaleFactor;}
    void createActions();
    void setBoardCurrentLevel(BOARD_LEVEL_ID level);
    void setBoardVisibleLevels(vector<BoardLevel>& levels);
    void setBoardVisibleLevel(BOARD_LEVEL_ID level,bool bSet);
    void print();
    void processSelectionDrop(QPoint pos,QPoint oldPos);    
    void setSelectionVisible(bool bVisible);
    void openNewSchematic(SchemData& data,vector<QString>& lNames,
                          vector< SmartPtr<GraphicalItem> >& lItems);
    void setSize(int w,int h);
    int  getWidth(){return m_schemData.m_width * m_scaleFactor/*m_width*/;}
    int  getHeight(){return m_schemData.m_height * m_scaleFactor /*m_height*/;}
    void loadImage(QString& fileName);
    void unloadImage();
    void processRuler();
    void processConnectedRuler(bool checked);
    void processCopy();
    void processPaste();
    void processCut();
    void processSelectAll();
    void constructPcbLayout();
    void processText(int fontSize,bool vcLevel);
    void updateBoardView(const QPoint& dif);
    BoardLayersWrapper* getBoardLayers(){return &boardLayers;}
    //return selecttion, needed when new object added to library
    GraphicalItemsMap *getSelection(){return &m_mapOfSelItems;}
protected:
//    void addGraphicalItemToLevel(BOARD_LEVEL_ID level,GraphicalItem *p);
//    void addGraphicalItemToLevels(QString& name,GraphicalItem* p);
    void addGraphicalItemToLevels(SmartPtr<GraphicalItem>& p);
//    void deleteGraphicalItem(const QString& name);
    void deleteGraphicalItem(SmartPtr<GraphicalItem>& p);
    void addVirtualConnector(SmartPtr<GraphicalItem>& p,map<GraphicalItem*,PointF>& connectorPoints);
    void cleanLinesMode();
    virtual void paintEvent(QPaintEvent*);
    QSizePolicy sizePolicy() const;
    virtual void mousePressEvent(QMouseEvent*);
    virtual void mouseMoveEvent(QMouseEvent*);
    virtual void mouseReleaseEvent(QMouseEvent*);
    virtual void keyPressEvent(QKeyEvent*);
    virtual void contextMenuEvent(QContextMenuEvent*);
    bool setItemsSelected(QPoint *pos,bool bCtrl);
    QPixmap getPixmapForSelection(int difX,int difY,const QSize& dif);
    void moveBetweenLayers(BOARD_LEVEL_ID newLevel,QString& s,
                           SmartPtr<GraphicalItem> p,ContainerType type);
    void resizeBoard();
    void clearCopyPasteMap();
    QString addItemToSchematic(SmartPtr<GraphicalItem>& p);
    void deleteItemFromSchematic(const QString& name);

    void moveItemBetweenLayers(BOARD_LEVEL_ID newLevel,QString& s,
                               SmartPtr<GraphicalItem>& p,ContainerType type);
    void setItemPosAndGeometry(GraphicalItem *p,PointF& pos, GeomCommonProps *props);
    bool groupCommand(GraphicalItemsMap& map,SmartPtr<GraphicalItem>& ptr);
    shared_ptr<GraphicalItemsMap> ungroupCommand(shared_ptr<GraphicalItemsMap::value_type> keyAndValue,
                                                 shared_ptr<GraphicalItemsMap> names);
    void ungroupCommand(SmartPtr<GraphicalItem>& pContainer);

//    void regroupCommand(QString& name,GenericGraphicalItemsContainer *pCont,GraphicalItemsMap& map);
    void regroupCommand(SmartPtr<GraphicalItem>& pCont,GraphicalItemsMap& map);
    bool fillPcb(PcbLayoutVec& m,GraphicalItemsMap* items,set<ITEM_ID>& ids,VecIdsInc ln,VecIdsInc cl);
    void createPointsForLineFromLeePath(vector<PointF>& points,
                                        vector<UniCoord>& px,
                                        vector<UniCoord>& py,
                                        int len);
    void handleProcessText(int x, int y);
    void handleConnectorCreation(QMouseEvent* e,bool bMsLeftButton);
    //init drag for items on pcboard
    void initDrag(QMouseEvent* e);
public slots:
    void zoomItems(int k);
    void zoomIn();
    void zoomOut();
    void setLineWidth(const QString& text);
};

#endif // PCBOARD_H
