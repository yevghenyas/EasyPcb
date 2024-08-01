#include "textgraphicalitem.h"
#include "drawwrapper.h"

TextGraphicalItem::TextGraphicalItem(float x,float y,QString& text,
                                     int fontSize,ITEMS_ORIENTATION o,BOARD_LEVEL_ID level,ITEM_ID id):
    GraphicalItem(level,x,y,id),m_text(text),m_fontSize(fontSize)
{
  QFont fnt("Arial",fontSize);
  m_type = o;
}

 bool TextGraphicalItem::isPointIn(int x,int y, BOARD_LEVEL_ID )
 {
    float xf,yf;
    coordRelativeToAbsolute(xf,yf);

    QScreen* pScreen = QGuiApplication::primaryScreen();
    auto ratioX = static_cast<float>(pScreen->physicalDotsPerInchX()/72);
    auto ratioY = static_cast<float>(pScreen->physicalDotsPerInchY()/72);

    auto size = static_cast<float>(m_fontSize);
    auto baseX = static_cast<int>(xf * PIXELS_PER_MM);
    auto baseY = static_cast<int>(yf * PIXELS_PER_MM);

    auto w = static_cast<int>((size * m_text.length() * ratioX)/2);
    auto h = static_cast<int>((size * ratioY)/2);
    bool bHor = m_type == ITEMS_ORIENTATION::O_HORIZONTAL_LEFT ||
            m_type == ITEMS_ORIENTATION::O_HORIZONTAL_RIGHT;

    auto dHor = bHor ? w : h;
    auto dVert = bHor ? h : w;


    return (y > baseY - dVert && y < baseY + dVert) &&
            (x > baseX - dHor &&
             x < baseX + dHor);

 }
 bool TextGraphicalItem::isPointInF(float x,float y, BOARD_LEVEL_ID )
 {
    return isPointIn(static_cast<int>(x*PIXELS_PER_MM),
                     static_cast<int>(y*PIXELS_PER_MM),
                     BOARD_LEVEL_ID::LEVEL_ALL);
 }
 ITEM_ID TextGraphicalItem::isRectOccupied(PointF& ,PointF& )
 {

 }
 void TextGraphicalItem::paintItem(QPainter& p,QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID idLevel)
 {
    if(m_level == idLevel)
    {
//       auto fSize = static_cast<float>(m_fontSize)/PIXELS_PER_MM;
       //auto xx = m_x - (m_text.length() * fSize/4);
       //auto yy = m_y - fSize/2;
        float x1,y1;
        coordRelativeToAbsolute(x1,y1);

       float centerX = -1;
       float centerY = -1;
/*
       if(m_parent != nullptr)
       {
          centerX = m_parent->x();
          centerY = m_parent->y();
       }
*/
       DrawWrapper::drawText(p,c,m_text,m_fontSize,centerX,centerY,x1,y1,
                             m_type,zoom,zoom_d);
    }
 }
 void TextGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter&p ,QColor& c,
                                             int zoom,int zoom_d)
 {

     float x1,y1;
     coordRelativeToAbsolute(x1,y1);

     float centerX = -1;
     float centerY = -1;
/*
     if(m_parent != nullptr)
     {
        centerX = m_parent->x();
        centerY = m_parent->y();
     }
*/



     DrawWrapper::drawText(p,c,m_text,m_fontSize,
                           centerX > 0 ? centerX + difX : centerX,
                           centerY > 0 ? centerY + difY : centerY,
                           x1 + difX,
                           y1 + difY,
                           m_type,zoom,zoom_d);
 }

 bool TextGraphicalItem::isAboveNode(QPoint& ,int& )
 {
    return false;
 }
 int TextGraphicalItem::prepareToChange(const QPoint& )
 {

 }
 PointF TextGraphicalItem::getPos()
 {
    PointF pt(m_x,m_y);
    return pt;
 }
 SmartPtr<GraphicalItem> TextGraphicalItem::clone()
 {
    auto p = SmartPtr<GraphicalItem>::make_smartptr<TextGraphicalItem>(m_x,
                                                                       m_y,
                                                                       m_text,
                                                                       m_fontSize,
                                                                       m_type,
                                                                       m_level,
                                                        IDsGenerator::instance()->getNewID());
    p->setVisible(m_bVisible);
    p->setSelected(m_selected);
    p->setParent(m_parent,false);
    return p;
 }
 void TextGraphicalItem::setGeometry(GeomCommonProps& props)
 {
    m_fontSize = props.txt_g.fntSize;
 }
 GeomCommonProps TextGraphicalItem::getGeometry()
 {

 }
 void TextGraphicalItem::getConnectors(set<ITEM_ID>& )
 {

 }

 void TextGraphicalItem::connect(ITEM_ID )
 {

 }

 void TextGraphicalItem::rotate(BOARD_LEVEL_ID idLevel,int x,int y,int grad )
 {
     if(idLevel != m_level && idLevel != LEVEL_ALL)
        return;

     float x1,y1;
     coordRelativeToAbsolute(x1,y1);
     PointF center(static_cast<float>(x)/PIXELS_PER_MM,
                   static_cast<float>(y)/PIXELS_PER_MM);
     if(x == -1)
        center.setX(x1);
     if(y == -1)
        center.setY(y1);

     rotateRectShape(center,x1,y1,grad,m_type);
     coordAbsoluteToRelative(x1,y1);
/*
     switch(m_type)
     {
        case O_VERTICAL_TOP:
           m_type = O_HORIZONTAL_RIGHT;
           break;
        case O_HORIZONTAL_RIGHT:
           m_type = O_VERTICAL_BOTTOM;
           break;
        case O_VERTICAL_BOTTOM:
           m_type = O_HORIZONTAL_LEFT;
           break;
        case O_HORIZONTAL_LEFT:
           m_type = O_VERTICAL_TOP;
           break;
        default:
           break;
     }
*/
 }

 void TextGraphicalItem::disconnect(ITEM_ID )
 {

 }

 bool TextGraphicalItem::isInArea(int x,int y)
 {
    return isPointIn(x,y,LEVEL_ALL);
 }

 PointF TextGraphicalItem::getArea()
 {
     QScreen* pScreen = QGuiApplication::primaryScreen();
     auto ratioX = static_cast<float>(pScreen->physicalDotsPerInchX()/72);
     auto ratioY = static_cast<float>(pScreen->physicalDotsPerInchY()/72);
     PointF pt(m_fontSize * ratioX * m_text.size()/PIXELS_PER_MM,
               ratioY * m_fontSize/PIXELS_PER_MM);
     return pt;
 }
