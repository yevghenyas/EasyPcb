#ifndef TEXTGRAPHICALITEM_H
#define TEXTGRAPHICALITEM_H

#include "graphicalitem.h"

class TextGraphicalItem : public GraphicalItem
{
    QString m_text;
    int m_fontSize{12};
public:
    TextGraphicalItem(float x,float y,QString& text,int fontSize,
                      ITEMS_ORIENTATION o,BOARD_LEVEL_ID level,ITEM_ID id);
    virtual bool isPointIn(int x,int y, BOARD_LEVEL_ID idLevel) override;
    virtual bool isPointInF(float x,float y, BOARD_LEVEL_ID idLevel) override;
    virtual ITEM_ID isRectOccupied(PointF& top,PointF& bottom) override;
    virtual void paintItem(QPainter& p,QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID idLevel = LEVEL_NONE) override;
    virtual void paintItemIntoBuffer(float difX,float difY,QPainter&p ,QColor& c,int zoom,int zoom_d) override;
    virtual bool isAboveNode(QPoint& p,int& index) override;
    virtual int prepareToChange(const QPoint& p) override;
    virtual PointF getPos() override;
    virtual SmartPtr<GraphicalItem> clone() override;
    virtual void setGeometry(GeomCommonProps& props) override;
    virtual GeomCommonProps getGeometry() override;
    virtual void connect(ITEM_ID connectorID) override;
    virtual void disconnect(ITEM_ID connectorID) override;
    virtual void getConnectors(set<ITEM_ID>& connectors) override;
    int getConnectorsNumber() override {return 0;}
    void setFontSize(int size){m_fontSize = size;}
    int getFontSize(){return m_fontSize;}
    QString getText(){return m_text;}
    void setText(const QString& text){m_text = text;}
    virtual void rotate(BOARD_LEVEL_ID idLevel,int x,int y,int  grad) override;
    virtual bool isInArea(int x,int y) override;
    virtual PointF getArea() override;
};

#endif // TEXTGRAPHICALITEM_H
