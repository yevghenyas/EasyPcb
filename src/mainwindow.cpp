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

#include "mainwindow.h"
#include <QMenuBar>
#include <QToolBar>
#include <QGroupBox>
#include "drawwrapper.h"
#include <QMessageBox>

MainWindow::MainWindow()
{
   m_widget = new MyWidget();
   setCentralWidget(m_widget);
   QToolBar *fileBar = new QToolBar();

   const QIcon newDocIcon = QIcon::fromTheme("document-open", QIcon(":/images/new-document.png"));
   newDocAct = new QAction(newDocIcon,tr(NEW_MENU_STR), this);
   newDocAct->setShortcuts(QKeySequence::Open);
   newDocAct->setStatusTip(tr(OPEN_TIP_STR));
   connect(newDocAct, SIGNAL(triggered()), m_widget, SLOT(newDocument()));
   fileBar->addAction(newDocAct);


   const QIcon openIcon = QIcon::fromTheme("document-open", QIcon(":/images/open.png"));
   openAct = new QAction(openIcon,tr(OPEN_MENU_STR), this);
   openAct->setShortcuts(QKeySequence::Open);
   openAct->setStatusTip(tr(OPEN_TIP_STR));
   connect(openAct, SIGNAL(triggered()), m_widget, SLOT(open()));
   fileBar->addAction(openAct);

   const QIcon saveIcon = QIcon::fromTheme("document-open", QIcon(":/images/save.png"));
   saveAct = new QAction(saveIcon,tr(SAVE_MENU_STR), this);
   saveAct->setShortcuts(QKeySequence::Save);
   saveAct->setStatusTip(tr(SAVE_TIP_STR));
   connect(saveAct, SIGNAL(triggered()), m_widget, SLOT(save()));
   fileBar->addAction(saveAct);
   
   saveAsAct = new QAction(tr(SAVE_AS_MENU_STR), this);
   saveAsAct->setShortcuts(QKeySequence::SaveAs);
   saveAsAct->setStatusTip(tr(SAVE_TIP_STR));
   connect(saveAsAct, SIGNAL(triggered()), m_widget, SLOT(saveAs()));

   const QIcon gbrIcon = QIcon::fromTheme("document-open", QIcon(":/images/gbr.png"));
   gbrAct = new QAction(gbrIcon,tr("Gerber file"), this);
   gbrAct->setStatusTip(tr("Export to gerber file"));
   connect(gbrAct, &QAction::triggered, m_widget, &MyWidget::exportGerber);
   fileBar->addAction(gbrAct);

   const QIcon excIcon = QIcon::fromTheme("document-open", QIcon(":/images/exc.png"));
   excAct = new QAction(excIcon,tr("Excellon file"), this);
   excAct->setStatusTip(tr("Export to excellon file"));
   connect(excAct, &QAction::triggered, m_widget, &MyWidget::exportExcellon);
   fileBar->addAction(excAct);


   const QIcon printIcon = QIcon::fromTheme("document-open", QIcon(":/images/print.png"));
   printAct = new QAction(printIcon,tr(PRINT_MENU_STR), this);
   printAct->setShortcuts(QKeySequence::Print);
   printAct->setStatusTip(tr(PRINT_TIP_STR));
   connect(printAct, SIGNAL(triggered()), m_widget, SLOT(print()));
   fileBar->addAction(printAct);
   
   const QIcon exitIcon = QIcon::fromTheme("document-open", QIcon(":/images/exit.png"));
   exitAct = new QAction(exitIcon,tr(EXIT_MENU_STR), this);
   exitAct->setShortcuts(QKeySequence::Quit);
   exitAct->setStatusTip(tr(EXIT_TIP_STR));
   connect(exitAct, &QAction::triggered, this, &MainWindow::close);
   fileBar->addAction(exitAct);
   

   QToolBar *toolsBar = new QToolBar();

   const QIcon selectIcon = QIcon::fromTheme("document-open", QIcon(":/images/select.png"));
   QAction *selectAct = new QAction(selectIcon,tr(CURSOR_TEXT), this);
   selectAct->setStatusTip(tr(CURSOR_TEXT_TIP));
   connect(selectAct, SIGNAL(triggered()), m_widget, SLOT(processCursor()));
   toolsBar->addAction(selectAct);

   const QIcon connectionIcon = QIcon::fromTheme("document-open", QIcon(":/images/connection.png"));
   QAction *connectionAct = new QAction(connectionIcon,tr(CONNECTOR_TEXT), this);
   connectionAct->setStatusTip(tr(CONNECTOR_TEXT_TIP));
   connect(connectionAct, SIGNAL(triggered()), m_widget, SLOT(processLines()));
   toolsBar->addAction(connectionAct);

   const QIcon linkIcon = QIcon::fromTheme("document-open", QIcon(":/images/link.png"));
   QAction *linkAct = new QAction(linkIcon,tr(EL_CONNECTOR_TEXT), this);
   linkAct->setStatusTip(tr(EL_CONNECTOR_TEXT_TIP));
   connect(linkAct, SIGNAL(triggered()), m_widget, SLOT(processVirtConnector()));
   toolsBar->addAction(linkAct);

   const QIcon zoomIcon = QIcon::fromTheme("document-open", QIcon(":/images/zoom.png"));
   QAction *zoomAct = new QAction(zoomIcon,tr(ZOOM_IN_TEXT), this);
   zoomAct->setStatusTip(tr(ZOOM_IN_TEXT_TIP));
   connect(zoomAct, SIGNAL(triggered()), m_widget, SLOT(processZoom_plus()));
   toolsBar->addAction(zoomAct);

   const QIcon zoomOutIcon = QIcon::fromTheme("document-open", QIcon(":/images/zoom-out.png"));
   QAction *zoomOutAct = new QAction(zoomOutIcon,tr(ZOOM_OUT_TEXT), this);
   zoomOutAct->setStatusTip(tr(ZOOM_OUT_TEXT_TIP));
   connect(zoomOutAct, SIGNAL(triggered()), m_widget, SLOT(processZoom_minus()));
   toolsBar->addAction(zoomOutAct);

   const QIcon rulerIcon = QIcon::fromTheme("document-open", QIcon(":/images/ruler.png"));
   QAction *rulerAct = new QAction(rulerIcon,tr(RULER_STR), this);
   rulerAct->setStatusTip(tr(RULER_STR));
   connect(rulerAct, SIGNAL(triggered()), m_widget, SLOT(processRuler()));
   toolsBar->addAction(rulerAct);

   const QIcon distIcon = QIcon::fromTheme("document-open", QIcon(":/images/distance.png"));
   distAct = new QAction(distIcon,tr(DISTANCE_STR), this);
   distAct->setCheckable(true);
   distAct->setStatusTip(tr(DISTANCE_STR));
   connect(distAct, &QAction::triggered, this, &MainWindow::processConnectedRuler);
//   connect(distAct, SIGNAL(triggered()), this, SLOT(processConnectedRuler()));
   toolsBar->addAction(distAct);


   const QIcon templateIcon = QIcon::fromTheme("document-open", QIcon(":/images/ic.png"));
   loadTemplateAct = new QAction(templateIcon,tr(LOAD_TEMPLATE), this);
   loadTemplateAct->setStatusTip(tr(LOAD_TEMPLATE_TIP));
   connect(loadTemplateAct, SIGNAL(triggered()), m_widget, SLOT(loadImage()));
   toolsBar->addAction(loadTemplateAct);
   
   const QIcon eraseTmpltIcon = QIcon::fromTheme("document-open", QIcon(":/images/eraser.png"));
   unloadTemplateAct = new QAction(eraseTmpltIcon,tr(UNLOAD_TEMPLATE), this);
   unloadTemplateAct->setStatusTip(tr(UNLOAD_TEMPLATE_TIP));
   connect(unloadTemplateAct, SIGNAL(triggered()), m_widget, SLOT(unloadImage()));
   toolsBar->addAction(unloadTemplateAct);

   QAction *aboutAct = new QAction(tr("About"),this);
   connect(aboutAct, &QAction::triggered,
           this,[=](){QMessageBox::about(this,"About program",
                                         "Author:Yevgheny Svidersky");
                     });
   toolsBar->addAction(unloadTemplateAct);


   const QIcon autoIcon = QIcon::fromTheme("document-open", QIcon(":/images/auto.png"));
   autodrawAction = new QAction(autoIcon,tr("Construct PCB"),this);
   autodrawAction->setStatusTip(tr("Replace electrical connector with physical"));
   connect(autodrawAction, SIGNAL(triggered()), m_widget, SLOT(processAutodraw()));
   toolsBar->addAction(autodrawAction);

   toolsBar->addWidget(createPlateCombo());
   toolsBar->addWidget(createTextCombo());
   
   fileMenu = menuBar()->addMenu(tr("&File"));
   fileMenu->addAction(newDocAct);
   fileMenu->addAction(openAct);
   fileMenu->addAction(saveAct);
   fileMenu->addAction(saveAsAct);
   auto exportMenu = fileMenu->addMenu(tr("Export"));
   exportMenu->addAction(gbrAct);
   exportMenu->addAction(excAct);
   fileMenu->addAction(printAct);
   fileMenu->addAction(exitAct);

   QToolBar *editBar = new QToolBar();

   const QIcon cutIcon = QIcon::fromTheme("document-open", QIcon(":/images/cut.png"));
   cutAct = new QAction(cutIcon,tr("Cut"), this);
   cutAct->setShortcuts(QKeySequence::Cut);
   cutAct->setStatusTip(tr(CUT_DEF_TIP_STR));
   connect(cutAct, SIGNAL(triggered()), m_widget, SLOT(processCut()));
   editBar->addAction(cutAct);

   const QIcon copyIcon = QIcon::fromTheme("document-open", QIcon(":/images/copy.png"));
   copyAct = new QAction(copyIcon,tr("Copy"), this);
   copyAct->setShortcuts(QKeySequence::Copy);
   copyAct->setStatusTip(tr(COPY_DEF_TIP_STR));
   connect(copyAct, SIGNAL(triggered()), m_widget, SLOT(processCopy()));
   editBar->addAction(copyAct);

   const QIcon pasteIcon = QIcon::fromTheme("document-open", QIcon(":/images/paste.png"));
   pasteAct = new QAction(pasteIcon,tr("Paste"), this);
   pasteAct->setShortcuts(QKeySequence::Paste);
   pasteAct->setStatusTip(tr(CUT_DEF_TIP_STR));
   connect(pasteAct, SIGNAL(triggered()), m_widget, SLOT(processPaste()));
   editBar->addAction(pasteAct);

   const QIcon selectAllIcon = QIcon::fromTheme("document-open", QIcon(":/images/select-all.png"));
   selAllAct = new QAction(selectAllIcon,tr("Select all"), this);
   selAllAct->setShortcuts(QKeySequence::SelectAll);
   selAllAct->setStatusTip(tr(SELALL_DEF_TIP_STR));
   connect(selAllAct, SIGNAL(triggered()), m_widget, SLOT(processSelectAll()));
   editBar->addAction(selAllAct);

   m_pStack = new QUndoStack(this);

   const QIcon undoIcon = QIcon::fromTheme("document-open", QIcon(":/images/undo.png"));
   undoAction = m_pStack->createUndoAction(this, tr("&Undo"));
   undoAction->setShortcuts(QKeySequence::Undo);
   undoAction->setIcon(undoIcon);

   const QIcon redoIcon = QIcon::fromTheme("document-open", QIcon(":/images/redo.png"));
   redoAction = m_pStack->createRedoAction(this, tr("&Redo"));
   redoAction->setShortcuts(QKeySequence::Redo);
   redoAction->setIcon(redoIcon);

   m_widget->setUndoStack(m_pStack);

   editMenu = menuBar()->addMenu(tr("Edit"));
   editMenu->addAction(cutAct);
   editMenu->addAction(copyAct);
   editMenu->addAction(pasteAct);
   editMenu->addAction(selAllAct);
   editMenu->addAction(undoAction);
   editMenu->addAction(redoAction);

   editBar->addAction(undoAction);
   editBar->addAction(redoAction);

   toolsMenu = menuBar()->addMenu(tr("Tools"));
   toolsMenu->addAction(aboutAct);
   toolsMenu->addAction(loadTemplateAct);
   toolsMenu->addAction(unloadTemplateAct);
   toolsMenu->addAction(autodrawAction);
   addToolBar(fileBar);
   addToolBar(editBar);
   addToolBar(toolsBar);
}

QComboBox* MainWindow::createPlateCombo()
{
    QComboBox *combo = new QComboBox();

    QPixmap buffer(24,24);
    buffer.fill(Qt::transparent);
    QPainter pnt(&buffer);
    QColor c(Qt::black);
    DrawWrapper::drawPlate(pnt,c,12,12,22,10);
    QIcon icon(buffer);
    combo->addItem(icon,"");

    QPixmap buffer1(24,24);
    buffer1.fill(Qt::transparent);
    QPainter pnt1(&buffer1);
    DrawWrapper::drawRectPlate(pnt1,c,12,12,22,22);
    QIcon icon1(buffer1);
    combo->addItem(icon1,"");

    QPixmap buffer2(24,24);
    buffer2.fill(Qt::transparent);
    QPainter pnt2(&buffer2);
    DrawWrapper::drawRectPackage(pnt2,c,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,12,12,22,22);
    QIcon icon2(buffer2);
    combo->addItem(icon2,"");

    QPixmap buffer3(24,24);
    buffer3.fill(Qt::transparent);
    QPainter pnt3(&buffer3);
    DrawWrapper::drawRoundPackage(pnt3,c,12,12,22,22,1);
    QIcon icon3(buffer3);
    combo->addItem(icon3,"");

    //double plate,metallyzed hole.
    QPixmap buffer4(24,24);
    buffer4.fill(Qt::transparent);
    QPainter pnt4(&buffer4);
    QColor c4(Qt::gray);
    DrawWrapper::drawMultiPlate(pnt4,c4,12,12,18,4);
    QIcon icon4(buffer4);
    combo->addItem(icon4,"");

    //double plate,metallyzed hole.
    QPixmap buffer5(24,24);
    buffer5.fill(Qt::transparent);
    QPainter pnt5(&buffer5);
    DrawWrapper::drawFilledRectPackage(pnt5,c4,ITEMS_ORIENTATION::O_HORIZONTAL_LEFT,12,12,22,22);
    QIcon icon5(buffer5);
    combo->addItem(icon5,"");

    QObject::connect(combo, SIGNAL(activated(int)), m_widget, SLOT(processPlate(int)));
    return combo;
}

QWidget* MainWindow::createTextCombo()
{
   QWidget *groupText = new QWidget;
   QHBoxLayout *vText = new QHBoxLayout;

   QLabel *label = new QLabel("Abc...");
   QComboBox *fntCombo = new QComboBox;
   for(auto item:fontSizes)
      fntCombo->addItem(QString::number(item));

   vText->addWidget(label);
   vText->addWidget(fntCombo);

   groupText->setLayout(vText);
   connect(fntCombo,SIGNAL(activated(int)),m_widget,SLOT(processText(int)));
   return groupText;
}


void MainWindow::processConnectedRuler()
{
   m_widget->processConnectedRuler(distAct->isChecked());
}

MainWindow::~MainWindow()
{
   m_widget->close();
}
