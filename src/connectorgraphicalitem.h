#ifndef CONNECTORGRAPHICALITEM_H
#define CONNECTORGRAPHICALITEM_H

#include "graphicalitem.h"
#include <vector>
#include <QPoint>
#include <map>
using namespace std;

using GraphicalItemMap = map<ITEM_ID,GraphicalItem>;

class MouseTrace;

class ConnectorGraphicalItem : public GraphicalItem
{
   using VecCoordIdx = vector<PointF>::size_type;
   vector<PointF> m_points;
   float m_w;
   LINE_STYLE m_style;
   unique_ptr<map<int,SmartPtr<GraphicalItem>>> m_vcCons;
   CONNECTOR_TYPE m_connectorType;
   //id of temporary connector item.
   //used when constucting SCHEMATIC connector
   void paintInternal(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d);
   void calculateCenterCoord();
   inline void resetCenterCoord() noexcept {m_x = -1;m_y = -1;}
public:
   //this constructor creates only PCB connectors
   ConnectorGraphicalItem(vector<PointF>&& points,float w,BOARD_LEVEL_ID level,
                                                  CONNECTOR_TYPE conType,ITEM_ID id);
   //basic constructor
   ConnectorGraphicalItem(vector<MouseTrace>& points,float w,BOARD_LEVEL_ID level,
                          CONNECTOR_TYPE type,ITEM_ID id);

   ConnectorGraphicalItem(const ConnectorGraphicalItem& ) = delete;
   virtual ~ConnectorGraphicalItem() override;
   virtual void paintItem(QPainter& p, QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
   virtual void paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d) override;
   virtual bool isPointIn(int x, int y, BOARD_LEVEL_ID idLevel) override;
   virtual bool isPointInF(float x, float y, BOARD_LEVEL_ID idLevel) override;
   virtual void moveDown(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
   virtual void moveLeft(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
   virtual void moveRight(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
   virtual void moveUp(BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
   //insert node into spicific place of connector
   virtual void insertNode(int curNode) override;
   //push back a node
   void addNode(PointF& pt){m_points.push_back(pt);}
   virtual bool removeNode(int curNode, PointF& pt) override;
   virtual bool isAboveNode(QPoint& p,int& index) override;
   bool isAboveNodeInternal(QPoint& p,int& index);
   virtual int prepareToChange(const QPoint& p) override;
   virtual void moveNode(PointF& p,int index) override;
   virtual PointF getPos() override{return PointF(0,0);}
   virtual void setX(float x) override;
   virtual void setY(float y) override;
   virtual void moveFor(BOARD_LEVEL_ID idLevel, PointF dif) override;
   float width(){return m_w;}
   void setWidth(float w){m_w = w;}
   vector<PointF>* getPoints(){return &m_points;}
   virtual SmartPtr<GraphicalItem> clone() override;
   virtual void setGeometry(GeomCommonProps& props) override;
   virtual GeomCommonProps getGeometry() override {return GeomCommonProps(m_w,true);}
   virtual GraphicalItem* isConnectable(PointF& pt) override;
   //connect connectable item(i.e. creates schematic connection)
   void addConnectedNode(SmartPtr<GraphicalItem>& item);
   void insertConnectedNode(int index,SmartPtr<GraphicalItem>& item);
   void connect(ITEM_ID) override;
   virtual void disconnect(ITEM_ID connectorID) override;
   void getConnectors(set<ITEM_ID>& connectors) override;
   map<int,SmartPtr<GraphicalItem>>* getConnectedItems(){return m_vcCons.get();}
   virtual ITEM_ID isRectOccupied(PointF& ,PointF& ) override;
   LINE_STYLE getStyle(){return m_style;}
   bool isAboveTheCenter(PointF&) override {return false;}
   CONNECTOR_TYPE getConnectorType(){return m_connectorType;}
   virtual bool isInArea(int x,int y) override;
   virtual PointF getArea() override;
   virtual float abs_x() override;
   virtual float abs_y() override;
   virtual bool isGerberSupported()override {return true;}
   virtual string getGerberString(GerberGenerator& gen,
                                  BOARD_LEVEL_ID idLevel) override;
   PointF getFirstPoint(){return m_points[0];}
   void setFirstPoint(const PointF& pt){m_points[0] = pt;}
   PointF getLastPoint(){return m_points[m_points.size() - 1];}
   void setLastPoint(const PointF& pt){m_points[m_points.size() - 1] = pt;}
   PointF getPointAt(size_t index){return m_points[index];}
   void setPointAt(const PointF& pt,size_t index){m_points[index] = pt;}
};

#endif // CONNECTORGRAPHICALITEM_H
