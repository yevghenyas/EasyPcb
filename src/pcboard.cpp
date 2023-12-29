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
#include <qpainter.h>
#include "pcboard.h"
#include <QMouseEvent>
#include <QMenu>
#include <QMimeData>
#include <QDrag>
#include <QScreen>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <QFileDialog>
#include "ruler.h"
#include "mywidget.h"
#include <QtPrintSupport/QPrinter>
#include "rectgraphicalitem.h"
#include "drawwrapper.h"
#include "itemsfactory.h"
#include <QtPrintSupport/QPrintDialog>
#include "addneweltolibdlg.h"
#include "graphicalitempropsdlg.h"
#include "pcboardpropsdlg.h"
#include "setprintpropsdlg.h"
#include "additemcommand.h"
#include "deleteitemcommand.h"
#include "movebetweenlayerscommand.h"
#include "setpropscommand.h"
#include "groupcommand.h"
#include "ungroupcomand.h"
#include "moveitemcommand.h"
#include "movenodecommand.h"
#include "addnodecommand.h"
#include "deletenodecommand.h"
#include "rotateitemcommand.h"
#include "setboardprops.h"
#include "textgraphicalitem.h"
#include "smartptr.h"
#include <QMessageBox>
#include "autodlg.h"
#include "pcbautoconstructor.h"
#include "autoconstructprops.h"
#include "multiplategraphicalitem.h"
#include "editmultiplate.h"
#include "autocommanditem.h"
#include "editexporttogerberprops.h"
#include "excellongenerator.h"

using namespace std;


QPoint ActionEx::getPos()
{
   return m_pos;
}

void ActionEx::setPos(const QPoint& pos)
{
   m_pos.setX(pos.x());
   m_pos.setY(pos.y());
}

ActionEx::~ActionEx()
{

}


void TextEditField::keyPressEvent( QKeyEvent * event)
{
   if( event->key() == Qt::Key_Return )
   {
      // optional: if the QPlainTextEdit should do its normal action
      // even when the return button is pressed, uncomment the following line
      // QPlainTextEdit::keyPressEvent( event )
      if(PcBoard *p = dynamic_cast<PcBoard*>(parent()))
      {
         p->setMode(MODE_CURSOR);
      }
      event->accept();
   }
   else
      QPlainTextEdit::keyPressEvent( event );
}





PcBoard::PcBoard(SchemData& data,MyWidget *parent) : QWidget(parent),m_currMode(0),
                     m_scaleFactor(1),m_dInt(6),m_dExt(10),
                     m_currentLevel(BOARD_LEVEL_ID::LEVEL_A),//m_width(w),m_height(h),
                     m_myWidget(parent),m_pTemplateImage(nullptr),m_schemData(data)
{
    QPalette pal = palette();
    //yellow
    pal.setColor(QPalette::Background,bckgColor);
    setAutoFillBackground(true);
    setPalette(pal);
    setFocusPolicy(Qt::StrongFocus);
    setMouseTracking(true);
    createActions();
    setMinimumSize(QSize(data.m_width,data.m_height));
//    m_RectAndRndCoord[0] = m_RectAndRndCoord[1] = QPoint(-1,-1);
    m_RulerCoord[0] = m_RulerCoord[1] = QPoint(-1,-1);
    m_tempConnector = SmartPtr<ConnectorWrapper>::make_smartptr<ConnectorWrapper>();
    m_ConnectedRuler = SmartPtr<ConnectorWrapper>::make_smartptr<ConnectorWrapper>();
    m_SimpleRuler = SmartPtr<ConnectorWrapper>::make_smartptr<ConnectorWrapper>();
    m_packWrapper = SmartPtr<PackageWrapper>::make_smartptr<PackageWrapper>();
}

PcBoard::~PcBoard()
{
    m_mapOfSelItems.clear();
    m_mapOfDragData.clear();
    clearCopyPasteMap();
}


void PcBoard::paintEvent( QPaintEvent * e)
{
    QPainter p( this );
    //erase contents to redraw completely
    p.eraseRect(e->rect());
    //draw background image if any from menu tools->loadImage
    if(m_pTemplateImage)
    {
       p.drawImage(0,0,*m_pTemplateImage);
    }
    QColor c(Qt::black);
    //when in print mode the background is white
    if(m_currMode == MODE_PRINT)
       p.fillRect(e->rect(),Qt::white);

    m_tempConnector->clearConnector();
    m_ConnectedRuler->clearConnector();

    SmartPtr<GraphicalItem> ptr;
    if(m_tempConnector->isConnecting() || m_ConnectedRuler->isConnecting())
    {
       PointF pt = m_tempConnector->isConnecting() ? m_tempConnector->getPoint() :
                                          m_ConnectedRuler->getPoint();

       boardLayers.checkIfHoveringAndPaint(p,static_cast<MODE_DRAW>(m_currMode),pt,ptr,m_scaleFactor);
       if(m_tempConnector->isConnecting())
          m_tempConnector->setConnector(ptr);
       if(m_ConnectedRuler->isConnecting())
          m_ConnectedRuler->setConnector(ptr);
    }
    else
        boardLayers.paintLayers(p,static_cast<MODE_DRAW>(m_currMode),m_scaleFactor);

    m_tempConnector->paintItem(p,m_scaleFactor);
    m_ConnectedRuler->paintItem(p,m_scaleFactor);
    m_SimpleRuler->paintItem(p,m_scaleFactor);
    m_packWrapper->paintPackage(p,m_currentLevel,m_scaleFactor);

/* temporarily
    if(m_RectAndRndCoord[0].x() != -1 && m_RectAndRndCoord[1].x() != -1)
    {
        QColor c = m_currMode == MODE_CURSOR ?
                   selectColor :
                   LevelsWrapper::getColorForLevel(m_currentLevel);
       m_packWrapper->paintPackage(p,c,m_scaleFactor);

       if(m_currMode == MODE_PACK_RECT || m_currMode == MODE_CURSOR)
       {
          DrawWrapper::drawRectPackage(p,c,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                                       (m_RectAndRndCoord[0].x() + m_RectAndRndCoord[1].x())/2,
                                       (m_RectAndRndCoord[0].y() + m_RectAndRndCoord[1].y())/2,
                                        m_RectAndRndCoord[1].x() - m_RectAndRndCoord[0].x(),
                                        m_RectAndRndCoord[1].y() - m_RectAndRndCoord[0].y());
          
       }
       else if(m_currMode == MODE_PACK_ROUND)
       {
          QColor c(LevelsWrapper::getColorForLevel(m_currentLevel));
          DrawWrapper::drawRoundPackage(p,c,
                                            (m_RectAndRndCoord[0].x() + m_RectAndRndCoord[1].x())/2,
                                            (m_RectAndRndCoord[0].y() + m_RectAndRndCoord[1].y())/2,
             m_RectAndRndCoord[1].x() - m_RectAndRndCoord[0].x(),
             m_RectAndRndCoord[1].y() - m_RectAndRndCoord[0].y(),1);

       }

       p.setPen(Qt::NoPen);       
    }
*/
}

QSizePolicy PcBoard::sizePolicy() const
{
   cout<<"Reading policy"<<endl;
   return QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}

void PcBoard::unselect()
{
   for(auto item:m_mapOfSelItems)
      item.second->setSelected(false);
   m_mapOfSelItems.clear();            
}

void PcBoard::setSelectionVisible(bool bVisible)
{
   
   if(bVisible)
      cout<<"visible"<<endl;
   else
      cout<<"invisible"<<endl;
   for(auto item:m_mapOfSelItems)
      item.second->setVisible(bVisible);
}


bool PcBoard::setItemsSelected(QPoint *pos,bool bCtrl)
{
   for(auto item:m_mapOfSelItems)
   {
      if(item.second->isPointIn(pos->x()/m_scaleFactor,
                                pos->y()/m_scaleFactor,LEVEL_ALL))
      {
         if(!bCtrl)
         {
            return true;
         }
         else
         {
            item.second->setSelected(false);
            m_mapOfSelItems.erase(item.first);
            return false;
         }
      }
   }
   QString name;
   SmartPtr<GraphicalItem> pItem;
   if(boardLayers.isPointIn(pos->x()/m_scaleFactor,pos->y()/m_scaleFactor,name,pItem))
   {
      if(!bCtrl)
      {
         unselect();
      }
      pItem->setSelected(true);
      m_mapOfSelItems[name] = pItem;
      return false;
   }
   unselect();
   return false;

/*
   ///***************
   ///new
   ///
//   for(VecLayerInrementer i = 0 ; i < m_layers.size(); ++i)
   for(list<VecLayerInrementer>::iterator iter = m_selectionOrder.begin();
       iter != m_selectionOrder.end(); ++iter)
   {
      QString name;
      SmartPtr<GraphicalItem> pItem;
      int index  =  *iter;
      if(m_layers[*iter].isPointIn(pos->x()/m_scaleFactor,pos->y()/m_scaleFactor,name,pItem))
      {
         if(!bCtrl)
         {
            unselect();
         }
         pItem->setSelected(true);
         m_mapOfSelItems[name] = pItem;
         return false;         
      }
   }
   unselect();   
   return false;
*/
}

QPixmap PcBoard::getPixmapForSelection(int difX,int difY,const QSize& size)
{
   cout <<"setting buffer:"<<this->width()<<" "<<this->height()<<endl;

   QPixmap buffer(size.width(),
                  size.height());
   buffer.fill(Qt::transparent);
   QPainter p(&buffer);
   QColor c(LevelsWrapper::getColorForSelection());
   int zoom = m_scaleFactor;
   int zoom_d = 1;
   //
   for(auto item:m_mapOfSelItems)
   {
      item.second->paintItemIntoBuffer(difX/PIXELS_PER_MM/zoom,
                                       difY/PIXELS_PER_MM/zoom,
                                       p,c,zoom,zoom_d);
   }
//   return buffer.scaled(this->width(),this->height());

   return buffer;
}

void PcBoard::processSelectionDrop(QPoint pos,QPoint oldPos)
{
   //reset dragging coordinate difference
   float dif_x = static_cast<float>((pos - oldPos).x())/m_scaleFactor/PIXELS_PER_MM;
   float dif_y = static_cast<float>((pos - oldPos).y())/m_scaleFactor/PIXELS_PER_MM;
   PointF dif(dif_x,dif_y);
   char buf[64];
//   sprintf(buf,"dif.x=%f,dif.y=%f",dif.x(),dif.y());
   cout<<buf<<endl;
   for(auto& item:m_mapOfSelItems)
      item.second->releaseDragCoord();
   m_myWidget->getUndoStack()->push(new MoveItemCommand(m_mapOfSelItems,dif,this));
/*
   for(auto item:m_mapOfSelItems)
   {
//      sprintf(buf,"x=%f,y=%f",item.second->x(),item.second->y());
      cout<<buf<<endl;
      item.second->moveFor(LEVEL_ALL,dif);
      item.second->setVisible(true);      
      cout<<"visualize"<<endl;
//      sprintf(buf,"x=%f,y=%f",item.second->x(),item.second->y());
      cout<<buf<<endl;      
   }
*/
   cout<<"drop selection"<<endl;
}

void PcBoard::initDrag(QMouseEvent* e)
{
//   cout<<"mode cursor"<<endl;
   QPoint pos = e->pos();
   //check,add or remove item from selection list
   if(setItemsSelected(&pos, e->modifiers() & Qt::ControlModifier))
   {
      //calculate the pos realtive to parent
      QPoint parentPos = mapToParent(e->pos());
      QPoint dif = parentPos - pos;
//      cout<<"scroll area width:"<<m_myWidget->getScrollArea()->width()<<endl;
      //get pixmap with selected items drawn(pixmap for cursor)
      QPixmap pix = getPixmapForSelection(dif.x(),dif.y(),
                                        m_myWidget->getScrollArea()->size());
      QByteArray itemData;
//        QString s = m_plibElementPrwview->getPreviewItemType().c_str();
      cout<<"Drag selection "<<endl;

      //set data for dragging
      QMimeData *mimeData = new QMimeData;
      mimeData->setData("application/x-dnditemdata", itemData);
      mimeData->setProperty("s_type","selection");
      mimeData->setProperty("s_offset",e->pos());
//         setSelectionVisible(false);
      repaint();

      QDrag *drag = new QDrag(this);
      drag->setMimeData(mimeData);
      drag->setPixmap(pix);
//      cout<<"pos:"<<m_myWidget->getScrollArea()->pos().x()<<" "<<m_myWidget->getScrollArea()->pos().y()<<endl;
      //set pos for cursor pixmap
      drag->setHotSpot(parentPos); // m_myWidget->getScrollArea()->pos());//event->pos() - child->pos());
      //start drag
      drag->exec();
   }
   repaint();
}

void PcBoard::mousePressEvent(QMouseEvent* e)
{
   bool bNextPointMaybeAdded = true;
   switch(m_currMode)
   {
      case MODE_PLATE:
      case MODE_PLATE_RECT:
      case MODE_MULTIPLATE:
      {
         processSimpleGraphicalItems(e);
      }
      break;
      case MODE_LINE:
      case MODE_VC_CON:
      case MODE_CONNECTED_RULER:
      {
         handleConnectorCreation(e,e->button() == Qt::LeftButton);
         if(m_currMode == MODE_CONNECTED_RULER)
            bNextPointMaybeAdded = m_ConnectedRuler->isConnecting();
         repaint();
      }
      break;
      case MODE_PACK_RECT:
      case MODE_PACK_ROUND:
      {
         m_packWrapper->setPt1(e->pos(),m_scaleFactor);
      }
      break;
      case MODE_SIMPLE_RULER:
      {
         if(m_SimpleRuler->isConnecting())
            m_SimpleRuler->addPoint();
      }
      break;
      case MODE_CURSOR:
      {
         //we do not need to handle this event
         //this is context menu event
         if(e->buttons() & Qt::RightButton)
            return;
         m_packWrapper->setPt1(e->pos(),m_scaleFactor);
         initDrag(e);
      }
      break;
      case MODE_NODE_PRE_MOVE:
      {
         if(e->button() == Qt::LeftButton)
         {
            m_currMode = MODE_NODE_MOVE;
            m_packWrapper->setTypeByMode(MODE_NODE_MOVE);
         }
      }
      break;
      case MODE_TEXT:
      {
         handleProcessText(e->pos().x(),e->pos().y());
      }
      default:
       break;
   }
   if(!bNextPointMaybeAdded)
   {
      setMode(MODE_CURSOR);
   }
}

void PcBoard::handleConnectorCreation(QMouseEvent* ,bool bMsLeftButton)
{
   if(bMsLeftButton)
   { 
      m_tempConnector->addPoint();
      m_ConnectedRuler->addPoint();
   }
   else
   {
      CONNECTOR_TYPE type = m_tempConnector->getConnectorType();
      SmartPtr<GraphicalItem> p = m_tempConnector->commit();
      if(p.get() != nullptr)
      {
//         if(type == CONNECTOR_TYPE::BOARD)
            addItemToSchematic(p);
//         else
//            addGraphicalItemToLevels(p);
      }
      //reset properties after commit
      if(m_currMode == MODE_DRAW::MODE_LINE || m_currMode == MODE_DRAW::MODE_VC_CON)
          m_tempConnector->setProperties(type,m_wLine,
                                         m_currMode == MODE_DRAW::MODE_VC_CON ?
                                             BOARD_LEVEL_ID::LEVEL_VC : m_currentLevel);
   }
}

void PcBoard::mouseReleaseEvent(QMouseEvent *e )
{
   QWidget::mouseReleaseEvent(e);
   if(m_currMode == MODE_NODE_MOVE)
   {
      PointF p(-1,-1);
      m_myWidget->getUndoStack()->push(new MoveNodeCommand(m_nodeChangeItem,this));
      m_nodeChangeItem.finishMovement(p);
      m_currMode = MODE_CURSOR;
      m_packWrapper->setTypeByMode(MODE_CURSOR);
   }   
   else if(m_currMode == MODE_PACK_RECT || m_currMode == MODE_PACK_ROUND)
   {
      SmartPtr<GraphicalItem> p = m_packWrapper->commit(m_currentLevel,filledPack);
      /*temporarily
      float x =  static_cast<float>((m_RectAndRndCoord[0].x() +  m_RectAndRndCoord[1].x()))/2/m_scaleFactor/PIXELS_PER_MM;
      float y =  static_cast<float>((m_RectAndRndCoord[0].y() +  m_RectAndRndCoord[1].y()))/2/m_scaleFactor/PIXELS_PER_MM;
      float w = static_cast<float>((m_RectAndRndCoord[1].x() -  m_RectAndRndCoord[0].x()))/m_scaleFactor/PIXELS_PER_MM;
      float h = static_cast<float>((m_RectAndRndCoord[1].y() -  m_RectAndRndCoord[0].y()))/m_scaleFactor/PIXELS_PER_MM;
      
      if(m_currMode == MODE_PACK_RECT )
      {
         p = ItemsFactory::createRectPackage(x,y,w,h,m_currentLevel,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,ID_NONE,
                                             m_scaleFactor,iNoZoom,filledPack);
      }
      else
      {
         p = ItemsFactory::createRoundPackage(x,y,w,h,m_currentLevel,ITEMS_ORIENTATION::O_VERTICAL_TOP,ID_NONE,
                                              m_scaleFactor,iNoZoom);
      }
      */
      addItemToSchematic(p);
//      m_RectAndRndCoord[0] = m_RectAndRndCoord[1] = QPoint(-1,-1);
      m_currMode = MODE_CURSOR;
      m_packWrapper->setTypeByMode(MODE_CURSOR);
      m_packWrapper->clear();
      repaint();
   }
   else if(m_currMode == MODE_SIMPLE_RULER)
   {
      m_SimpleRuler->rollback();
      m_currMode = MODE_CURSOR;
      repaint();
   }
   else if(m_currMode == MODE_CURSOR && m_mapOfSelItems.empty())
   {
      //release rectangle coordinates and select items
      QRect rect;
      if(m_packWrapper->getArea(rect,1))
      {
         boardLayers.checkSelectForArea(rect.topLeft(),
                  rect.bottomRight(),
                  m_mapOfSelItems);
      }
/*trmporarily

      boardLayers.checkSelectForArea(m_RectAndRndCoord[0]/m_scaleFactor,
              m_RectAndRndCoord[1]/m_scaleFactor,
              m_mapOfSelItems);
      m_RectAndRndCoord[0] = m_RectAndRndCoord[1] = QPoint(-1,-1);
*/
      m_packWrapper->clear();
      repaint();
   }
}


void PcBoard::mouseMoveEvent(QMouseEvent *e)
{
//   cout<<"Moving mouse x="<<e->x()<<"y="<<e->y()<<endl;
    PointF ptf(static_cast<float>(e->pos().x())/PIXELS_PER_MM/m_scaleFactor,
              static_cast<float>(e->pos().y())/PIXELS_PER_MM/m_scaleFactor);

   if(e->buttons() & Qt::LeftButton)
   {
      m_packWrapper->setPt2(ptf);
      if(m_currMode == MODE_PACK_RECT
              || m_currMode == MODE_PACK_ROUND
              || m_currMode == MODE_CURSOR)
      {
//         m_RectAndRndCoord[1] = e->pos();
         if(!m_mapOfSelItems.empty())
         {
            for(auto& it:m_mapOfSelItems)
               it.second->setSelected(false);
            m_mapOfSelItems.clear();
         }
      }
      else if(m_currMode == MODE_SIMPLE_RULER)
      {
          m_RulerCoord[1] = e->pos();
      }
//      cout<<"Moving mouse x="<<e->x()<<"y="<<e->y()<<endl;
      repaint();
   }
   if(m_myWidget)
      m_myWidget->setBoardCursorCoord(e->pos());
   
   if(m_currMode == MODE_CURSOR || m_currMode == MODE_NODE_PRE_MOVE)
   {
      //check if we are above the line and prepare node for the move
      for(auto iter:m_mapOfSelItems)     
      {
         int index = 0;
         QPoint pt((e->pos()).x()/m_scaleFactor,(e->pos()).y()/m_scaleFactor);
         if(iter.second->isAboveNode(pt,index))
         {
            setCursor(Qt::CrossCursor);
            m_currMode = MODE_NODE_PRE_MOVE;
            m_nodeChangeItem.setItem(iter.second,index,ptf,ptf);
            return;
         }
      }
      m_currMode = MODE_CURSOR;
      unsetCursor();
      m_nodeChangeItem.clear();
      return;      
   }
   else if(m_currMode == MODE_NODE_MOVE)
   {
      m_nodeChangeItem.moveNode(ptf);
      repaint();
   }
   else if(m_currMode == MODE_VC_CON
           || m_currMode == MODE_LINE
           || m_currMode == MODE_CONNECTED_RULER
           || m_currMode == MODE_SIMPLE_RULER)
   {
      m_tempConnector->setPoint(ptf);

      if(m_ConnectedRuler->isConnecting())
         m_ConnectedRuler->setPoint(ptf);

      m_SimpleRuler->setPoint(ptf);


      repaint();
   }
}


void PcBoard::setMode(int mode)
{
   cout<<"setting mode"<<endl;
   m_packWrapper->setTypeByMode(static_cast<MODE_DRAW>(mode));
   m_currMode = mode;
   if ( mode != MODE_TEXT)
   {
      m_text.clear();
      text_x_pos = text_y_pos = -1;
      if(m_pText)
      {
         QString text = m_pText->toPlainText();
         auto size = static_cast<float>(m_pText->fontInfo().pointSize());

         auto x = (static_cast<float>(m_pText->x()) + (size * text.length())/2)/PIXELS_PER_MM/m_scaleFactor;
         auto y = (static_cast<float>(m_pText->y()) + size)/PIXELS_PER_MM/m_scaleFactor;

//         auto x = static_cast<float>(m_pText->x() + size/4)/PIXELS_PER_MM/m_scaleFactor;
//         auto y = static_cast<float>(m_pText->y() + size + size/3)/PIXELS_PER_MM/m_scaleFactor;



         auto p = ItemsFactory::createText(
                     x,
                     y,
                     text,m_pText->fontInfo().pointSize()/m_scaleFactor,
                     ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                     m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
         if(p.get())
            addItemToSchematic(p);
         m_pText->clear();
         m_pText->setVisible(false);
      }
   }
   if (mode != MODE_PLATE && mode != MODE_PLATE_RECT)
   {
      unsetCursor();
   }
   if(m_tempConnector.get())
   {
      m_tempConnector->rollback();
      repaint();
   }
}


void PcBoard::zoomIn()
{
    m_scaleFactor <<= 1;
    if(m_pTemplateImage)
    {
       QImage im =  m_pTemplateImage->scaled(m_pTemplateImage->width() * 2 ,m_pTemplateImage->height() * 2);
       delete m_pTemplateImage;
       m_pTemplateImage = new QImage(im);
    }
#ifdef _SCROLL    
    resizeBoard();
#endif
    repaint();
}

void PcBoard::checkResetMode(MODE_DRAW mode)
{
   if(mode != m_currMode)
      setMode(MODE_CURSOR);
}

void PcBoard::zoomOut()
{
    m_scaleFactor >>= 1;
    if(m_pTemplateImage)
    {
       QImage im =  m_pTemplateImage->scaled(m_pTemplateImage->width()/ 2 ,m_pTemplateImage->height() / 2);
       delete m_pTemplateImage;
       m_pTemplateImage = new QImage(im);
    }
#ifdef _SCROLL    
    resizeBoard();
#endif
    repaint();
}


void PcBoard::zoomItems(int )
{
}


void PcBoard::processSimpleGraphicalItems(QMouseEvent* e)
{
   if(e->button() == Qt::LeftButton)
   {
      switch(m_currMode)
      {
         case MODE_PLATE:
            processPlate(e->x(),e->y());
            break;
         case MODE_PLATE_RECT:
            processPlateRect(e->x(),e->y());
            break;
         case MODE_MULTIPLATE:
            processMultiPlate(e->x(),e->y());
            break;
            
      }
   }
   else
   {
      m_currMode = MODE_CURSOR;
      unsetCursor();
      m_mapOfDragData.clear();         
   }
}



void PcBoard::preProcessPlate(float dOut, float dInt)
{
   checkResetMode(MODE_PLATE);
   m_mapOfDragData[D_EX_DEF] = dOut;
   m_mapOfDragData[D_IN_DEF] = dInt;   
   int w1 = static_cast<int>(dOut * PIXELS_PER_MM * m_scaleFactor);
   int w2 = static_cast<int>(dInt * PIXELS_PER_MM * m_scaleFactor);
   QPixmap buffer(w1,w1);
   buffer.fill(Qt::transparent);
   QPainter painter(&buffer);
   QColor c(LevelsWrapper::getColorForMove());
   DrawWrapper::drawPlate(painter,c,w1/2,w1/2,w1,w2);
   QCursor cursor(buffer);
   setCursor(cursor);
   m_currMode = MODE_PLATE;
}

void PcBoard::processPlate(int x,int y){
   auto i1 = m_mapOfDragData.find(D_EX_DEF);
   auto i2 = m_mapOfDragData.find(D_IN_DEF);
   if(i1 != m_mapOfDragData.end() && i2 != m_mapOfDragData.end())
   {
      float w1 = i1->second;
      float w2 = i2->second;
      char buf[20];
      sprintf(buf,"%d",m_scaleFactor);
      cout<<"r_p "<<buf<<endl;         
      auto p = ItemsFactory::createRoundPlate(static_cast<float>(x)/m_scaleFactor/PIXELS_PER_MM,
                                                     static_cast<float>(y)/m_scaleFactor/PIXELS_PER_MM,
                                                     w1,w2,
                                                     m_currentLevel,ID_NONE,m_scaleFactor,
                                                        iNoZoom);
//      addGraphicalItemToLevel(m_currentLevel,p);
      addItemToSchematic(p);
      repaint();
   }
}

void PcBoard::processMultiPlate(int x, int y)
{
   auto i1 = m_mapOfDragData.find(D_EX_DEF);
   auto i2 = m_mapOfDragData.find(D_IN_DEF);
   if(i1 != m_mapOfDragData.end() && i2 != m_mapOfDragData.end())
   {
      float w1 = i1->second;
      float w2 = i2->second;
      vector<BOARD_LEVEL_ID> levels;
      levels.push_back(m_currentLevel);
      if(m_currentLevel == LEVEL_A)
         levels.push_back(LEVEL_F);
      else if(m_currentLevel == LEVEL_F)
         levels.push_back(LEVEL_A);
      else
         levels.push_back(LEVEL_F);;
         
      auto p = ItemsFactory::createRoundMultiPlate(static_cast<float>(x)/m_scaleFactor/PIXELS_PER_MM,
                                                     static_cast<float>(y)/m_scaleFactor/PIXELS_PER_MM,
                                                     w1,w2,
                                                     levels,ID_NONE,m_scaleFactor,
                                                     iNoZoom);
      EditMultiPlate dlg(static_cast<MultiplateGraphicalItem*>(p.get()),
                         m_schemData.m_width/PIXELS_PER_MM,
                         m_schemData.m_height/PIXELS_PER_MM);
      if(dlg.exec() == QDialog::Accepted)
      {
         addItemToSchematic(p);
         repaint();
      }
/*
      QString s = "";
      addGraphicalItemToLevels(s,p);
*/
   }   
}


void PcBoard::processLine(float w)
{
   checkResetMode(MODE_LINE);
   m_wLine = w;
   m_currMode = MODE_LINE;
   m_packWrapper->setTypeByMode(MODE_LINE);
   m_tempConnector->setProperties(CONNECTOR_TYPE::BOARD,m_wLine,m_currentLevel);
}

void PcBoard::processVcCon()
{
   checkResetMode(MODE_VC_CON);
   m_currMode = MODE_VC_CON;
   m_packWrapper->setTypeByMode(MODE_VC_CON);
   m_wLine = fDefVirtConWidth;
   m_tempConnector->setProperties(CONNECTOR_TYPE::SCHEMATIC,m_wLine,BOARD_LEVEL_ID::LEVEL_VC);
}

void PcBoard::rotate()
{
   cout<<"rotate"<<endl;
   m_myWidget->getUndoStack()->push(new RotateItemCommand(m_mapOfSelItems.begin()->second,this));
}

void PcBoard::keyPressEvent(QKeyEvent *e)
{
   if(e->key() == Qt::Key_Escape)
   {
      if(m_tempConnector.get())
      {
         m_tempConnector->rollback();
         repaint();
      }
      return;
   }
   if(m_mapOfSelItems.empty()) //nothing to do
      return;
   if(e->key() == Qt::Key_Left ||
      e->key() == Qt::Key_Right ||
      e->key() == Qt::Key_Up ||
      e->key() == Qt::Key_Down ||
      e->key() == Qt::Key_Delete ||
      e->key() == Qt::Key_R  )
   {
      vector<SmartPtr<GraphicalItem>> toDelVec;
      PointF dif(0.0f,0.0f);
      switch (e->key())
      {
         case Qt::Key_Delete:
            for(auto& item:m_mapOfSelItems)
               toDelVec.push_back(item.second);
            m_myWidget->getUndoStack()->push(new DeleteItemCommand(std::move(toDelVec),this));
            break;
         case Qt::Key_Left:
            dif.setX(-minGraularity);
            m_myWidget->getUndoStack()->push(new MoveItemCommand(m_mapOfSelItems,dif,this));
            break;
         case Qt::Key_Right:
            dif.setX(minGraularity);
            m_myWidget->getUndoStack()->push(new MoveItemCommand(m_mapOfSelItems,dif,this));
            break;
         case Qt::Key_Up:
            dif.setY(-minGraularity);
            m_myWidget->getUndoStack()->push(new MoveItemCommand(m_mapOfSelItems,dif,this));
            break;
         case Qt::Key_Down:
            dif.setY(minGraularity);
            m_myWidget->getUndoStack()->push(new MoveItemCommand(m_mapOfSelItems,dif,this));
            break;
         case Qt::Key_R:
            rotate();
            break;
         default:
            break;
      }
      repaint();
   }
   else
      QWidget::keyPressEvent(e);
}

void PcBoard::setBoardCurrentLevel(BOARD_LEVEL_ID level)
{
   m_currentLevel = level;
}

void PcBoard::contextMenuEvent(QContextMenuEvent *e)
{
    QWidget::contextMenuEvent(e);
    if (m_currMode == MODE_NODE_PRE_MOVE)
    {
       QMenu menu(this);
//       newNodeAct->setPos(e->pos());
       menu.addAction(newNodeAct);
//       delNodeAct->setPos(e->pos());
       menu.addAction(delNodeAct);
       menu.exec(e->globalPos());
    }
    else if(m_currMode == MODE_CURSOR)
    {
       QMenu menu(this);
       bool bSelNumMoreThanOne = m_mapOfSelItems.size() > 1;
       if(!m_mapOfSelItems.empty())
       {
          if(bSelNumMoreThanOne)
          {
            // more than 1 item can be grouped             
             menu.addAction(groupAct);
          }
          else
          {
             auto item = m_mapOfSelItems.begin();
             GraphicalItem *pGr = item->second.get();
             GenericGraphicalItemsContainer *p = nullptr;
             // only container can be ungrouped
             if((p = dynamic_cast<GenericGraphicalItemsContainer*>(pGr)) != nullptr)
             {
                menu.addAction(ungroupAct );
             }
             menu.addAction(rotateAct);
          }
          menu.addAction(cutAct);
          menu.addAction(copyAct);
       }
       //paste for items in copy map
       if(!m_mapofItemsToCopy.empty())
          menu.addAction(pasteAct);
       //for more than one item properties
       //should not be displayed
       if(!bSelNumMoreThanOne)
          menu.addAction(propAct);
       menu.exec(e->globalPos());
    }
}

void PcBoard::createActions()
{
    newNodeAct = new ActionEx(tr(ADD_NODE_MENU_STR), this);
    newNodeAct->setStatusTip(tr(ADD_NODE_MENU_STR));
    connect(newNodeAct, SIGNAL(triggered()), this, SLOT(newNodeAction()));
    
    delNodeAct = new ActionEx(tr(DELETE_NODE_MENU_STR), this);
    delNodeAct->setStatusTip(tr(DELETE_NODE_TIP_STR));
    connect(delNodeAct, SIGNAL(triggered()), this, SLOT(delNodeAction()));
    
    groupAct = new QAction(tr(GROUP_MENU_STR), this);
    groupAct->setStatusTip(tr(GROUP_TIP_STR));
    connect(groupAct, SIGNAL(triggered()), this, SLOT(groupAction()));
    
    ungroupAct = new QAction(tr(UNGROUP_MENU_STR), this);
    ungroupAct->setStatusTip(tr(UNGROUP_TIP_STR));
    connect(ungroupAct, SIGNAL(triggered()), this, SLOT(ungroupAction()));
    
    
    addToLib = new QAction(tr(ADDTOLIB_MENU_STR), this);
    addToLib->setStatusTip(tr(ADDTOLIB_TIP_STR));
    connect(addToLib, SIGNAL(triggered()), this, SLOT(addToLibAction()));
    
    propAct = new QAction(tr(PROP_DEF_STR), this);
    propAct->setStatusTip(tr(PROP_DEF_TIP_STR));
    connect(propAct, SIGNAL(triggered()), this, SLOT(editPropsAction()));

    copyAct = new QAction(tr("Copy"), this);
    copyAct->setStatusTip(tr(COPY_DEF_TIP_STR));
    connect(copyAct, &QAction::triggered, this, &PcBoard::processCopy);

    cutAct = new QAction(tr("Cut"), this);
    cutAct->setStatusTip(tr(CUT_DEF_TIP_STR));
    connect(cutAct, &QAction::triggered, this, &PcBoard::processCut);

    pasteAct = new QAction(tr("Paste"), this);
    pasteAct->setStatusTip(tr(PASTE_DEF_TIP_STR));
    connect(pasteAct, &QAction::triggered, this, &PcBoard::processPaste);

    rotateAct = new QAction(tr("Rotate"), this);
    rotateAct->setStatusTip(tr(ROTATE_STRING));
    connect(rotateAct, &QAction::triggered, this, &PcBoard::rotate);

}

void PcBoard::newNodeAction()
{

   //set the  node below the cursor
//   pMoveItem->prepareToChange(newNodeAct->getPos());
   //now add the node accroding to the current node below the cursor
//   pMoveItem->addNode(0);
//   newNodeAct->setPos(QPoint(-1,-1));
//   m_nodeChangeItem.addNode();
   m_myWidget->getUndoStack()->push(new AddNodeCommand(m_nodeChangeItem,this));
   repaint();

}

void PcBoard::delNodeAction()
{

   //set the  node below the cursor
//   pMoveItem->prepareToChange(delNodeAct->getPos());
   //now delete the node below the cursor
//   pMoveItem->removeNode(0);
   m_myWidget->getUndoStack()->push(new DeleteNodeCommand(m_nodeChangeItem,this));
//   m_nodeChangeItem.removeNode();
//   newNodeAct->setPos(QPoint(-1,-1));
   repaint();

}

//recreate container from existing items and existing empty container
void PcBoard::regroupCommand(SmartPtr<GraphicalItem> &pCont, GraphicalItemsMap& map)
{
    vector<SmartPtr<GraphicalItem> > tmp;
    for(auto& item:map)
    {
       deleteGraphicalItem(item.second);
       tmp.push_back(item.second);
    }
    GenericGraphicalItemsContainer *p = static_cast<GenericGraphicalItemsContainer*>(pCont.get());
    p->setChildren(std::move(tmp));
    p->groupItems();
    addGraphicalItemToLevels(pCont);
}
/*
//recreate container from existing items and existing empty container
void PcBoard::regroupCommand(QString& name,GenericGraphicalItemsContainer *pCont, GraphicalItemsMap& map)
{
   vector<GraphicalItem*> tmp;
   for(auto& item:map)
   {
       deleteGraphicalItem(item.first);
      tmp.push_back(item.second);
   }
   pCont->setChildren(std::move(tmp));
   pCont->groupItems();
   addGraphicalItemToLevels(name,pCont);
}
*/
//groups items in container,adds the container to current level,
//removes grouped items from levels
bool PcBoard::groupCommand(GraphicalItemsMap& map,SmartPtr<GraphicalItem>& ptr)
{
    float minx,miny,maxx,maxy;
    auto firstItem = map.begin();
    if(firstItem == map.end())
       return false;
    //first thing we need to determine physical center of selection
    auto areaOfFirst = firstItem->second->getArea();
    minx = firstItem->second->x() - areaOfFirst.x()/2;
    miny = firstItem->second->y() - areaOfFirst.y()/2;
    maxx = firstItem->second->x() + areaOfFirst.x()/2;
    maxy = firstItem->second->y() + areaOfFirst.y()/2;
    //additionally we will find all layers
    //to which the items belong
    set<BOARD_LEVEL_ID> layers_ids;
    vector<SmartPtr<GraphicalItem> > list_of_items;
    list_of_items.reserve(map.size());
    auto connectorLevel = BOARD_LEVEL_ID::LEVEL_NONE;
    auto packageLevel = BOARD_LEVEL_ID::LEVEL_NONE;
    for(auto& item:map)
    {
       if(dynamic_cast<ConnectorGraphicalItem*>(item.second.get()) != nullptr)
       {
          //connectors should not be grouped
          continue;
       }
       if(dynamic_cast<GenericGraphicalItemsContainer*>(item.second.get()) != nullptr)
       {
          //do not allow hierarchical grouping
          QMessageBox msgBox;
          msgBox.setIcon(QMessageBox::QMessageBox::Critical);
          msgBox.setInformativeText(QString::fromUtf8(FAILED_HIERARCHICAL_GROUPING));
          msgBox.setStandardButtons(QMessageBox::Ok);
          msgBox.exec();
          return false;
       }
       if(connectorLevel == BOARD_LEVEL_ID::LEVEL_NONE &&
          LevelsWrapper::isActiveLevelExternal(item.second->getLevel()))
           connectorLevel = item.second->getLevel();

       if(packageLevel == BOARD_LEVEL_ID::LEVEL_NONE &&
          LevelsWrapper::isPackageLevel(item.second->getLevel()))
           packageLevel = item.second->getLevel();

       auto cx = item.second->x();
       auto cy = item.second->y();
       auto area = item.second->getArea();
       if(cx + area.x()/2 > maxx)
          maxx = cx + area.x()/2;
       if(cx - area.x()/2 < minx)
          minx = cx - area.x()/2;
       if(cy + area.y()/2 > maxy)
          maxy = cy + area.y()/2;
       if(cy - area.y()/2 < miny)
          miny = cy - area.y()/2;
       item.second->getLevels(layers_ids);
       list_of_items.push_back(item.second);
       cout<<"grouping "<<item.first.toStdString()<<" "<<item.second->getID()<<endl;
    }
    float centerx = minx + (maxx -minx)/2;
    float centery = miny + (maxy -miny)/2;
    GraphicalItem *parent = nullptr;
    //we need to find an item that will be the parent
    //this should be the item which contains the center point of selection
    for(auto& item:map)
    {
       if(item.second->isInArea(static_cast<int>(centerx * PIXELS_PER_MM),
                                 static_cast<int>(centery * PIXELS_PER_MM)))
       {

          if(abs(item.second->x() - centerx) < 1 && abs(item.second->y() - centery) < 1)
          {
             parent = item.second.get();
             centerx = item.second->x();
             centery = item.second->y();
             break;
          }
       }
       cout<<"grouping 1"<<item.first.toStdString()<<" "<<item.second->getID()<<endl;
    }
    //container layer should be an external layer,in case there is no external layer
    //or package layer for this set of items set container layer to current layer
    auto containerLevel = connectorLevel;
    if(containerLevel == BOARD_LEVEL_ID::LEVEL_NONE)
    {
       if(packageLevel == BOARD_LEVEL_ID::LEVEL_NONE)
          containerLevel = m_currentLevel;
       else
          containerLevel = LevelsWrapper::getConnectorLevelForPackage(packageLevel);
    }

    //now we create container ,add all items and add it to layers
    SmartPtr<GraphicalItem> pContainer = ItemsFactory::createContainer(centerx,
                                                              centery,
                                                              containerLevel,
                                                              list_of_items,ID_NONE);
    static_cast<GenericGraphicalItemsContainer*>(pContainer.get())->groupItems();
    //this operation should be done before removing items

    addGraphicalItemToLevels(pContainer);

    //
    //now we need to remove all items from levels
    for(auto& item:map)
    {
       cout<<"delete from level "<<item.first.toStdString()<<" "<<item.second->getID()<<endl;
       deleteGraphicalItem(item.second);
    }
    ptr = pContainer;
    return true;
}

void PcBoard::groupAction()
{
    //move the content to the new map
//    shared_ptr<GraphicalItemsMap> map = make_shared<GraphicalItemsMap>(std::move(m_mapOfSelItems));
    m_myWidget->getUndoStack()->push(new GroupCommand(std::move(m_mapOfSelItems),this));
    m_mapOfSelItems.clear();
//    groupCommand(map);
/*
   float minx,miny,maxx,maxy;
   auto firstItem = m_mapOfSelItems.begin();
   if(firstItem == m_mapOfSelItems.end())
      return;
   //first thing we need to determine physical center of selection
   minx = firstItem->second->x();
   miny = firstItem->second->y();
   maxx = firstItem->second->x();
   maxy = firstItem->second->y();
   //additionally we will find all layers
   //to which the items belong
   set<BOARD_LEVEL_ID> layers_ids;
   vector<GraphicalItem*> list_of_items;
   list_of_items.reserve(m_mapOfSelItems.size());
   for(auto item:m_mapOfSelItems)
   {
      if(item.second->x() > maxx)
         maxx = item.second->x();
      if(item.second->x() < minx)
         minx = item.second->x();
      if(item.second->y() > maxy)
         maxy = item.second->y();
      if(item.second->y() < miny)
         miny = item.second->y();
      item.second->getLevels(layers_ids);
      list_of_items.push_back(item.second);
   }
   float centerx = minx + (maxx -minx)/2;
   float centery = miny + (maxy -miny)/2;
   GraphicalItem *parent = nullptr;
   //we need to find an item that will be the parent
   //this should be the item which contains the center point of selection
   for(auto item:m_mapOfSelItems)
   {
      if(item.second->isPointIn(static_cast<int>(centerx * PIXELS_PER_MM),
                                static_cast<int>(centery * PIXELS_PER_MM)))
      {
         parent = item.second;
         break;
      }
   }
   
   //now we create container ,add all items and add it to layers
   GraphicalItem *pContainer = ItemsFactory::createContainer(centerx,
                                                             centery,
                                                             parent? m_currentLevel : parent->getLevel(),
                                                             list_of_items,
                                                             m_scaleFactor,1);
   static_cast<GenericGraphicalItemsContainer*>(pContainer)->groupItems();
   //this operation should be done before removing items

   QString name_new;
   addGraphicalItemToLevels(name_new,pContainer);

//   addItemToSchematic(pContainer);
   //
   //now we need to remove all items from levels
   for(auto item:m_mapOfSelItems)
   {
      deleteGraphicalItem(item.first);
   }
   m_mapOfSelItems.clear();
*/
}

void PcBoard::ungroupCommand(SmartPtr<GraphicalItem>& pContainer)
{
    //the order of operations in this function is very important
    //basic tests to see if we have container
    if(!pContainer.get() || pContainer->getID() == ID_NONE)
       return ;
    GenericGraphicalItemsContainer *pC = dynamic_cast<GenericGraphicalItemsContainer*>(pContainer.get());
    if(pC)
    {
       QString containerName = QString::number(pContainer->getID());
       //set parent to NULL
       pC->unGroupItems();
       //acquire children
       vector<SmartPtr<GraphicalItem> >* pChildren = pC->getChildren();
       vector<SmartPtr<GraphicalItem> > children = *pChildren;
       //add them to levels
       for(VecLayerInrementer i = 0; i < pChildren->size(); ++i)
       {
          addGraphicalItemToLevels(children[i]);
       }
       //try to remove container from selected items
       //after this operation the item cannot be used
       m_mapOfSelItems.erase(containerName);
       //clean the container class
       pC->deleteAllChildren();
       //delete it
       deleteGraphicalItem(pContainer);
    }
}
/*
//ungroups container
//keyAndValue - name and pointer to the container that should be ungrouped.
shared_ptr<GraphicalItemsMap> PcBoard::ungroupCommand(shared_ptr<GraphicalItemsMap::value_type> keyAndValue,
                                                        shared_ptr<GraphicalItemsMap> names)
{
    //the order of operations in this function is very important
    //basic tests to see if we have container
    if(!keyAndValue)
    {
       return nullptr;
    }
    shared_ptr<GraphicalItemsMap> map = make_shared<GraphicalItemsMap>();
    //check if we have valid container in keyAndValue
    GenericGraphicalItemsContainer *pC = dynamic_cast<GenericGraphicalItemsContainer*>(keyAndValue->second);
    QString containerName = keyAndValue->first;
    //both values should not be empty
    if(pC && containerName.length() > 0)
    {
       //set parent to NULL
       pC->unGroupItems();
       //acquire children
       vector<GraphicalItem*>* pChildren = pC->getChildren();
       //naive test to check if the names are valid
       //add them to levels
       for(VecLayerInrementer i = 0; i < pChildren->size(); ++i)
       {
          char buf[32];
          sprintf(buf,"%d",(*pChildren)[i]->getID());
          QString name;
          if(names &&
                  static_cast<int>((*pChildren)[i]->getID()) != -1)
          {
             auto child = names->find(buf);
             if(child != names->end())
                name = child->first;
          }
          //The string will have the name generated in case it is empty
          addGraphicalItemToLevels((*pChildren)[i]);
             //add to map to return
          (*map)[name] = (*pChildren)[i];
       }
       //try to remove container from selected items
       //after this operation the item cannot be used
       m_mapOfSelItems.erase(containerName);
       //delete it
       deleteGraphicalItem(containerName);
       //clean the container class
       pC->deleteAllChildren();
    }
    return map;
}
*/
void PcBoard::ungroupAction()
{
   auto item = m_mapOfSelItems.begin();
   if(item != m_mapOfSelItems.end())
   {
      m_myWidget->getUndoStack()->push(new UngroupComand(this,item->second));
      //remove container from selected items
      //after this operation the item cannot be used
      m_mapOfSelItems.clear();
   }
/*
   //the order of operations in this function is very important
   //basic test to see if we have container
   auto item = m_mapOfSelItems.begin();
   auto pGr = item->second;
   GenericGraphicalItemsContainer *p = nullptr;
   if((p = dynamic_cast<GenericGraphicalItemsContainer*>(pGr)))
   {
      //set parent to NULL
      p->unGroupItems();
      set<BOARD_LEVEL_ID> set_lvls;
      //acquire levels
      p->getLevels(set_lvls);
      //acquire children
      vector<GraphicalItem*>* pChildren = p->getChildren();      
      //add them to levels
      for(auto item_ch:*pChildren)
      {

         QString name;
         addGraphicalItemToLevels(name,item_ch);
//         QString name = addItemToSchematic(item_ch);
         m_mapOfSelItems[name] = item_ch;
      }
      //first copy the name
      QString name_container = item->first;
      //remove container from selected items
      //after this operation the item cannot be used
      m_mapOfSelItems.erase(item->first);
      //delete it 
      deleteGraphicalItem(name_container);
      repaint();
   }
*/
}

void PcBoard::addToLibAction()
{
  //add dialog here
  AddNewElToLibDlg dlg(this);
  if(dlg.exec() == QDialog::Accepted)
  {
     vector<QString> v;
     QString name;
     dlg.getResult(v,name);
     if(v.size() > 0)
     {
        if(m_myWidget)
        {
           auto item = m_mapOfSelItems.begin();
           if(item != m_mapOfSelItems.end())
           {
              m_myWidget->addToLib(v,name,item->second);
           }
        }
     }
  }
}


void PcBoard::setBoardVisibleLevel(BOARD_LEVEL_ID level, bool bSet)
{
   boardLayers.getLayer(level)->setVisible(bSet);
   repaint();
}

void PcBoard::setBoardVisibleLevels(vector<BoardLevel>& levels)
{
   boardLayers.initLayers(levels);
}

void PcBoard::deleteGraphicalItem(SmartPtr<GraphicalItem>& p)
{
   // delete it in case it was copied into clipboard
//   char name[32];
//   sprintf(name,"%d",p->getID());
   QString name = QString::number(p->getID());
   p->setSelected(false);
   m_mapofItemsToCopy.erase(name);
   m_mapOfSelItems.erase(name);
   boardLayers.deleteGraphicalItemFromLayers(p);
}



//adds graphical items to the layers that should be set for this item
//by GraphicalItem::setLevel(level) or in GraphicalItem::constructor
//Containers may contain GraphicalItem(s) that belong to different layers,
//this is handled by this method
void PcBoard::addGraphicalItemToLevels(SmartPtr<GraphicalItem>& p)
{
   boardLayers.addGraphicalItemToLayers(p);
}

void PcBoard::addVirtualConnector(SmartPtr<GraphicalItem>& p, map<GraphicalItem *, PointF>& connectorPoints)
{
   for(auto& item:connectorPoints)
   {
      if(item.first->getID() == -1)
      {
         item.first->setID(IDsGenerator::instance()->getNewID());
      }
   }
   p->setLevel(LEVEL_VC);
   addGraphicalItemToLevels(p);
}

void PcBoard::openNewSchematic(SchemData& data,vector< QString >& lNames, vector< SmartPtr<GraphicalItem> >& lItems)
{
   m_mapOfSelItems.clear();
   m_mapOfDragData.clear();
   boardLayers.removeAll();

   for(VecLayerInrementer i = 0; i < lItems.size();++i)
   {
      addGraphicalItemToLevels(lItems[i]);
   }
   m_schemData = data;
   resizeBoard();
   repaint();
}


void PcBoard::processContainer(const QPoint& position,const QString& type,
                                 const QString& name,int n,SmartPtr<GraphicalItem>& pItem)
{
//   char buf[64];
   SmartPtr<GraphicalItem> p;
   BOARD_LEVEL_ID descLevel = LEVEL_NONE;
//   cout<<type.toStdString()<<" OK"<<endl;
   PointF pos(position.x()/PIXELS_PER_MM,position.y()/PIXELS_PER_MM);
   if(type.startsWith(TYPE_S_CHIP_DIP))
   {
      descLevel = LevelsWrapper::geLevelForDip(m_currentLevel);
      int n = type.mid(3).toInt();
      p = ItemsFactory::createStdDip(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_VERTICAL_TOP,n,
                                     m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(TYPE_S_CHIP_SO))
   {
      descLevel = LevelsWrapper::geLevelForSO(m_currentLevel);
      int n = type.mid(2).toInt();
      p = ItemsFactory::createStdSO(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_VERTICAL_TOP,n,
                                    m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(TYPE_S_0805) ||
           type.startsWith(TYPE_S_1206) ||
           type.startsWith(TYPE_S_1210) ||
           type.startsWith(TYPE_S_2510) ||
           type.startsWith(TYPE_S_2512))
   {
      descLevel = LevelsWrapper::geLevelForSO(m_currentLevel);
      p = ItemsFactory::createSmdType(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                                      getSmdPackForStr(type),m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(TYPE_S_R_DIP))
   {
      descLevel = LevelsWrapper::geLevelForDip(m_currentLevel);
      p = ItemsFactory::createResistor(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                                      getLResistorFromStrType(type),m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(TYPE_S_CHIP_MSOP) ||
           type.startsWith(TYPE_S_CHIP_TSOP))
   {
       descLevel = LevelsWrapper::geLevelForTSOP(m_currentLevel);
       int n = type.mid(strlen(TYPE_S_CHIP_TSOP)).toInt();
       p = ItemsFactory::createStdTSOP(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_VERTICAL_TOP,n,
                                       m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(TYPE_S_EL_C))
   {
      descLevel = LevelsWrapper::geLevelForDip(m_currentLevel);
      p = ItemsFactory::createElCapacitor(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                                      getLForELCapFromStrType(type),m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(TYPE_S_C))
   {
      descLevel = LevelsWrapper::geLevelForDip(m_currentLevel);
      p = ItemsFactory::createCap(pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,
                                      getLForCapFromStrType(type),m_currentLevel,ID_NONE,m_scaleFactor,iNoZoom);
   }
   else if(type.startsWith(GENERIC_TYPE_DEF) && pItem.get())
   {
      cout<<"duplication"<<endl;
      p  = ItemsFactory::duplicateItem(pItem.get(),pos.x()/m_scaleFactor,pos.y()/m_scaleFactor,false);
      //setting current zoom
      p->setID(ID_NONE);
   }
   if(p.get())
   {
/*
      QString name;
      addGraphicalItemToLevels(name,p);
*/
      addItemToSchematic(p);
      repaint();
   }

}

void PcBoard::print()
{
   auto tempScaleRatio = m_scaleFactor;
   m_scaleFactor = 1;
   SetPrintPropsDlg dlg(this);
   dlg.exec();
   m_scaleFactor = tempScaleRatio;
   return;
/*
   m_currMode = MODE_PRINT;
   QImage target(QSize(this->width(), this->height()), QImage::Format_ARGB32);
   QPainter painter(&target);
   render(&painter);
   QPrinter printer(QPrinter::HighResolution);
   printer.setOutputFileName("print.ps");
   QPrintDialog printDialog(&printer);
   if (printDialog.exec() == QDialog::Accepted) 
   {   
      QPainter p1(&printer);
      p1.scale(printer.resolution()/254, printer.resolution()/254);      
      p1.drawImage(QPoint(0,0),target.mirrored(true,false));
      p1.end();
   }
*/
/*   
   QPrinter printer(QPrinter::HighResolution);
   printer.setOutputFileName("print.ps");
   QPainter painter;
   painter.begin(&printer);
   painter.scale(printer.resolution()/254, printer.resolution()/254);   
   render(&painter);
*/
   m_currMode = MODE_CURSOR;
}

void PcBoard::processPlateRect(int x,int y)
{
   auto i1 = m_mapOfDragData.find(WIDTH_DEF);
   auto i2 = m_mapOfDragData.find(HEIGHT_DEF);
   if(i1 != m_mapOfDragData.end() && i2 != m_mapOfDragData.end())
   {
      char buf[20];
       sprintf(buf,"%d",m_scaleFactor);
      cout<<"r_p "<<buf<<endl;
      SmartPtr<GraphicalItem> p = ItemsFactory::createRect(static_cast<float>(x)/m_scaleFactor/PIXELS_PER_MM,
                                                  static_cast<float>(y)/m_scaleFactor/PIXELS_PER_MM,
                                                     i1->second,i2->second,
                                                     m_currentLevel,ITEMS_ORIENTATION::O_VERTICAL_TOP
                                                     ,ID_NONE,m_scaleFactor,iNoZoom);
/*
      addGraphicalItemToLevel(m_currentLevel,p);
*/
      addItemToSchematic(p);
      repaint();
   }
}


void PcBoard::preProcessPlateRect(float w, float h)
{
   checkResetMode(MODE_PLATE_RECT);
   m_mapOfDragData[WIDTH_DEF] = w;
   m_mapOfDragData[HEIGHT_DEF] = h;      
   int w1 = static_cast<int>(w * PIXELS_PER_MM * m_scaleFactor);
   int h1 = static_cast<int>(h * PIXELS_PER_MM * m_scaleFactor);
   QPixmap buffer(w1,h1);
   buffer.fill(Qt::transparent);
   QPainter painter(&buffer);
   QColor c(LevelsWrapper::getColorForMove());
   DrawWrapper::drawRectPlate(painter,c,w1/2,h1/2,w1,h1);
   QCursor cursor(buffer);
   setCursor(cursor);
   cout<<"plate_rect"<<endl;
   m_currMode = MODE_PLATE_RECT;   
}

void PcBoard::processPackRect(float wl,bool filled)
{
   checkResetMode(MODE_PACK_RECT);
   m_wLine = wl;
   m_currMode = MODE_PACK_RECT;
   m_packWrapper->setTypeByMode(MODE_PACK_RECT);
   filledPack = filled;
}

void PcBoard::processPackRound(float wl)
{
   checkResetMode(MODE_PACK_ROUND);
   m_wLine = wl;
   m_currMode = MODE_PACK_ROUND;
   m_packWrapper->setTypeByMode(MODE_PACK_ROUND);
}

void PcBoard::editPropsAction()
{
   if(m_mapOfSelItems.size() == 1)
   {
      auto item = m_mapOfSelItems.begin();
      SmartPtr<GraphicalItem> p = item->second;
      BOARD_LEVEL_ID iLevel = LEVEL_NONE;
      GraphicalItemPropsDlg dlg(this,p);
      if(dlg.exec() == QDialog::Accepted)
      {
         shared_ptr<PointF> pos;
         shared_ptr<GeomCommonProps> props;
         ContainerType containerType;
         ITEMS_ORIENTATION o;
         if(dlg.getResult(iLevel,pos,props,containerType,o) && pos.get())
             m_myWidget->getUndoStack()->push(new SetPropsCommand(p,pos,props,this));
         if(p->getLevel() != iLevel)
         {
            //we cannot pass const QString& to moveBetweenLayers
            QString s = item->first;
//            moveBetweenLayers(iLevel,s,p);
            moveItemBetweenLayers(iLevel,s,p,containerType);
         }
         repaint();
      }
   }
   else if(m_mapOfSelItems.empty())
   {
      PcBoardPropsDlg dlg(this,m_scaleFactor);
      if(dlg.exec() == QDialog::Accepted)
      {
         int w,h;
         dlg.getResult(w,h);
         m_myWidget->getUndoStack()->push(new SetBoardProps(this,w,h));
         setSize(w,h);
      }      
   }
   
}


void PcBoard::moveItemBetweenLayers(BOARD_LEVEL_ID newLevel,QString& s,
                                    SmartPtr<GraphicalItem>& p,ContainerType type)
{
    if(p->getLevel() != newLevel)
    {
        //produce undo/redo command which will call movind
        m_myWidget->getUndoStack()->push(new MoveBetweenLayersCommand(p,s,this,newLevel,type));
    }
}


void PcBoard::moveBetweenLayers(BOARD_LEVEL_ID newLevel,QString& s,
                                SmartPtr<GraphicalItem> p,ContainerType type)
{
   if(p->getLevel() != newLevel)
   {
      deleteGraphicalItem(p);
      GenericGraphicalItemsContainer *pCont =
              dynamic_cast<GenericGraphicalItemsContainer*>(p.get());
      if(pCont != nullptr)
      {
         auto children = pCont->getChildren();
         auto it = std::find_if(children->begin(),children->end(),
                      [](SmartPtr<GraphicalItem>& p)
                        {
                           return LevelsWrapper::isPackageLevel(p->getLevel());
                        }
                      );
         bool isPackaged = (it != children->end());
         //For packaged items we consider only relocating from one side
         //of pcb to another and do not take into consideration newLevel
         for(auto& item:*children)
         {
            if(LevelsWrapper::isPackageLevel(item->getLevel()))
               item->setLevel(LevelsWrapper::getOppositePackageLevel(item->getLevel()));
            else if(isPackaged && LevelsWrapper::isActiveLevelExternal(p->getLevel()))
               item->setLevel(LevelsWrapper::getOppositeLevel(p->getLevel()));
            else
               item->setLevel(newLevel);
         }
      }
      /*
      //special handling for RelocatableContainer
      if(type == ContainerType::RelocatableDipType ||
         type == ContainerType::RelocatableSoType )
      {
         auto packageLevel = BOARD_LEVEL_ID::LEVEL_NONE;
         auto oldPackageLevel = BOARD_LEVEL_ID::LEVEL_NONE;
         if(type == ContainerType::RelocatableDipType)
         {
            packageLevel = LevelsWrapper::geLevelForDip(newLevel);
            oldPackageLevel = LevelsWrapper::geLevelForDip(p->getLevel());
         }
         else
         {
            packageLevel = LevelsWrapper::geLevelForSO(newLevel);
            oldPackageLevel = LevelsWrapper::geLevelForSO(p->getLevel());
         }
         //set it for children
         GenericGraphicalItemsContainer *pCont = 
                 static_cast<GenericGraphicalItemsContainer*>(p.get());
         auto children = pCont->getChildren();
         for(auto& item:*children )
         {
            if(item->getLevel() == oldPackageLevel)
            {
               item->setLevel(packageLevel);
               if(dynamic_cast<TextGraphicalItem*>(item.get()) != nullptr)
               {
                  //text needs reflection
                  item->setX(-item->x());
               }
            }
            else if(item->getLevel() == p->getLevel())
                item->setLevel(newLevel);
         }

      }
      */
      p->setLevel(newLevel);
      addGraphicalItemToLevels(p);
   }
}


void PcBoard::resizeBoard()
{
    setMaximumWidth(m_schemData.m_width * m_scaleFactor);
    setMinimumWidth(m_schemData.m_width * m_scaleFactor);
    setMaximumHeight(m_schemData.m_height * m_scaleFactor);
    setMinimumHeight(m_schemData.m_height * m_scaleFactor);
    resize(m_schemData.m_width * m_scaleFactor,
           m_schemData.m_height * m_scaleFactor);

}


void PcBoard::setSize(int w, int h)
{
   if(m_schemData.m_width != w || m_schemData.m_height != h)
   {
      m_schemData.m_width = w ;
      m_schemData.m_height = h;
      resizeBoard();
      QWidget *p = static_cast<QWidget*>(parent());
      if(p)
         p->repaint();
      if(m_myWidget)
         m_myWidget->updateBoardSize(w,h);
   }
}

void PcBoard::loadImage(QString& fileName)
{
   QImage im(fileName);
   float dpmX = im.dotsPerMeterX();
   float dpmY = im.dotsPerMeterY();
   float ratiox = PIXELS_PER_MM * 1000.0f/dpmX;
   float ratioy = PIXELS_PER_MM * 1000.0f/dpmY;
   QImage im1 = im.scaled(ratiox * im.width() * m_scaleFactor,ratioy * im.height() * m_scaleFactor);
   m_pTemplateImage = new QImage(im1);
/*   
   int width = m_pTemplateImage->width();
   int height = m_pTemplateImage->height();
   float ratio = (float)PIXELS_PER_MM/(float)PIXELS_PER_MM_PRTPREVIW;
   width *= ratio;
   height *= ratio;
   m_pTemplateImage->scaled(width,height);
*/
   repaint();
}


void PcBoard::unloadImage()
{
   delete m_pTemplateImage;
   m_pTemplateImage = nullptr;
   repaint();
}

void PcBoard ::processRuler()
{
   m_currMode = MODE_SIMPLE_RULER;
   m_SimpleRuler->setProperties(CONNECTOR_TYPE::SIMPLE_RULER,fDefVirtConWidth,LEVEL_NONE);
}

void PcBoard ::processConnectedRuler(bool checked)
{
   if(checked)
      checkResetMode(MODE_SIMPLE_RULER);

   if(checked)
   {
      m_ConnectedRuler->setProperties(CONNECTOR_TYPE::CONNECTED_RULER,fDefVirtConWidth,LEVEL_NONE);
      m_currMode = MODE_CONNECTED_RULER;
   }
   else
   {
      m_ConnectedRuler->rollback();
      m_currMode = MODE_CURSOR;
      repaint();
   }

}

void PcBoard::processCut()
{
    if(m_currMode != MODE_CURSOR)
       return;

    GraphicalItemsMap temp = std::move(m_mapOfSelItems);

    for(auto& item:temp)
       deleteGraphicalItem(item.second);

    m_mapofItemsToCopy = std::move(temp);

    repaint();
}

void PcBoard ::processCopy()
{
    if(m_currMode != MODE_CURSOR)
       return;

    m_mapofItemsToCopy = m_mapOfSelItems;

}

void PcBoard::processPaste()
{
   if(m_currMode != MODE_CURSOR)
      return;

   QPoint pt = mapFromGlobal(QCursor::pos());
   cout<<"Paste "<<std::to_string(pt.x())<<endl;
   //we need to find upmost item. Ths will be the
   //the starting point of insertion
   int startY = m_schemData.m_height * m_scaleFactor;
   int startX = 0;
   for(auto& item:m_mapofItemsToCopy)
   {
      if(item.second->y() * PIXELS_PER_MM < startY/m_scaleFactor)
      {
         startY = static_cast<int>(item.second->y() * PIXELS_PER_MM * m_scaleFactor);
         startX = static_cast<int>(item.second->x() * PIXELS_PER_MM * m_scaleFactor);
      }
   }
   for(auto item:m_mapofItemsToCopy)
   {
      int x = pt.x() + (static_cast<int>(item.second->x() * PIXELS_PER_MM * m_scaleFactor) - startX);
      int y = pt.y() + (static_cast<int>(item.second->y() * PIXELS_PER_MM * m_scaleFactor) - startY);
      auto p = ItemsFactory::duplicateItem(item.second.get(),
                                                     static_cast<float>(x)/PIXELS_PER_MM/m_scaleFactor,
                                                     static_cast<float>(y)/PIXELS_PER_MM/m_scaleFactor,false);
      //pasted items should not be selected
      p->setSelected(false);
/*
      QString name = ""; //empty because we want the new name to be generated by addGraphicalItemToLevels
      addGraphicalItemToLevels(name,p);
*/
      addItemToSchematic(p);
   }
   repaint();
}

void PcBoard::clearCopyPasteMap()
{
    m_mapofItemsToCopy.clear();
}

void PcBoard::preProcessMultiPlate(float dOut,float dIn)
{
   checkResetMode(MODE_MULTIPLATE);
   m_mapOfDragData[D_EX_DEF] = dOut;
   m_mapOfDragData[D_IN_DEF] = dIn;   
   int w1 = static_cast<int>(dOut * PIXELS_PER_MM * m_scaleFactor);
   int w2 = static_cast<int>(dIn * PIXELS_PER_MM * m_scaleFactor);
   QPixmap buffer(w1 * 2,w1 * 2);
   buffer.fill(Qt::transparent);
   QPainter painter(&buffer);
   QColor c(LevelsWrapper::getColorForMove());
   QPoint hot = DrawWrapper::drawMultiPlate(painter,c,w1,w1,w1,w2);
   //adjust the cursor pos to center of upper plate
   QCursor cursor(buffer,w1 - hot.x() ,w1 - hot.y() );
   setCursor(cursor);
   m_currMode = MODE_MULTIPLATE;   
}

void PcBoard::processSelectAll()
{
    if(m_currMode != MODE_CURSOR)
       return;
    clearCopyPasteMap();
    boardLayers.copyItems(m_mapOfSelItems);
    for(auto& item:m_mapOfSelItems)
        item.second->setSelected(true);
    repaint();
}

QString PcBoard::addItemToSchematic(SmartPtr<GraphicalItem>& p)
{

    QString name = "";
    m_myWidget->getUndoStack()->push(new AddItemCommand(p,name,this));
    return name;
}

void PcBoard::deleteItemFromSchematic(const QString& name)
{
   SmartPtr<GraphicalItem> p = boardLayers.findItem(name);
   if(p.get() != nullptr)
      m_myWidget->getUndoStack()->push(new DeleteItemCommand(p,this));
}

void PcBoard::setLineWidth(const QString& text)
{
   m_wLine = text.toFloat();
}

void PcBoard::setItemPosAndGeometry(GraphicalItem *p, PointF& pos, GeomCommonProps *props)
{
    p->setX(pos.x());
    p->setY(pos.y());
    if(props)
    {
       p->setGeometry(*props);
    }
    repaint();
}

void PcBoard ::constructPcbLayout()
{
   /*
   if(!m_myWidget->getUndoStack()->isClean())
   {
      QMessageBox msgBox(QMessageBox::Critical,SAVE_SCHEMA_TITLE,
                  "It is strongly recommended to save schema before proceeding!",
                   QMessageBox::Ok);
      msgBox.exec();
      return;
   }
*/

   using ConMap = map<BOARD_LEVEL_ID,vector<SmartPtr<GraphicalItem>>>;
   using ConPair = pair<BOARD_LEVEL_ID,vector<SmartPtr<GraphicalItem>>>;

   auto layerVC = boardLayers.getLayer(BOARD_LEVEL_ID::LEVEL_VC);

   set<BOARD_LEVEL_ID> layersSet;
   auto vcCons = layerVC->getConnectItemsInLevel();
   ConMap levelToSimpleConnectors;
   ConMap dualConnectors;
   //check all layers used in this pcb and put them into map
   for(auto& vcCon:*vcCons)
   {
      auto conItems = static_cast<ConnectorGraphicalItem*>(vcCon.second.get())->getConnectedItems();
      set<BOARD_LEVEL_ID> conLayersSet;
      for(auto& conItem:*conItems)
         conItem.second->getLevels(conLayersSet);

      layersSet.insert(conLayersSet.begin(),conLayersSet.end());

      auto id = *conLayersSet.begin();
      pushIntoMappedVector(levelToSimpleConnectors,
                           id,vcCon.second);

/*
      auto lAddCon = [&vcCon](ConMap& m,BOARD_LEVEL_ID id)
      {
         auto levelItems = m.find(id);
         if(levelItems == m.end())
            m[id] = {vcCon.second};
         else
            levelItems->second.push_back(vcCon.second) ;

      };

      lAddCon(levelToSimpleConnectors,*conLayersSet.begin());
*/
   }
   //check if essential layers exist
   if(levelToSimpleConnectors.find(BOARD_LEVEL_ID::LEVEL_A) == levelToSimpleConnectors.end()
        && levelToSimpleConnectors.find(BOARD_LEVEL_ID::LEVEL_F) == levelToSimpleConnectors.end())
   {
      QMessageBox msgBox(QMessageBox::Critical,"Error",
                         "Layer A or Layer F should have \n schematic connectors",
                         QMessageBox::Ok);
      msgBox.exec();
      return;
   }

   //edit properties before construction
   float fConWidth = fSizeOfMatrixCell;
   vector<BOARD_LEVEL_ID> layerIds;
   AutoConstructProps dlg(std::move(layersSet));
   if(dlg.exec() == QDialog::Accepted)
      dlg.getResult(fConWidth,layerIds);
   else
      return;

   //add checked by user additional layers if any
   for(auto id:layerIds)
      if(levelToSimpleConnectors.find(id) == levelToSimpleConnectors.end())
      {
         levelToSimpleConnectors[id] = {};
      }

   //construct board
   PcbAutoConstructor pcbAutoConstrutor(m_myWidget,fConWidth,layerIds);
   //these variables intended to store results
   ConstructedLayer bestProc;
   map<ITEM_ID,vector<SmartPtr<GraphicalItem>>> multiplates;
   pcbAutoConstrutor.constructPcbLayout(boardLayers,levelToSimpleConnectors,
                                                    dualConnectors,
                                                    m_schemData.m_width * m_scaleFactor * minGraularity,
                                                    m_schemData.m_height * m_scaleFactor * minGraularity,
                                                    bestProc,multiplates);

   if(bestProc.size() > 0)
   {
      m_myWidget->getUndoStack()->push(new AutoCommandItem(std::move(bestProc),                                                           std::move(multiplates),this));
      repaint();
   }
   return;

}

/*
void PcBoard ::constructPcbLayout()
{

    VecLayerInrementer sizeX = static_cast<VecLayerInrementer>(m_schemData.m_width *
                                                               m_scaleFactor/fSizeOfMatrixCell);
    VecLayerInrementer sizeY = static_cast<VecLayerInrementer>(m_schemData.m_height *
                                                               m_scaleFactor/fSizeOfMatrixCell);
    PcbLayoutVec m(sizeY,vector<ITEM_ID>(sizeX,ID_NONE));
    vector<UniCoord> px((sizeX + sizeY) * 2,ID_NONE);
    vector<UniCoord> py((sizeX + sizeY) * 2,ID_NONE);
    map<QString,vector<PointF> > mapOfLinesData;
    if(layerA != nullptr && layerVC != nullptr)
    {
       AUTO_SORT index = TOP_LEFT;
       map<AUTO_SORT,map<QString,SmartPtr<GraphicalItem>>> mapOfConnectors;
       while(true)
       {
          //virtual connectors
          auto vcConnectors = layerVC->getConnectItemsInLevel();
          vector<SmartPtr<GraphicalItem> > vcCons;
          set<QString> toDeleteSet;
          using ConIterator = GraphicalItemsMap::const_iterator;

          for(ConIterator iter = vcConnectors->begin(); iter != vcConnectors->end();++iter)
          {
//             cout<<iter->second->getID()<<endl;
             vcCons.push_back(iter->second);
          }
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
//          for(VecIndex i = 0; i < vcCons.size();++i)
//          {
//             cout<<vcCons[i]->getID()<<endl;
//          }

          auto connectors = layerA->getConnectItemsInLevel();
          auto non_connector_items = layerA->getItemsInLevel();
          int len = 0;
          map<QString,SmartPtr<GraphicalItem>> vcIdToConPoints;
          // TEST!!!! Some computation here
//
//          set<ITEM_ID> ids;
//          auto start = std::chrono::system_clock::now();

//          fillPcb(m,non_connector_items,ids,0,0);
//          fillPcb(m,connectors,ids,0,0);
//          auto end = std::chrono::system_clock::now();
//          std::chrono::duration<double> elapsed_seconds = end-start;
//          std::time_t end_time = std::chrono::system_clock::to_time_t(end);

//          std::cout << "finished computation at " << std::ctime(&end_time)
//                       << "elapsed time: " << elapsed_seconds.count() << "s"
//                       << std::endl;

//          AutoDlg dlg(&m,m_width * 4,m_height * 4);
//          dlg.exec();
//          return;
//
          //END of TEST!!!
          for(VecIndex i = 0; i < vcCons.size();++i)
          {

              ConnectorGraphicalItem* connector = static_cast<ConnectorGraphicalItem*>(vcCons[i].get());
              cout<<"ID of connector:"<<connector->getID()<<endl;
              //now we get items that this connector connects to
              set<ITEM_ID> ids;
              auto pMap = connector->getConnectedItems();
              for(auto& item:*pMap)
                 ids.insert(item.second->getID());
//              for(VecIdsInc ln = 0; ln < sizeY; ++ln)
//              {
//                 for(VecIdsInc cl = 0; cl < sizeX; ++cl)
//                 {
                    fillPcb(m,non_connector_items,ids,0,0);
                    fillPcb(m,connectors,ids,0,0);
                    fillPcb(m,&vcIdToConPoints,ids,0,0);
//                 }
//              }
              PointF startPoint = connector->getPoints()->at(0);
              PointF endPoint = connector->getPoints()->at(connector->getPoints()->size() - 1);
              int ax = static_cast<int>(startPoint.x()/fSizeOfMatrixCell);
              int ay = static_cast<int>(startPoint.y()/fSizeOfMatrixCell);
              int bx = static_cast<int>(endPoint.x()/fSizeOfMatrixCell);
              int by = static_cast<int>(endPoint.y()/fSizeOfMatrixCell);
              len = 0;
              if(lee(m,static_cast<int>(sizeX),
                     static_cast<int>(sizeY),
                     static_cast<VecLayerInrementer>(ax),
                     static_cast<VecLayerInrementer>(ay),
                     static_cast<VecLayerInrementer>(bx),
                     static_cast<VecLayerInrementer>(by),len,px,py,ids))
              {
                 vector<PointF> points;
                 createPointsForLineFromLeePath(points,px,py,len);

                 auto p = SmartPtr<GraphicalItem>::make_smartptr<ConnectorGraphicalItem>(std::move(points),
                                                   fWidthOfConAuto,
                                                   LEVEL_A,
                                                   CONNECTOR_TYPE::SCHEMATIC,
                                                   IDsGenerator::instance()->getNewID());
                 QString str_id = QString::number(connector->getID());
                 vcIdToConPoints[str_id] = p;
              }
              AutoDlg dlg(&m,m_schemData.m_width * m_scaleFactor * 4,
                          m_schemData.m_height * m_scaleFactor * 4,ax,ay,bx,by);
              dlg.exec();
          }
          auto connectorsFromVcs = [&layerVC](map<QString,SmartPtr<GraphicalItem>>& map,PcBoard *ptrThis)
          {
             for(auto& vcToPoints:map)
             {
                vcToPoints.second->setVisible(true);
                ptrThis->addGraphicalItemToLevels(vcToPoints.second);
                layerVC->deleteGraphicalItemFromLevel(vcToPoints.first);
             }
          };
          if(vcIdToConPoints.size() == vcCons.size())
          {
             //we are done.create schematic connectors from points
             connectorsFromVcs(vcIdToConPoints,this);
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
                   connectorsFromVcs(bestProcessing->second,this);
                }
                break;
             }
             else
             {
                //store results
                mapOfConnectors[old_index] = std::move(vcIdToConPoints);
                //in case the map was copied(not moved) clear it
                vcIdToConPoints.clear();
                for(VecIdsInc ln = 0; ln < sizeY; ++ln)
                   for(VecIdsInc cl = 0; cl < sizeX; ++cl)
                      m[ln][cl] = ID_NONE;

             }
          }
       }
       repaint();
   }
//   }
}
*/
void PcBoard::createPointsForLineFromLeePath(vector<PointF>& points,
                                               vector<UniCoord>& px,
                                               vector<UniCoord>& py,
                                               int length)
{
    unsigned len = static_cast<unsigned>(length);
    points.push_back(PointF(px[0].pos * fSizeOfMatrixCell + fSizeOfMatrixCell/2,
                     py[0].pos * fSizeOfMatrixCell + fSizeOfMatrixCell/2));
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
          points.push_back(PointF(px[index - 1].pos * fSizeOfMatrixCell + fSizeOfMatrixCell/2,
                           py[index - 1].pos * fSizeOfMatrixCell + fSizeOfMatrixCell/2));

          px[index - 1] = -1;
          py[index - 1] = -1;
       }

    }
    points.push_back(PointF(px[len - 1].pos * fSizeOfMatrixCell + fSizeOfMatrixCell/2,
                     py[len - 1].pos * fSizeOfMatrixCell + fSizeOfMatrixCell/2));
}



bool PcBoard ::fillPcb(PcbLayoutVec& m,GraphicalItemsMap* items,set<ITEM_ID>& ids,
                         VecIdsInc ln,VecIdsInc cl)
{
    using PcbVectorIndex = std::vector<ITEM_ID>::size_type;
    auto layerId = BOARD_LEVEL_ID::LEVEL_A;
    for(auto& item:*items)
    {
       PointF top;
//       top.setX(cl * fSizeOfMatrixCell);
//       top.setY(ln * fSizeOfMatrixCell);
       PointF bottom;
//       bottom.setX((cl + 1) * fSizeOfMatrixCell);
//       bottom.setY((ln + 1) * fSizeOfMatrixCell);
       auto& p = item.second;
       GenericGraphicalItemsContainer *pc = nullptr;
       auto lambda = [&top,&bottom,ids](GraphicalItem *item,PcbLayoutVec& pcb)
       {
          PointF pt = item->getArea();
          float x = item->abs_x();
          float y = item->abs_y();
          PointF topCoord{x - pt.x()/2,y - pt.y()/2};
          PointF bottCoord{x + pt.x()/2,y + pt.y()/2};
          QPoint topCell = getCellCoordForPoint(topCoord,fSizeOfMatrixCell);
          QPoint bottomCell = getCellCoordForPoint(bottCoord,fSizeOfMatrixCell);
          auto topIndexX = static_cast<PcbVectorIndex>(topCell.x());
          auto topIndexY = static_cast<PcbVectorIndex>(topCell.y());
          auto botIndexX = static_cast<PcbVectorIndex>(bottomCell.x());
          auto botIndexY = static_cast<PcbVectorIndex>(bottomCell.y());
          for(PcbVectorIndex i = topIndexX; i < botIndexX; ++i)
          {
             for(PcbVectorIndex j = topIndexY; j < botIndexY; ++j)
             {
                top.setX(i * fSizeOfMatrixCell);
                top.setY(j * fSizeOfMatrixCell);
                bottom.setX(top.x() + fSizeOfMatrixCell);
                bottom.setY(top.y() + fSizeOfMatrixCell);
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
/*
       ITEM_ID id = p.get()->isRectOccupied(top,bottom);
       if(id != ID_NONE)
       {
          //in case the cell is already occupied see if it has connectors connected
          if(m[ln][cl] < WALL)
          {
             auto iter = ids.find(WALL - m[ln][cl]);
             if(iter == ids.end())
             {
                iter = ids.find(id);
                if(iter != ids.end())
                {
                   m[ln][cl] = WALL - *iter;
                   return true;
                }
             }
             else
                return true;
         }
         else
         {
            m[ln][cl] = WALL - id;
            return false;
         }
      }
      else if(!(m[ln][cl] < WALL))
          m[ln][cl] = ID_NONE;
*/
   }
   return false;
}

void PcBoard::processText(int fontSize,bool vcLevel)
{
  checkResetMode(MODE_TEXT);
  setMode(MODE_TEXT);
  textFontSize = fontSize;
}

void PcBoard::updateBoardView(const QPoint& dif)
{
   for(auto& item:m_mapOfSelItems)
   {
      PointF pt(dif.x()/PIXELS_PER_MM/m_scaleFactor,
                dif.y()/PIXELS_PER_MM/m_scaleFactor);
      item.second->setDragCoord(pt);
   }
   repaint();
}
void PcBoard::handleProcessText(int x, int y)
{
   if(!m_pText)
   {
      m_pText = new TextEditField(this);
      m_pText->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
      m_pText->setBackgroundVisible(true);
   }
   if(!m_pText->isVisible())
   {
      if(m_pText->fontInfo().pointSize() != textFontSize)
      {
         QScreen* pScreen = QGuiApplication::primaryScreen();
         auto ratioX = pScreen->physicalDotsPerInchX()/72;
         auto ratioY = pScreen->physicalDotsPerInchY()/72;
         QFont font("Arial",textFontSize * m_scaleFactor);
         m_pText->setFont(font);
         QPalette p = m_pText->palette();
         p.setColor(QPalette::Text,LevelsWrapper::getColorForLevel(m_currentLevel));
         m_pText->setPalette(p);
         //7 to 14 characters
         auto width = castFloatToInt(textFontSize * static_cast<float>(ratioX) * 7 * m_scaleFactor);
         auto height = castFloatToInt(textFontSize * static_cast<float>(ratioY) * 2 * m_scaleFactor);
         m_pText->setGeometry(x,y,width,height);
      }
      m_pText->show();
   }
}

void PcBoard::processExcellon(QString& name)
{

   QString url = QDir::currentPath() + "/" +
           QFileInfo(name).baseName() + ".drl";

   QString fileName =
           QFileDialog::getSaveFileName(this,
                                              tr("Select folder"),
                                              url,
                                                ".drl");
   if(!fileName.isEmpty())
   {
      fstream out_excellon(fileName.toStdString(),std::fstream::in | std::fstream::out | std::fstream::app);
      ExcellonGenerator gen(&boardLayers);
      gen.generateFile(out_excellon);
   }

}

void PcBoard::processGerber(QString& name)
{
   set<BOARD_LEVEL_ID> layersSet;
   boardLayers.getNonEmptyLayersIds(layersSet,true);
   if(layersSet.size() > 0)
   {
      QString fileName = name.isEmpty() ? "out" : name;
      EditExportToGerberProps dlg(&boardLayers,
                                  std::move(layersSet),
                                  m_schemData.m_width/PIXELS_PER_MM,
                                  m_schemData.m_height/PIXELS_PER_MM,
                                  name);
      dlg.exec();
   }
   return;
   /*
   cout<<"Export to gerber"<<endl;
   fstream out_gerber("C:\\gerber_out.gbr",std::fstream::in | std::fstream::out | std::fstream::app);



   //show dialog to select layers to be exported into gerber
   //...
   //
   GerberGenerator gerber(3,4,3,4);
   gerber.setboardMaxY(m_schemData.m_height/PIXELS_PER_MM);
   gerber.setMirrowed(false);
   out_gerber<<gerber.getInitGerberString();
   for (auto item:*boardLayers.getLayer(BOARD_LEVEL_ID::LEVEL_A)->getConnectItemsInLevel())
      out_gerber<<item.second->getGerberString(gerber,
                                               BOARD_LEVEL_ID::LEVEL_A)<<endl;
   for (auto item:*boardLayers.getLayer(BOARD_LEVEL_ID::LEVEL_A)->getItemsInLevel())
      out_gerber<<item.second->getGerberString(gerber,
                                               BOARD_LEVEL_ID::LEVEL_A)<<endl;

   out_gerber<<gerber.getFinishGerberString()<<endl;
   out_gerber.close();
   */
}

//#include "PcBoard.moc"

