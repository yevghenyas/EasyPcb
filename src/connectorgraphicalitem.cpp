#include "connectorgraphicalitem.h"
#include "graphicalitemscontainer.h"
#include "drawwrapper.h"

using VectorInd = std::vector<PointF>::size_type;

ConnectorGraphicalItem::ConnectorGraphicalItem(vector<MouseTrace>& points,float w,BOARD_LEVEL_ID level,
                                               CONNECTOR_TYPE type,ITEM_ID id):
                                               GraphicalItem(level,0,0,id),m_w(w)
                                               ,m_style(LINE_STYLE::LINE_ROUNDED)
                                               ,m_connectorType(type)
{
    m_points.reserve(points.size());
    for (std::vector<QPoint>::size_type i = 0; i < points.size(); ++i)
    {
       PointF pt(points[i].getCoord());
       m_points.push_back(pt);
       if(points[i].isConnected())
       {
          SmartPtr<GraphicalItem> item = points[i].getConnectedItem();
          addConnectedNode(item);
       }
    }
    m_bVisible = true;
    resetCenterCoord();
}

ConnectorGraphicalItem::ConnectorGraphicalItem(vector<PointF>&& points,float w,BOARD_LEVEL_ID level,
                                               CONNECTOR_TYPE conType,ITEM_ID id):
                                               GraphicalItem(level,0,0,id)
                                               ,m_points(std::move(points)),m_w(w)
                                               ,m_style(LINE_STYLE::LINE_ROUNDED)
                                               ,m_connectorType(conType)

{
   m_bVisible = true;
   resetCenterCoord();
}

void ConnectorGraphicalItem::paintInternal(float difX,float difY,QPainter& p, QColor& c,int zoom,int zoom_d)
{
   PointF ptf(0.0f,0.0f);
   //correct points of connrction before drawing connector
   if(m_connectorType != CONNECTOR_TYPE::BOARD && m_vcCons)
   {
      //correct coordinates for connection point and draw them if needed
      for(auto& item:*m_vcCons)
      {
         float x = item.second->abs_x();
         float y = item.second->abs_y();
         m_points[static_cast<VectorInd>(item.first)].setX(
                     m_parent? x - m_parent->x() : x);
         m_points[static_cast<VectorInd>(item.first)].setY(
                     m_parent? y - m_parent->y() : y);
         if(m_connectorType != CONNECTOR_TYPE::BOARD)
            DrawWrapper::drawConnectorPlate(p,c,x + difX,y + difY,zoom);
      }
   }
   if(m_parent)
   {
      ptf.setX(m_parent->x());
      ptf.setY(m_parent->y());
   }
   PointF dif(difX,difY);
   if(m_points.size() > 1)
      DrawWrapper::drawLine(p,c,m_points,&ptf,dif,m_selected,
                            m_w,m_style,zoom,zoom_d);
}

ConnectorGraphicalItem::~ConnectorGraphicalItem()
{

}



void ConnectorGraphicalItem::paintItem(QPainter& p, QColor& c,int zoom,int zoom_d,BOARD_LEVEL_ID )
{
   if(m_bVisible)
   {
      paintInternal(0,0,p,c,zoom,zoom_d);
   }
}

void ConnectorGraphicalItem::paintItemIntoBuffer(float difX,float difY,QPainter& p, QColor& c,
                                                 int zoom,int zoom_d)
{
   paintInternal(difX,difY,p,c,zoom,zoom_d);
}

bool ConnectorGraphicalItem::isPointIn(int x, int y, BOARD_LEVEL_ID idLevel)
{
    for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
    {
       int xx,yy,xx1,yy1;
       if(i == 0)
       {
          if(m_parent)
          {
             xx = static_cast<int>((m_points[i].x() + m_parent->x()) * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i].y() + m_parent->y() * PIXELS_PER_MM);
          }
          else
          {
             xx = static_cast<int>(m_points[i].x() * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i].y() * PIXELS_PER_MM);
          }
          if (x == xx && y == yy)
             return true;
       }
       if (i > 0)
       {
          if(m_parent)
          {
             xx = static_cast<int>(m_points[i - 1].x() + m_parent->x() * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i - 1].y() + m_parent->y() * PIXELS_PER_MM);
             xx1 = static_cast<int>(m_points[i].x() + m_parent->x() * PIXELS_PER_MM);
             yy1 = static_cast<int>(m_points[i].y() + m_parent->y() * PIXELS_PER_MM);
          }
          else
          {
             xx = static_cast<int>(m_points[i - 1].x() * PIXELS_PER_MM);
             yy = static_cast<int>(m_points[i - 1].y() * PIXELS_PER_MM);
             xx1 = static_cast<int>(m_points[i].x() * PIXELS_PER_MM);
             yy1 = static_cast<int>(m_points[i].y() * PIXELS_PER_MM);
          }
          int ww = static_cast<int>((m_w * PIXELS_PER_MM)/2);
          //parallel case
          if (abs(xx1 - xx) < 2 &&
             x >= std::min(xx1, xx) - ww &&
             x <= std::max(xx1, xx) + ww &&
             y >= std::min(yy1, yy) &&
             y <= std::max(yy1, yy))
             return true;

          //perpendicular case
          if (abs(yy1 - yy) < 2 &&
             y >= std::min(yy1, yy) - ww &&
             y <= std::max(yy1, yy) + ww &&
             x >= std::min(xx1, xx) &&
             x <= std::max(xx1, xx))
             return true;

              //check if the point on the line determined by the two points
          if (x >= std::min(xx1, xx) &&
              x <= std::max(xx1, xx) &&
              y >= std::min(yy1,yy) &&
              y <= std::max(yy1,yy))
          {
             float dif_x = abs(static_cast<float>(xx1) - static_cast<float>(xx));
             float dif_y = abs(static_cast<float>(yy1) - static_cast<float>(yy));
             float tg = dif_y/dif_x;
             float cos = dif_x/std::sqrt(dif_x * dif_x + dif_y * dif_y);
             //point on the line that lies with the x = point.x
             int y_temp = 0;

             if((xx > xx1 && yy > yy1) || (xx1 >xx && yy > yy1))
                y_temp = static_cast<int>(yy - (static_cast<float>(abs(x - xx))) * tg);
             else
                y_temp = static_cast<int>(yy + (static_cast<float>(abs(x - xx))) * tg);

             ww /= cos;
             if(y <= y_temp +  ww && y >= y_temp - ww)
                return true;
          }
          else
          {
          }
       }
     }
     return false;
}

bool ConnectorGraphicalItem::isPointInF(float x, float y, BOARD_LEVEL_ID )
{
  for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
  {
     float xx,yy,xx1,yy1;
     if(i == 0)
     {
        if(m_parent != nullptr)
        {
           xx = m_points[i].x() + m_parent->x();
           yy = m_points[i].y() + m_parent->y();
        }
        else
        {
           xx = m_points[i].x();
           yy = m_points[i].y();
        }
     }
     if (i > 0)
     {
        if(m_parent != nullptr)
        {
           xx = m_points[i - 1].x() + m_parent->x();
           yy = m_points[i - 1].y() + m_parent->y();
           xx1 = m_points[i].x() + m_parent->x();
           yy1 = m_points[i].y() + m_parent->y();
        }
        else
        {
           xx = m_points[i - 1].x();
           yy = m_points[i - 1].y();
           xx1 = m_points[i].x();
           yy1 = m_points[i].y();
        }
        float ww = m_w;
        //parallel case
        if (abs(xx1 - xx) < 2 &&
           x >= std::min(xx1, xx) - ww &&
           x <= std::max(xx1, xx) + ww &&
           y >= std::min(yy1, yy) &&
           y <= std::max(yy1, yy))
           return true;

        //perpendicular case
        if (abs(yy1 - yy) < 2 &&
           y >= std::min(yy1, yy) - ww &&
           y <= std::max(yy1, yy) + ww &&
           x >= std::min(xx1, xx) &&
           x <= std::max(xx1, xx))
           return true;

        //check if the point in the plate determined by the two points
        if (x >= std::min(xx1, xx) &&
            x <= std::max(xx1, xx) &&
            y >= std::min(yy1,yy) &&
            y <= std::max(yy1,yy))
        {
           float dif_x = abs(xx1 - xx);
           float dif_y = abs(yy1 - yy);
           float tg = dif_y/dif_x;
           float cos = dif_x/std::sqrt(dif_x * dif_x + dif_y * dif_y);
           //point on the line that lies with the x = point.x
           float y_temp = 0;

           if((xx > xx1 && yy > yy1) || (xx1 >xx && yy > yy1))
              y_temp = yy - (abs(x - xx)) * tg;
           else
              y_temp = yy + (abs(x - xx)) * tg;

           ww = m_w/cos;
           if(y <= y_temp +  ww && y >= y_temp - ww)
              return true;
        }
        else
        {
        }
     }
   }
   return false;
}

void ConnectorGraphicalItem::moveDown(BOARD_LEVEL_ID )
{
   for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
   {
      m_points[i].setY(m_points[i].y() + 1.0f/PIXELS_PER_MM);
   }
   resetCenterCoord();
}

void ConnectorGraphicalItem::moveLeft(BOARD_LEVEL_ID )
{
   for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
   {
      m_points[i].setX(m_points[i].x() - 1.0f/PIXELS_PER_MM);
   }
   resetCenterCoord();
}

void ConnectorGraphicalItem::moveRight(BOARD_LEVEL_ID )
{
   for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
   {
      m_points[i].setX(m_points[i].x() + 1.0f/PIXELS_PER_MM);
   }
   resetCenterCoord();
}

void ConnectorGraphicalItem::moveUp(BOARD_LEVEL_ID )
{
  for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
  {
     m_points[i].setY(m_points[i].y() - 1.0f/PIXELS_PER_MM);
  }
  resetCenterCoord();
}

void ConnectorGraphicalItem::insertNode(int index)
{

   if (index >= 0 && index <= static_cast<int>(m_points.size()) - 1 )
   {
       PointF point(0.0f,0.0f);
       if(index == 0)
       {
          point.setX(m_points[0].x() + (m_points[1].x() -  m_points[0].x())/2);
          point.setY(m_points[0].y() + (m_points[1].y() -  m_points[0].y())/2);
          m_points.insert(m_points.begin() + 1,point);
       }
       else
       {
          point.setX(m_points[static_cast<VectorInd>(index) - 1].x()
                  + (m_points[static_cast<VectorInd>(index)].x() -
                  m_points[static_cast<VectorInd>(index) - 1].x())/2);
          point.setY(m_points[static_cast<VectorInd>(index) - 1].y()
                  + (m_points[static_cast<VectorInd>(index)].y() -
                     m_points[static_cast<VectorInd>(index) - 1].y())/2);
          m_points.insert(m_points.begin() + index,point);
       }
       resetCenterCoord();
   }
   return;
}


bool ConnectorGraphicalItem::removeNode(int index,PointF& pt)
{
   if(m_points.size() < 3)
      return false;
   if (index >= 0 && index <= static_cast<int>(m_points.size()) - 1 )
   {
      pt = m_points.at(static_cast<VectorInd>(index));
      m_points.erase(m_points.begin() + index);
      resetCenterCoord();
      return true;
   }
   return false;
}

bool ConnectorGraphicalItem::isAboveNodeInternal(QPoint &p, int &index)
{
    bool bRes = false;
    for( std::vector<PointF>::size_type i = 0 ; i <  m_points.size(); ++i)
    {
       int x = static_cast<int>(m_points[i].x() * PIXELS_PER_MM);
       int y = static_cast<int>(m_points[i].y() * PIXELS_PER_MM);
//       cout<<"x="<<x<<"y="<<y<<"pt.x="<<p.x()<<"pt.y="<<p.y()<<endl;
       float dif_x = x - p.x();
       float dif_y = y - p.y();
       int dd = static_cast<int>(sqrt(dif_x * dif_x + dif_y * dif_y));
       bRes =  (dd <= (m_w * PIXELS_PER_MM)/2);
       if(bRes)
       {
          index = static_cast<int>(i);
          p.setX(static_cast<int>(m_points[i].x() * PIXELS_PER_MM));
          p.setY(static_cast<int>(m_points[i].y() * PIXELS_PER_MM));
          return bRes;
       }
    }
    return bRes;
}

bool ConnectorGraphicalItem::isAboveNode(QPoint& p,int& index)
{
   if(!m_selected)
      return m_selected;
   return isAboveNodeInternal(p,index);
}

int ConnectorGraphicalItem::prepareToChange(const QPoint& p)
{
   int indNodeToMove = -1;
   //if we are above the node the index will be set to
   //some value and no need to check result
   isAboveNode(const_cast<QPoint&>(p),indNodeToMove);
   return indNodeToMove;
}


void ConnectorGraphicalItem::moveNode(PointF& p,int index)
{
   if(p.x() < 0)
   {
      //move is finished
      return;
   }
   if(index != -1)
   {
      m_points[static_cast<VectorInd>(index)].setX(p.x());
      m_points[static_cast<VectorInd>(index)].setY(p.y());
      resetCenterCoord();
   }
}

void ConnectorGraphicalItem::moveFor(BOARD_LEVEL_ID idLevel, PointF dif)
{
   //dif should prepared before calling this function according to zoom value
   if(m_level == idLevel || idLevel == LEVEL_ALL)
   {
      for (std::vector<PointF>::size_type i = 0; i < m_points.size(); ++i)
      {
         m_points[i] += dif;
//          m_points[i].setX(m_points[i].x() + dif.x());
//          m_points[i].setY(m_points[i].y() + dif.y());
         resetCenterCoord();
      }
   }
}



void ConnectorGraphicalItem::setX(float)
{
   //does not make sense for this item
   //use moveFor
}

void ConnectorGraphicalItem::setY(float)
{
   //does not make sense for this item
   //use moveFor
}

SmartPtr<GraphicalItem> ConnectorGraphicalItem::clone()
{
   PointF pt(m_x,m_y);
   vector<PointF> vec(m_points);
   auto p = SmartPtr<GraphicalItem>::make_smartptr<ConnectorGraphicalItem>(std::move(vec),m_w,m_level,m_connectorType,
                                                IDsGenerator::instance()->getNewID());
   p->setVisible(m_bVisible);
   p->setSelected(m_selected);
   p->setParent(m_parent,false);
   p->setType(m_type);
   return p;
}

void ConnectorGraphicalItem::setGeometry(GeomCommonProps& props)
{
   setWidth(props.line_g.width);
   m_style = props.line_g.style;
}

GraphicalItem* ConnectorGraphicalItem::isConnectable(PointF& ptf)
{
   int index = -1;
   QPoint pt(static_cast<int>(ptf.x()*PIXELS_PER_MM),
             static_cast<int>(ptf.y()*PIXELS_PER_MM));
   if (isAboveNodeInternal(pt,index))
   {
       float x = m_points[static_cast<VectorInd>(index)].x();
       float y = m_points[static_cast<VectorInd>(index)].y();
       if(m_parent)
           coordRelativeToAbsolute(x,y);
       ptf.setX(x);
       ptf.setY(y);
       return this;
   }
   return nullptr;
}




void ConnectorGraphicalItem::getConnectors(set<ITEM_ID>& connectors)
{
}




ITEM_ID ConnectorGraphicalItem::isRectOccupied(PointF& top,PointF& bottom)
{
   auto x1 = top.x();
   auto y1 = bottom.y();
   auto x2 = bottom.x();
   auto y2 = top.y();
   if(isPointInF(top.x(),top.y(),LEVEL_ALL) ||
      isPointInF(bottom.x(),bottom.y(),LEVEL_ALL) ||
      isPointInF(x1,y1,LEVEL_ALL) ||
      isPointInF(x2,y2,LEVEL_ALL) ||
      isPointInF((x1 + x2)/2,(y1 + y2)/2,LEVEL_ALL))
       return m_id;
   else {
       return ID_NONE;
   }
}



void ConnectorGraphicalItem::addConnectedNode(SmartPtr<GraphicalItem>& item)
{

   m_points.push_back(PointF(item->abs_x(),item->abs_y()));
   resetCenterCoord();
   if(!m_vcCons)
      m_vcCons.reset(new map<int,SmartPtr<GraphicalItem>>);
   (*m_vcCons)[static_cast<int>(m_points.size() - 1)] = item;
}

void ConnectorGraphicalItem::connect(ITEM_ID)
{

}

void ConnectorGraphicalItem::disconnect(ITEM_ID)
{

}

void ConnectorGraphicalItem::insertConnectedNode(int index,SmartPtr<GraphicalItem>& item)
{
   if(!m_vcCons)
       m_vcCons.reset(new map<int,SmartPtr<GraphicalItem>>);

   (*m_vcCons)[index] = item;
}

bool ConnectorGraphicalItem::isInArea(int x,int y)
{
   return isPointIn(x,y,BOARD_LEVEL_ID::LEVEL_ALL);
}

float ConnectorGraphicalItem::abs_x()
{
   if(m_x < 0)
      calculateCenterCoord();
   return m_x;
}

float ConnectorGraphicalItem::abs_y()
{
   if(m_y < 0)
      calculateCenterCoord();
   return m_y;
}

void ConnectorGraphicalItem::calculateCenterCoord()
{
   float maxX = -1;
   float maxY = -1;
   float minX = -1;
   float minY = -1;
   getMaxMinCoord(m_points,maxX,maxY,minX,minY);
   m_x = (maxX + minX)/2;
   m_y = (maxY + minY)/2;
}

PointF ConnectorGraphicalItem::getArea()
{
    float maxX = -1;
    float maxY = -1;
    float minX = -1;
    float minY = -1;
    getMaxMinCoord(m_points,maxX,maxY,minX,minY);
    PointF pt(maxX - minX,maxY - minY);
    return pt;
}

string ConnectorGraphicalItem::getGerberString(GerberGenerator& gen,
                                               BOARD_LEVEL_ID )
{
   return gen.getStringForConnector(getID(),width(),m_points);
}
