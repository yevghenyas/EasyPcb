/*
 * Copyright 2017 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
#include <qslider.h>
#include <QSpinBox>
#include <qlcdnumber.h>
#include <qfont.h>
#include "testboard.h"
#include "platedata.h"
#include "ruler.h"
#include "common.h"
#include <QXmlStreamReader>
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

#ifndef MYWIDGET_H
#define MYWIDGET_H

#include <QApplication>
#include <qpushbutton.h>
#include <QTreeView>
#include <QLabel>
#include <QDoubleSpinBox>
#include <QUndoStack>
#include <QCheckBox>
#include <qboxlayout.h>
#include <vector>
#include "filereader.h"
#include <map>
#include "genericgraphicalitemscontainer.h"
#include "pcboard.h"
#include <iostream>
#include "ruler.h"
#include "filewriter.h"
#include <QScrollArea>

using namespace std;

enum class ContextMenuId {Add = 0,Rename,Delete};

class PreviewLabel : public QLabel
{
   GraphicalItem *m_itemInPreview;
   QString m_sItemName;
   int zoom_d{1};
public:
   PreviewLabel():m_itemInPreview(nullptr){}
   virtual QSize sizeHint() const
   {
      return QSize(iPreviewSizeX,iPreviewSizeY); 
   }
   virtual QSize minimumSize() const
   {
      return QSize(iPreviewSizeX,iPreviewSizeY); 
   }
   void previewItem(SmartPtr<GraphicalItem>& item,const QString& name)
   {
      m_itemInPreview = item.get();
      m_sItemName = name;
      GenericGraphicalItemsContainer *pG = nullptr;
      auto zoom = 1;
      zoom_d = 1;
      //all containers should be with (0,0) coordinates after creation
      if((pG = dynamic_cast<GenericGraphicalItemsContainer*>(item.get())))
      {
         auto maxY = static_cast<int>(pG->getMaxY() * PIXELS_PER_MM);
         auto maxX = static_cast<int>(pG->getMaxX() * PIXELS_PER_MM);
         auto maxYPr = iPreviewSizeY/2;
         auto maxXPr = iPreviewSizeX/2;
         auto zoomX = maxX > maxXPr ? maxX/maxXPr * 2 : 1;
         auto zoomY = maxY > maxYPr ? maxY/maxYPr * 2 : 1;
         zoom_d = std::max(zoomX,zoomY);
         //set them temporarily for the center of label
         pG->setX(static_cast<float>(iPreviewSizeX) /PIXELS_PER_MM/2 * zoom_d / zoom);
         pG->setY(static_cast<float>(iPreviewSizeY) /PIXELS_PER_MM/2 * zoom_d / zoom);
      }

      //paint
      QPixmap *p = const_cast<QPixmap*>(pixmap());
      QColor c(Qt::black);
      QPainter pnt(p);
      //calculate
      item->paintItemIntoBuffer(0,0,pnt,c,zoom,zoom_d);
//      item->paintItem(pnt,c,zoom,zoom_d);
      repaint();
      //set them back to (0.0)
      if(pG != nullptr)
      {
         pG->setX(0);
         pG->setY(0);
      }     
   }
   void clearPreview()
   {
      m_itemInPreview = nullptr;
      m_sItemName.clear();
//      clear();
   }
   bool isDragOfElement(const QPoint& pos)
   {
      cout<<"2.1"<<endl;
      if(m_itemInPreview)
      {
         GenericGraphicalItemsContainer *pG = nullptr;

         int zoom = 1;
         //all containers should be with (0,0) coordinates after creation
         //set real coordinates of parent(center of preview panel
         if((pG = dynamic_cast<GenericGraphicalItemsContainer*>(m_itemInPreview)))
         {
            //set them temporarily for the center of label
            pG->setX(static_cast<float>(iPreviewSizeX)/2/PIXELS_PER_MM);
            pG->setY(static_cast<float>(iPreviewSizeY)/2/PIXELS_PER_MM);
         }

         if(m_itemInPreview->isInArea(pos.x(),pos.y()*zoom_d/zoom))
            return true;

         if(pG != nullptr)
         {
            pG->setX(0);
            pG->setY(0);
         }
      }
      return false;
   }
   QString getPreviewItemName(){return m_sItemName;}
   QPixmap* getPixMap(QPoint& ,int )
   {
      if(m_itemInPreview)
      {
         QPixmap *pix = new QPixmap(*pixmap());
         return pix;
      }
      return nullptr;
   }
   void setDefaultPixmap()
   {
      const QPixmap *p = pixmap();
      if(!p)
      {
         QPixmap buffer(size());
         setPixmap(buffer);
      }
      const_cast<QPixmap*>(pixmap())->fill(Qt::transparent);
   }   
};






using TreeData = map <QString,tree_data>;
using TreeItem = TreeData::iterator;

class MyWidget : public QWidget
{
  Q_OBJECT  
  int m_currMode;
  PcBoard *m_board;
  QDoubleSpinBox *m_plExtspin;
  QDoubleSpinBox *m_plIntspin;
  QDoubleSpinBox *m_pLineSpin;
  QDoubleSpinBox *m_pRectWidthSpin;
  QDoubleSpinBox *m_pRectHeightSpin;  
  Ruler *r_v;
  Ruler *r_h;
  LevelsWrapper m_LevelWrapper;
  BoardLevel *m_pActiveLevels,*m_pAvailLevels;
  vector<BoardLevel> m_ActiveLevels;
  vector<BoardLevel> m_AvailLevels;
  FileReader m_reader;
  FileWriter m_writer;
  QTreeView *m_plibView;
  PreviewLabel    *m_plibElementPrwview;
  TreeData m_TreeData;
  map <QString,SchemData> m_schematics;
  QString m_curSchemName;
  QUndoStack *m_pStack;
  QMenu *contextMenu{nullptr};
  bool m_bTreeWasUpdated{false};
  QStandardItem *oldUnderlinedItem{nullptr};
//  QCheckBox *m_chkVC;
#ifdef _SCROLL
  QScrollArea *m_scrollArea;
#endif
  std::unique_ptr<QButtonGroup> m_radioButGroup;
  std::unique_ptr<QButtonGroup> m_checkButGroup;
protected:
  void getTreeSelection(SmartPtr<GraphicalItem>& item,QString& name);
  void handleTreeItemUndelining(QStandardItem *item,bool underline);
public:
  MyWidget( QWidget *parent=nullptr, const char *name=nullptr );
  virtual void mousePressEvent(QMouseEvent*);
  virtual void dragEnterEvent(QDragEnterEvent*);
  virtual void dragMoveEvent(QDragMoveEvent*);
  virtual void dropEvent(QDropEvent*);
  void setBoardCursorCoord(QPoint point);
  virtual ~MyWidget();
  void addToLib(vector<QString>& v ,const QString& name,SmartPtr<GraphicalItem>& p);
  void updateBoardSize(int width,int height);
  QScrollArea* getScrollArea() {return m_scrollArea;}
  QVBoxLayout* createLayersPanel();
  QHBoxLayout* createPlatesAndLinesPanel();
  void openLibFile();
  void createComponentsPreview(QGridLayout *grid);
  void close();
  void checkSuggestSaving();
  void exportGerber();
public slots:
  void processPlate(int index = 0);
  void processCursor();
  void processLines();
  void processZoom_plus();
  void processZoom_minus();
  void processRuler();
  void processConnectedRuler(bool checked);
  void processAutodraw();
  void processContainer(const QPoint& pos,const QString& name);
  void newDocument();
  void open(QLayout *layout = nullptr);
  void treeSelectionChanged(const QItemSelection& o1,const QItemSelection& o2);
  void save(bool bSaveTo = false);
  void saveAs();
  bool saveToFile(QString& name);
  void print();
  void loadImage();
  void unloadImage();
  void processCopy();
  void processCut();
  void processPaste();
  void processSelectAll();
  void setUndoStack(QUndoStack* pStack){m_pStack = pStack;}
  QUndoStack* getUndoStack(){return m_pStack;}
  void processText(int fontSize);
  void processVirtConnector();
  void onCustomTreeContextMenu(const QPoint& p);
  void onNewTreeItem();
  void onRenameTreeItem();
  void onDeleteTreeItem();
};

#endif // MYWIDGET_H
