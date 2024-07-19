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

#include "mywidget.h"


#include <QPainter>
#include "custombutton.h"
#include <QDrag>
#include <QMouseEvent>
#include <QRadioButton>
#include <QButtonGroup>
#include "common.h"
#include <iostream>
#include "statehandler.h"
#include <QComboBox>
#include <QFileDialog>
#include <QMessageBox>
#include "itemsfactory.h"
#include <QSplitter>
#include "drawwrapper.h"
#include <QMimeData>
#include <QScrollBar>
#include <QGroupBox>
#include <QMenu>
#include <QInputDialog>

using namespace std;

using VecLevelInrementer = std::vector<BoardLevel>::size_type;


MyWidget::MyWidget( QWidget *parent, const char *name )
        : QWidget(parent),m_pActiveLevels(nullptr),m_pAvailLevels(nullptr),m_curSchemName(NEW_SCHEM_NAME_DEF)
{
    SchemData data;    
    m_schematics[m_curSchemName] = data;
   

    short lActiveLevels = 0;
    short lAvailLevels = 0;
    m_LevelWrapper.getActiveLevels(&m_pActiveLevels,lActiveLevels);
    m_LevelWrapper.getAvalableLevels(&m_pAvailLevels,lAvailLevels);
    for(int i = 0; i < lActiveLevels; ++i)
    {
       m_ActiveLevels.push_back(m_pActiveLevels[i]);
    }
    for(int i = 0; i < lAvailLevels; ++i)
    {
       m_AvailLevels.push_back(m_pAvailLevels[i]);
    }
    delete [] m_pActiveLevels;
    delete [] m_pAvailLevels;
//    m_board = new PcBoard(this,data.m_width,data.m_height);
    m_board = new PcBoard(data,this);
    m_board->setBoardVisibleLevels(m_AvailLevels);

    QGridLayout *gridLayout = new QGridLayout();
    QGridLayout *gridLayout1 = new QGridLayout();
    setLayout(gridLayout);
    gridLayout->addLayout(gridLayout1, 1, 1 );

    openLibFile();
    createComponentsPreview(gridLayout);
//***********

#ifdef _SCROLL
    m_scrollArea = new QScrollArea(this);
    m_scrollArea->setWidget(m_board);
    QScrollBar *b_v = m_scrollArea->verticalScrollBar();
    QScrollBar *b_h = m_scrollArea->horizontalScrollBar();
    r_v = new Ruler(b_v,data.m_height/PIXELS_PER_MM);
    r_h = new Ruler(b_h,data.m_width/PIXELS_PER_MM,Ruler::TYPE_HORIZONTAL);
#else
    r_v = new Ruler(-1,-1,-1);
    r_h = new Ruler(-1,-1,
                    -1,Ruler::TYPE_HORIZONTAL);
#endif

//************
    gridLayout1->addWidget(r_v, 1, 0);
    gridLayout1->addWidget(r_h, 0, 1);
#ifdef _SCROLL    
    gridLayout1->addWidget(m_scrollArea, 1, 1);
    connect(b_h,SIGNAL(valueChanged(int)), r_h, SLOT(setValue(int)));
    connect(b_v,SIGNAL(valueChanged(int)), r_v, SLOT(setValue(int)));
    connect(b_v,SIGNAL(rangeChanged(int,int)), r_v, SLOT(setRange(int,int)));
    connect(b_h,SIGNAL(rangeChanged(int,int)), r_h, SLOT(setRange(int,int)));
#else    
    gridLayout1->addWidget(m_board, 1, 1);
#endif
    gridLayout->addLayout(createLayersPanel(), 2,1);
    gridLayout1->setColumnStretch(1,10);
    gridLayout->setColumnStretch(1,10);
    setAcceptDrops(true);
}

void MyWidget::createComponentsPreview(QGridLayout *grid)
{
   if(m_plibView)
   {
      m_plibElementPrwview = new PreviewLabel;
      QVBoxLayout *pVPr = new QVBoxLayout;
      pVPr->addWidget(m_plibView);
      pVPr->addWidget(m_plibElementPrwview);
      grid->addLayout(pVPr,1,0);
      m_plibView->show();
   }
}

MyWidget::~MyWidget()
{
   if(m_bTreeWasUpdated)
      m_writer.writeModelToLibrary(static_cast<QStandardItemModel*>(m_plibView->model())
                                   ,m_TreeData);

   for(std::vector<BoardLevel>::size_type i = 0 ; i < m_ActiveLevels.size(); ++i)
   {
      if(m_ActiveLevels[i].pHandler)
         delete m_ActiveLevels[i].pHandler;
   }
   for(std::vector<BoardLevel>::size_type i = 0 ; i < m_AvailLevels.size(); ++i)
   {
      if(m_AvailLevels[i].pHandler)
         delete m_AvailLevels[i].pHandler;
   }
}


void MyWidget::mousePressEvent(QMouseEvent* event)
{
   //we don't need handle left button since this is context menu
   cout<<"1"<<endl;
    QWidget *child = static_cast<QWidget*>(childAt(event->pos()));
    if (!child)
        return;
    
   cout<<"2"<<endl;    
    
    if(child == m_plibElementPrwview && 
       m_plibElementPrwview->isDragOfElement(event->pos() - m_plibElementPrwview->pos()))
    {
       cout<<"3"<<endl;       
       QPoint pos;
       QPixmap *p = m_plibElementPrwview->getPixMap(pos,m_board->getZoomValue());
       QPixmap pixmap(*p);
       delete p;
       
       QByteArray itemData;
       QDataStream dataStream(&itemData, QIODevice::WriteOnly);
       QString name = m_plibElementPrwview->getPreviewItemName();
//       cout<<"Drag chips "<< name.toStdString() <<endl;
       dataStream << &pixmap << QPoint(event->pos() - m_plibElementPrwview->pos());
       

       QMimeData *mimeData = new QMimeData;
       mimeData->setData("application/x-dnditemdata", itemData);
       auto data = m_TreeData.find(name);
       if(data != m_TreeData.end())
       {
          QString str(data->second.getType());
          mimeData->setProperty("s_type",str);
          mimeData->setProperty("s_offset",QPoint(event->pos() - m_plibElementPrwview->pos()));
          mimeData->setProperty("s_name",name);

          QDrag *drag = new QDrag(this);
          drag->setMimeData(mimeData);
          drag->setPixmap(pixmap);
          QPoint pnt(pixmap.size().width(),pixmap.size().height());
          drag->setHotSpot(pnt/2); //event->pos() - child->pos());

          drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction);
//          cout<<"start drag"<<endl;
       }
    }
       

}

void MyWidget::dragEnterEvent(QDragEnterEvent* event)
{
//    cout<<"Drag enter begin"<<endl;
    if (event->mimeData()->hasFormat("application/x-dnditemdata")) 
    {
//        cout<<"Drag enter"<<endl;
        if (event->source() == this ||
            event->source() == m_board)
        {
//            cout<<"Drag enter accepted"<<endl;
            event->setDropAction(event->source() == m_board ? Qt::MoveAction : Qt::CopyAction);
            event->accept();
        }
        else
        {
            event->acceptProposedAction();
        }
    }
    else 
    {
        event->ignore();
    }
}

void MyWidget::handleTreeItemUndelining(QStandardItem* p,bool underline)
{
   auto font = p->data(Qt::FontRole).value<QFont>();
   font.setUnderline(underline);
   p->setData(font,Qt::FontRole);
}

void MyWidget::dragMoveEvent(QDragMoveEvent *event)
{
    auto *child = childAt(event->pos());
    if (event->mimeData()->hasFormat("application/x-dnditemdata") && child) 
    {
        if (event->source() == this ||
            event->source() == m_board) 
        {
           #ifdef _SCROLL
              QPoint pos2 = m_scrollArea->pos() + m_board->pos();
           #else
              QPoint pos2 = m_board->pos();
           #endif

           QPoint offset = event->mimeData()->property("s_offset").toPoint();
           event->setDropAction(event->source() == m_board ? Qt::MoveAction : Qt::CopyAction);
           event->accept();
           if(child->parentWidget() == m_plibView)
           {
              auto index = m_plibView->indexAt(event->pos());
              if(index.isValid())
              {
                 auto model = static_cast<ComponentsTreeModel*>(m_plibView->model());
                 auto item = model->itemFromIndex(index);
                 //only folders should be underlined as a target for drop
                 if(m_TreeData.find(item->text()) == m_TreeData.end())
                 {
                    if(item != oldUnderlinedItem )
                    {
                       if(oldUnderlinedItem != nullptr)
                          handleTreeItemUndelining(oldUnderlinedItem,false);

                       handleTreeItemUndelining(item,true);
                       oldUnderlinedItem = item;
                    }
                 }
              }
              else
              {
                 if(oldUnderlinedItem != nullptr)
                    handleTreeItemUndelining(oldUnderlinedItem,false);
              }
           }
           else
              static_cast<PcBoard*>(m_board)->updateBoardView(event->pos() - pos2);
           cout<<"mywidget x="<<(event->pos() - pos2).x()<<" y"<<(event->pos() - pos2).y()<<endl;
        }
        else 
        {
           event->acceptProposedAction();
        }
    }
    else
    {
        event->ignore();
    }
}


void MyWidget::dropEvent(QDropEvent *event)
{
   
    auto pChild = childAt(event->pos());
    QPoint pos_sc(0,0);
#ifdef _SCROLL
    pos_sc = m_scrollArea->pos();
#endif
    auto bToTreeView = false;
    if(pChild->parentWidget() == m_plibView)
       bToTreeView = true;
    bool bInBoard = event->pos().x() > pos_sc.x() + m_board->pos().x() &&
                    event->pos().y() > pos_sc.y() + m_board->pos().y() &&
                    event->pos().x() < pos_sc.x() + m_board->pos().x() + m_board->size().width() &&
                    event->pos().y() < pos_sc.y() + m_board->pos().y() + m_board->size().height();
    if (event->mimeData()->hasFormat("application/x-dnditemdata") && pChild &&
            ((bToTreeView && m_board->getSelection()->size() == 1) || bInBoard))
    {
        QByteArray itemData = event->mimeData()->data("application/x-dnditemdata");
        QDataStream dataStream(&itemData, QIODevice::ReadOnly);

        QPixmap pixmap;
        QPoint offset;
        QString type;
        QString name;
        dataStream >> pixmap >> offset;

        if (event->source() == this ||
            event->source() == m_board) 
        {
            event->setDropAction(Qt::CopyAction);
            type = event->mimeData()->property("s_type").toString();
            offset = event->mimeData()->property("s_offset").toPoint();
            name = event->mimeData()->property("s_name").toString();
            cout<<"Drop action "<<type.toStdString()<<endl;
            event->accept();
            if(bInBoard)
            {
#ifdef _SCROLL            
               QPoint pos2 = m_scrollArea->pos() + m_board->pos();
#else
               QPoint pos2 = m_board->pos();
#endif
               if(type. compare("selection") == 0)
               {
                  m_board->processSelectionDrop(event->pos() - pos2,offset);
                  m_board->repaint();
               }
               else
               {
                  processContainer(event->pos() - pos2,name);
               }
            }
            else if(bToTreeView)
            {
               if(oldUnderlinedItem != nullptr)
               {
                  //only folders should be underlined as a target for drop

                  if(m_TreeData.find(oldUnderlinedItem->text()) == m_TreeData.end())
                  {
                     auto newElem = m_board->getSelection()->begin();
                     auto text = QInputDialog::getText(this,tr("Name"),tr("Name"),
                                                       QLineEdit::EchoMode::Normal);
                     QStandardItem *p = new QStandardItem(text);
                     oldUnderlinedItem->appendRow(p);
                     m_TreeData[text] = tree_data::createContainerData(newElem->second);
                     oldUnderlinedItem = nullptr;
                     m_bTreeWasUpdated = true;
                  }
               }
            }
            
        } else {
            event->acceptProposedAction();
        }
    } else {
//        cout<<"Drop ignore"<<endl;
        event->ignore();
    }
    if(oldUnderlinedItem != nullptr)
       handleTreeItemUndelining(oldUnderlinedItem,false);
}



void MyWidget::processPlate(int index)
{
   if(index == 0)
      m_board->preProcessPlate(static_cast<float>(m_plExtspin->value()),
                               static_cast<float>(m_plIntspin->value()));
   else if (index == 1)
   {
      m_board->preProcessPlateRect(static_cast<float>(m_pRectWidthSpin->value()),
                                   static_cast<float>(m_pRectHeightSpin->value()));
   }
   else if (index == 2)
   {
      m_board->processPackRect(static_cast<float>(m_pLineSpin->value()));
   }
   else if (index == 3)
   {
      m_board->processPackRound(static_cast<float>(m_pLineSpin->value()));
   }
   else if (index == 4)
   {
      m_board->preProcessMultiPlate(static_cast<float>(MULPLATE_D_EX),
                                    static_cast<float>(MULPLATE_D_IN));
   }
   else if (index == 5)
   {
      m_board->processPackRect(static_cast<float>(m_pLineSpin->value()),true);
   }

}
void MyWidget::processCursor()
{
   m_board->setMode(MODE_CURSOR);
}
void MyWidget::processLines()
{
   //This parameter is not used
   m_board->processLine(static_cast<float>(m_pLineSpin->value()));
}


void MyWidget::processZoom_plus()
{
   if(m_board->getZoomValue() == 16)
      return;
   m_board->zoomIn();
   r_h->setZoom(m_board->getZoomValue());
   r_v->setZoom(m_board->getZoomValue());
   cout<<m_scrollArea->horizontalScrollBar()->minimum()<<endl;
   cout<<m_scrollArea->horizontalScrollBar()->maximum()<<endl;
   cout<<m_scrollArea->horizontalScrollBar()->pageStep()<<endl;
   cout<<m_scrollArea->verticalScrollBar()->minimum()<<endl;
   cout<<m_scrollArea->verticalScrollBar()->maximum()<<endl;
   cout<<m_scrollArea->verticalScrollBar()->pageStep()<<endl;

}
void MyWidget::processZoom_minus()
{
   if(m_board->getZoomValue() == 1)
      return;
   m_board->zoomOut();
   r_h->setZoom(m_board->getZoomValue());
   r_v->setZoom(m_board->getZoomValue());
}

void MyWidget::processContainer(const QPoint& pos,const QString& name)
{
   auto item = m_TreeData.find(name);
   QString type(item->second.getType());
   if(item != m_TreeData.end())
   {
      SmartPtr<GraphicalItem> pItem = item->second.getGraphicalItem();
      m_board->processContainer(pos,type,name,8,pItem);
   }
}


void MyWidget::setBoardCursorCoord(QPoint point)
{
   r_h->setDrawerCoordinates(point);
   r_v->setDrawerCoordinates(point);
}




QHBoxLayout* MyWidget::createPlatesAndLinesPanel()
{
    //external diameter of plate
    m_plExtspin = new QDoubleSpinBox;
    m_plExtspin->setRange(0,9);
    m_plExtspin->setSingleStep(0.1);
    m_plExtspin->setValue(static_cast<double>(CHIP_PLATE_D_EX));
    //internal diameter(the hole)
    m_plIntspin = new QDoubleSpinBox;
    m_plIntspin->setRange(0,8);
    m_plIntspin->setSingleStep(0.1);
    m_plIntspin->setValue(static_cast<double>(CHIP_PLATE_D_IN));

    m_pLineSpin = new QDoubleSpinBox;
    m_pLineSpin->setRange(0,8);
    m_pLineSpin->setSingleStep(0.1);
    m_pLineSpin->setValue(CONNECTOR_DEF_WIDTH);

    m_pRectWidthSpin = new QDoubleSpinBox;
    m_pRectWidthSpin->setRange(0,8);
    m_pRectWidthSpin->setSingleStep(0.1);
    m_pRectWidthSpin->setValue(DEFAULT_RECT_HEIGHT);

    m_pRectHeightSpin = new QDoubleSpinBox;
    m_pRectHeightSpin->setRange(0,8);
    m_pRectHeightSpin->setSingleStep(0.1);
    m_pRectHeightSpin->setValue(DEFAULT_RECT_HEIGHT);

    CustomButton *push = new CustomButton(CustomButton::TYPE_PLATE_EXT,"        ");
    CustomButton *push1 = new CustomButton(CustomButton::TYPE_PLATE_INTERN,"        ");
    CustomButton *push2 = new CustomButton(CustomButton::TYPE_LINE,"        ");
    CustomButton *push3 = new CustomButton(CustomButton::TYPE_PLATE_RECT_H,"        ");
    CustomButton *push4 = new CustomButton(CustomButton::TYPE_PLATE_RECT_W,"        ");
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->addWidget(push);
    hbox->addWidget(m_plExtspin);
    hbox->addWidget(push1);
    hbox->addWidget(m_plIntspin);
    hbox->addWidget(push2);
    hbox->addWidget(m_pLineSpin);
    hbox->addWidget(push3);
    hbox->addWidget(m_pRectWidthSpin);
    hbox->addWidget(push4);
    hbox->addWidget(m_pRectHeightSpin);
    connect(m_pLineSpin,SIGNAL(valueChanged(const QString&)),m_board,SLOT(setLineWidth(const QString&)));
    return hbox;
}

QVBoxLayout* MyWidget::createLayersPanel()
{
    QVBoxLayout *vLevels = new QVBoxLayout;
    QHBoxLayout *levelsActiveBox = new QHBoxLayout;
    QHBoxLayout *levelsAvailBox = new QHBoxLayout;
    QLabel *actLev = new QLabel(ACTIVE_LEVELS_TEXT);
    levelsActiveBox->addWidget(actLev);
    QLabel *visLev = new QLabel(VISIBLE_LEVELS_TEXT);
    levelsAvailBox->addWidget(visLev);
    BOARD_LEVEL_ID initLevel = LEVEL_A;
    m_radioButGroup.reset(new QButtonGroup);
    m_checkButGroup.reset(new QButtonGroup);
    const QButtonGroup *radioButGroup = m_radioButGroup.get();
    const QButtonGroup *checkButGroup = m_checkButGroup.get();
    //there may be many checked buttons in this group so this is not an exclusive group.
    m_checkButGroup->setExclusive(false);
    for(std::vector<BoardLevel>::size_type i = 0; i < m_ActiveLevels.size(); ++i)
    {
       QRadioButton *button = new QRadioButton(m_ActiveLevels[i].desc);
       QPixmap buffer(32,32);
       buffer.fill(m_ActiveLevels[i].color);
       button->setIcon(buffer);
       if(m_ActiveLevels[i].idLevel == initLevel)
          button->setChecked(true);
       levelsActiveBox->addWidget(button);
       m_radioButGroup->addButton(button,m_ActiveLevels[i].idLevel);
    }
    levelsActiveBox->addStretch(5);
    for(VecLevelInrementer i = 0; i < m_AvailLevels.size(); ++i)
    {
       QCheckBox *button = new QCheckBox(m_AvailLevels[i].desc);
       QPixmap buffer(32,32);
       buffer.fill(m_AvailLevels[i].color);
       button->setIcon(buffer);
       levelsAvailBox->addWidget(button);
       button->setCheckState(Qt::Checked);
       if(m_AvailLevels[i].idLevel == initLevel)
          button->setDisabled(true);
       m_checkButGroup->addButton(button,m_AvailLevels[i].idLevel);
    }

    auto const bdPtr = m_board;
    connect(m_radioButGroup.get(), static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
        [radioButGroup,checkButGroup,bdPtr](QAbstractButton *button)
    {
       auto id = static_cast<BOARD_LEVEL_ID>(radioButGroup->id(button));
       bdPtr->setBoardCurrentLevel(id);
       for (auto chkBut:checkButGroup->buttons())
           chkBut->setDisabled(checkButGroup->id(chkBut) == id);
    }
    );

    connect(m_checkButGroup.get(), static_cast<void(QButtonGroup::*)(QAbstractButton *)>(&QButtonGroup::buttonClicked),
        [radioButGroup,checkButGroup,bdPtr](QAbstractButton *button)
    {
        auto id = static_cast<BOARD_LEVEL_ID>(checkButGroup->id(button));
        bool state = button->isChecked();
        bdPtr->setBoardVisibleLevel(id,state);
        for (auto radBut:radioButGroup->buttons())
           if(radioButGroup->id(radBut) == id)
              radBut->setDisabled(!state);
    }
    );

    levelsAvailBox->addStretch(5);

    vLevels->addLayout(createPlatesAndLinesPanel());
    vLevels->addLayout(levelsActiveBox);
    vLevels->addLayout(levelsAvailBox);
    return vLevels;
}


void MyWidget::newDocument()
{
   checkSuggestSaving();
   SchemData data;
   m_curSchemName = NEW_SCHEM_NAME_DEF;
   m_schematics[m_curSchemName] = data;
   std::vector<SmartPtr<GraphicalItem> > items;
   std::vector<QString> names;
   m_board->openNewSchematic(data,names,items);

}

void MyWidget::open(QLayout *layout)
{
   checkSuggestSaving();
   QString fileName =
           QFileDialog::getOpenFileName(this, tr("Open input file"),
                                         QDir::currentPath(),
                                         tr("XML Files (*.bd_xml)"));
   if (fileName.isEmpty())
        return;


   QFile file(fileName);
   if (!file.open(QFile::ReadOnly | QFile::Text)) {
      QMessageBox::warning(this, tr("QXmlStream "),
                            tr("Cannot read file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
      return;
   }
   m_reader.setDevice(&file);
   std::vector<SmartPtr<GraphicalItem> > items;
   std::vector<QString> names;
   SchemData data = m_reader.parseItems(items,names);
   m_board->openNewSchematic(data,names,items);
   m_curSchemName = fileName;
   m_schematics[m_curSchemName] = data;
   r_h->setBoardSize(data.m_width/PIXELS_PER_MM);
   r_v->setBoardSize(data.m_height/PIXELS_PER_MM);
   cout<<"here"<<endl;
}

void MyWidget::checkSuggestSaving()
{
//   if(!m_schematics.find(m_curSchemName)->second.isStored())
   if(!getUndoStack()->isClean())
   {
      //suggest saving
      QMessageBox msgBox;
      msgBox.setWindowTitle(SAVE_SCHEMA_TITLE);
      msgBox.setIcon(QMessageBox::Information);
      msgBox.setInformativeText(QString::fromUtf8(SAVE_SCHEMA));
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      if(msgBox.exec() == QMessageBox::Yes)
      {
         save(false);
      }
   }
   getUndoStack()->setClean();
}

void MyWidget::close()
{
   checkSuggestSaving();
}




void MyWidget::openLibFile()
{
   QFile file("bd_lib_std.bd_xml");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return ;
    
   m_reader.setDevice(&file);
   m_plibView = new QTreeView(this);

   contextMenu = new QMenu(m_plibView);

   auto newAction = new QAction(tr("New folder"),this);
   connect(newAction,SIGNAL(triggered()), this, SLOT(onNewTreeItem()));
   contextMenu->addAction(newAction);

   auto renameAction = new QAction(tr("Rename"),this);
   connect(renameAction,SIGNAL(triggered()), this, SLOT(onRenameTreeItem()));
   contextMenu->addAction(renameAction);

   auto delAction = new QAction(tr("Delete"),this);
   connect(delAction,SIGNAL(triggered()), this, SLOT(onDeleteTreeItem()));
   contextMenu->addAction(delAction);

   QStandardItemModel *model = new ComponentsTreeModel;
   m_reader.readLibraryFile(*model,m_TreeData);
   m_plibView->setModel(model);
   QItemSelectionModel *selectionModel = new QItemSelectionModel(model);
   m_plibView->setSelectionModel(selectionModel);
   m_plibView->setHeaderHidden(true);
   m_plibView->setContextMenuPolicy(Qt::ContextMenuPolicy::CustomContextMenu);
   connect(m_plibView, SIGNAL(customContextMenuRequested(const QPoint &)), this, SLOT(onCustomTreeContextMenu(const QPoint &)));
   connect(selectionModel, SIGNAL(selectionChanged(const QItemSelection&,const QItemSelection&)), this, SLOT(treeSelectionChanged(const QItemSelection&,const QItemSelection&)));
   file.close();
   m_plibView->setDragEnabled(true);
   m_plibView->setAcceptDrops(true);
   m_plibView->setDropIndicatorShown(true);
}

void MyWidget::getTreeSelection(SmartPtr<GraphicalItem>& item,QString& name)
{
   const QModelIndex index = m_plibView->selectionModel()->currentIndex();
   name = index.data(Qt::DisplayRole).toString();
   auto it = m_TreeData.find(name);
   if(it != m_TreeData.end())
      item = it->second.getGraphicalItem();
}


void MyWidget::treeSelectionChanged(const QItemSelection& , const QItemSelection& )
{
   //set new pixmap or clear existing
   m_plibElementPrwview->setDefaultPixmap();
    //get the text of the selected item
   SmartPtr<GraphicalItem> item;
   QString name;
   getTreeSelection(item,name);
   if(item.get() != nullptr)
      m_plibElementPrwview->previewItem(item,name);
/*
   const QModelIndex index = m_plibView->selectionModel()->currentIndex();
   cout<<index.data(Qt::DisplayRole).toString().toStdString()<<endl;
   QString name = index.data(Qt::DisplayRole).toString();
   auto item = m_TreeData.find(name);
   if(item != m_TreeData.end())
   {
      m_plibElementPrwview->previewItem(item->second.pItem,name);
   }
   else
   {
      m_plibElementPrwview->clearPreview();
   }
*/
}

bool MyWidget::saveToFile(QString& name)
{
   /*
   vector<string> v;
   v.push_back("BD_LIBRARY");
   v.push_back("CUSTOM");
   v.push_back("DIP");
   v.push_back("CAPACITORS");
   
   m_writer.writeToLibrary(v,NULL,NULL);
   return true;
*/   
   QFile file(name);
   if (!file.open(QIODevice::WriteOnly))
      return false;
   
   m_writer.setDevice(&file);
   if(m_schematics.find(m_curSchemName) != m_schematics.end())
   {
      auto item = m_schematics.find(m_curSchemName);
      SchemData data = item->second;
      m_writer.startDocument(data.m_width,data.m_height);
   }
   else
   {
      m_writer.startDocument(NEW_SCHEM_DEF_WIDTH * PIXELS_PER_MM,NEW_SCHEM_DEF_HEIGHT * PIXELS_PER_MM);
   }
   m_board->getBoardLayers()->writeLayersToFile(m_writer);
   m_writer.endDocument();
   m_writer.setDevice(nullptr);
   file.close();
   return true;
}

void MyWidget::addToLib(vector< QString >& v,const QString& name,SmartPtr<GraphicalItem>& p)
{
   SmartPtr<GraphicalItem> p1;
   QMessageBox msgBox;
   if(m_TreeData.find(name) == m_TreeData.end())
   {
      SmartPtr<GraphicalItem> p1 = ItemsFactory::duplicateItem(p.get(),0,0,true);
//      p1->setZoom(1);
      m_writer.writeToLibrary(v,name,p1.get());
   }
   else
   {
      msgBox.setIcon(QMessageBox::Critical);
      msgBox.setInformativeText(NAME_IS_ALREADY_EXISTS_MSG); 
      msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
      int ret = msgBox.exec();
      if(ret == QMessageBox::Yes)
      {
         SmartPtr<GraphicalItem> p1 = ItemsFactory::duplicateItem(p.get(),0,0,true);
//         p1->setZoom(1);
         m_writer.writeToLibrary1(v,name,p1.get());
         auto item = m_TreeData.find(name);
         if(item != m_TreeData.end())
         {
            QString name_current = m_plibElementPrwview->getPreviewItemName();
            bool bRfrsh = (name_current.compare(name) == 0);
            if(bRfrsh)
               m_plibElementPrwview->clearPreview();
            m_TreeData.erase(item);
            if(bRfrsh)
            {
               m_plibElementPrwview->setDefaultPixmap();
               m_plibElementPrwview->previewItem(p1,name);
            }
         }
      }
   }
   tree_data data;
   data.setGraphicalItem(p1);
   QString s = CONTAINER_DEF;
   data.assignTypeFromQString(s);
   m_TreeData[name] = data;
}


void MyWidget::save(bool bSaveTo)
{
   if(m_curSchemName.compare(NEW_SCHEM_NAME_DEF) == 0 || bSaveTo)
   {
      auto item = m_schematics.find(m_curSchemName);
      SchemData data = item->second;
      QString name = QFileDialog::getSaveFileName(this, tr("Save file"),
                                         QDir::currentPath(),
                                         tr("BD_XML Files (*.bd_xml)"));
      if(name.isEmpty())
         return; //cancel was pressed
         
      if (!saveToFile(name))
         return;
      if(!bSaveTo)
         m_schematics.erase(item);
      m_curSchemName = name;
      m_schematics[m_curSchemName] = data;
   }
   else
   {
      saveToFile(m_curSchemName);
   }
   if(!bSaveTo)
      m_pStack->setClean();
}


void MyWidget::print()
{
   m_board->print();
}

void MyWidget::saveAs()
{
   save(true);
}

void MyWidget::loadImage()
{
   QString fileName =
           QFileDialog::getOpenFileName(this, tr("Open input file"),
                                         QDir::currentPath(),
                                         tr("Images (*.bmp *.gif *.png *.jpg)"));
   if (fileName.isEmpty())
        return;


   QFile file(fileName);
   if (!file.open(QFile::ReadOnly)) {
      QMessageBox::warning(this, tr("image"),
                            tr("Cannot read file %1:\n%2.")
                            .arg(fileName)
                            .arg(file.errorString()));
      return;
   }
   m_board->loadImage(fileName);   
}

void MyWidget::updateBoardSize(int width, int height)
{
   SchemData data(width,height);
   m_schematics[m_curSchemName] = data;
   r_h->setBoardSize(data.m_width /PIXELS_PER_MM);
   r_v->setBoardSize(data.m_height/PIXELS_PER_MM);
}

void MyWidget::processRuler()
{
   m_board->processRuler();
}

void MyWidget::unloadImage()
{
   m_board->unloadImage();   
}

void MyWidget::processCopy()
{
   m_board->processCopy();
}

void MyWidget::processCut()
{
   m_board->processCut();
}

void MyWidget::processPaste()
{
   m_board->processPaste();
}

void MyWidget::processSelectAll()
{
   m_board->processSelectAll();
}

void MyWidget::processText(int index)
{
  m_board->processText(fontSizes[static_cast<vector<int> :: size_type > (index)],false);
}

void MyWidget::processAutodraw()
{
    m_board->constructPcbLayout();
}

void MyWidget::processVirtConnector()
{
  m_board->processVcCon();
}

void MyWidget::processConnectedRuler(bool checked)
{
   m_board->processConnectedRuler(checked);
}

void MyWidget::onCustomTreeContextMenu(const QPoint& p)
{
   SmartPtr<GraphicalItem> item;
   QString name;
   getTreeSelection(item,name);
   if(name.size() == 0 && item.get() == nullptr)
      return;

   auto const index = m_plibView->selectionModel()->currentIndex();
   if((static_cast<QStandardItemModel*>(m_plibView->model()))->itemFromIndex(index)->hasChildren())
      contextMenu->actions()
              .at(static_cast<int>(ContextMenuId::Delete))->setDisabled(true);
   else
      contextMenu->actions()
              .at(static_cast<int>(ContextMenuId::Delete))->setDisabled(false);
   contextMenu->exec(m_plibView->viewport()->mapToGlobal(p));
}

void MyWidget::exportGerber()
{
   m_board->processGerber(m_curSchemName);
}

void MyWidget::exportExcellon()
{
   m_board->processExcellon(m_curSchemName);
}


void MyWidget::onNewTreeItem()
{
   auto index = m_plibView->currentIndex();
   if(!index.isValid())
      return;
   auto model = static_cast<QStandardItemModel*>(m_plibView->model());
   auto item = model->itemFromIndex(index);

   auto text = QInputDialog::getText(this,tr("Name"),tr("Name"),
                                     QLineEdit::EchoMode::Normal);

   auto pItem = new QStandardItem(text);
   item->appendRow(pItem);
   auto data = m_schematics.find(m_curSchemName);
//   m_writer.writeModelToLibrary(model,m_TreeData,data->second.m_height,
//                                data->second.m_width);
   m_bTreeWasUpdated = true;
}
void MyWidget::onRenameTreeItem()
{
   auto index = m_plibView->currentIndex();
   if(!index.isValid())
      return;
   auto model = static_cast<ComponentsTreeModel*>(m_plibView->model());
   auto item = model->itemFromIndex(index);
   auto item_text = item->text();
   auto data_pair = m_TreeData.find(item_text);
   auto text = QInputDialog::getText(this,tr("Name"),tr("Name"),QLineEdit::EchoMode::Normal,
                         item->text());

   item->setText(text);
   if(data_pair != m_TreeData.end())
   {
      auto data = data_pair->second;
      m_TreeData.erase(data_pair);
      m_TreeData[text] = data;
   }
   m_bTreeWasUpdated = true;

}
void MyWidget::onDeleteTreeItem()
{
   auto index = m_plibView->currentIndex();
   if(!index.isValid())
      return;


   auto model = static_cast<QStandardItemModel*>(m_plibView->model());
   auto item = model->itemFromIndex(index);
   auto item_text = item->text();
   m_TreeData.erase(item_text);
   model->removeRow(index.row(),index.parent());
   m_bTreeWasUpdated = true;
}



//#include "mywidget.moc"

