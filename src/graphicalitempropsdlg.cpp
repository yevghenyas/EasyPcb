/*
 * Copyright 2018 <copyright holder> <email>
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

#include "graphicalitempropsdlg.h"
#include "common.h"
#include "rectgraphicalitem.h"
#include "rectpackagegraphicalitem.h"
#include "roundpackagegraphicalitem.h"
#include "roundplategraphicalitem.h"
#include "connectorgraphicalitem.h"
#include "capgraphicalitem.h"
#include "genericgraphicalitemscontainer.h"
#include <QMessageBox>
#include "textgraphicalitem.h"
#include <QGroupBox>

GraphicalItemPropsDlg::GraphicalItemPropsDlg(QWidget* parent, SmartPtr<GraphicalItem>& item): QDialog(parent),
                layersCombo(nullptr),m_pItem(item)
{
   if (objectName().isEmpty())
      setObjectName(QString::fromUtf8("Dialog"));
   resize(620, 294);
   buttonBox = new QDialogButtonBox(this);
   buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
   buttonBox->setGeometry(QRect(520, 20, 81, 241));
   buttonBox->setOrientation(Qt::Vertical);
   buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
   gridLayoutWidget = new QWidget(this);
   gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
   gridLayoutWidget->setGeometry(QRect(9, 10, 470, 280));
   gridLayout = new QGridLayout(gridLayoutWidget);

   int rowCounter = 0;

   createOneRow(X_DEF,"posXSpinBox",&posXSpinBox,
                static_cast<double>(m_pItem->x()),rowCounter++);

   createOneRow(Y_DEF,"posYSpinBox",&posYSpinBox,
                static_cast<double>(m_pItem->y()),rowCounter++);

   //check type of grapfical item
   ContainerType type = ContainerType::NonContainer;
   set<BOARD_LEVEL_ID> ids;
   if(RectGraphicalItem *rp = dynamic_cast<RectGraphicalItem*>(m_pItem.get()))
   {
      setFieldsForRects(rp->width() ,rp->height(),rowCounter);
   }
   else if(RectPackageGraphicalItem *rpg = dynamic_cast<RectPackageGraphicalItem*>(m_pItem.get()))
   {
      setFieldsForRects(rpg->width(),rpg->height(),rowCounter);
   }
   else if(RoundPackageGraphicalItem *rndp = dynamic_cast<RoundPackageGraphicalItem*>(m_pItem.get()))
   {
      setFieldsForRoundShape(rndp->d(),rndp->d1(),-1,rndp->getStartAngle(),
                             rndp->getSpanAngle(),rndp->isClosed(),rowCounter);
   }
   else if(RoundPlateGraphicalItem *rndplt = dynamic_cast<RoundPlateGraphicalItem*>(m_pItem.get()))
   {
      setFieldsForRounds(rndplt->d(),rndplt->d(),rndplt->d1(),rowCounter);
   }
   else if(ConnectorGraphicalItem *pln = dynamic_cast<ConnectorGraphicalItem*>(m_pItem.get()))
   {
      setFieldsForLine(pln->width(),rowCounter);
   }
   else if(CapGraphicalItem *pc = dynamic_cast<CapGraphicalItem*>(m_pItem.get()))
   {
      setFieldsForLine(pc->f(),rowCounter);
   }
   else if(TextGraphicalItem *pText = dynamic_cast<TextGraphicalItem*>(m_pItem.get()))
   {
     setFieldsForText(pText->getFontSize(),
                      pText->getType(),pText->getText(),rowCounter);
   }
   else
   {
      type = checkContainerType(m_pItem.get(),ids);
      //special handling for containers
      if(type == ContainerType::Multiplate)
      {
         vector<BoardLevel> v;
         LevelsWrapper::getActiveLevels(v);
         multiplateChks.reserve(v.size());

         //create checkboxes for each level
         for(std::vector<BoardLevel>::size_type i = 0 ; i < v.size() ;++i)
         {
            QCheckBox *p = new QCheckBox(gridLayoutWidget);
            p->setObjectName(QString::fromUtf8(v[i].desc));
            p->setText(v[i].desc);
            if(ids.find(v[i].idLevel) != ids.end())
               p->setCheckState(Qt::Checked);
            else
               p->setCheckState(Qt::Unchecked);
            multiplateChks.push_back(p);
            gridLayout->addWidget(p, 2 + static_cast<int>(i), 1, 1, 1);
         }
      }
   }
   if(type == ContainerType::RelocatableDipType ||
      type == ContainerType::RelocatableSoType ||
      type == ContainerType::NonContainer )
   {
      vector<BoardLevel> v;
      if(type == ContainerType::RelocatableDipType ||
              type == ContainerType::RelocatableSoType)
      {
         LevelsWrapper::getActiveLevels(v);
      }
      else
      {
         //non-container,add level manually.
         //this will be used for setting active copbo level
         ids.insert(m_pItem->getLevel());
         LevelsWrapper::getDrawableLevels(v);
      }
      createLayersCombo(type,ids,v,rowCounter++);
   }


   updPropsChk = new QCheckBox(gridLayoutWidget);
   updPropsChk->setObjectName(QString::fromUtf8("checkBox"));
   updPropsChk->setText(tr(UPDATE_GEOM_PRP_STR));
   gridLayout->addWidget(updPropsChk); //, 3, 0, 1, 1);

   connect(posXSpinBox,SIGNAL(valueChanged(double)),this,SLOT(changedProperties(double)));
   connect(posYSpinBox,SIGNAL(valueChanged(double)),this,SLOT(changedProperties(double)));


   if(geomASpinBox)
      connect(geomASpinBox,SIGNAL(valueChanged(double)),this,SLOT(changedProperties(double)));
   if(geomBSpinBox)
      connect(geomBSpinBox,SIGNAL(valueChanged(double)),this,SLOT(changedProperties(double)));

   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

   QMetaObject::connectSlotsByName(this);   

}

void GraphicalItemPropsDlg::createOneRow(const char *szLabelName,
                                         const char *szSpinName,
                                         QDoubleSpinBox **spinBox,
                                         double value,
                                         int row,
                                         int minValue,
                                         int maxValue)
{
    QLabel *label = new QLabel(gridLayoutWidget);
    label->setObjectName(QString::fromUtf8(szLabelName));
    label->setText(QString::fromUtf8(szLabelName));

    gridLayout->addWidget(label, row, 0, 1, 1);

    *spinBox = new QDoubleSpinBox(gridLayoutWidget);
    (*spinBox)->setObjectName(QString::fromUtf8(szSpinName));
    (*spinBox)->setMinimum(minValue);
    (*spinBox)->setMaximum(maxValue);
    (*spinBox)->setValue(value);
    gridLayout->addWidget(*spinBox, row, 1, 1, 1);

}

void GraphicalItemPropsDlg::createLayersCombo(ContainerType type,set<BOARD_LEVEL_ID> &ids,
                                              vector<BoardLevel>& v,int rowCounter)
{
   QLabel *label_3 = new QLabel(gridLayoutWidget);
   label_3->setObjectName(QString::fromUtf8("label_3"));
   label_3->setText(tr(LEVEL_DEF));

   gridLayout->addWidget(label_3, rowCounter, 0, 1, 1);


   //create combo for selecting layer

   auto l = [&type,&ids](){
      if(type == ContainerType::NonContainer)
         return *ids.begin();
      for(auto id:ids)
         if(LevelsWrapper::isActiveLevelExternal(id))
            return id;
      };


   layersCombo = new LayersCombo(type == ContainerType::NonContainer ?
                                     LayersCombo::ComboType::Drawable :
                                     LayersCombo::ComboType::BaseContainerLayers ,
                                     l(),
                                     gridLayoutWidget);
   layersCombo->setObjectName(QString::fromUtf8("comboBox"));

   gridLayout->addWidget(layersCombo, rowCounter, 1, 1, 1);

   /*
   //create combo for selecting layer
   comboBox = new QComboBox(gridLayoutWidget);
   comboBox->setObjectName(QString::fromUtf8("comboBox"));

   gridLayout->addWidget(comboBox, rowCounter, 1, 1, 1);
   int activeIndex = -1;
   //only A and F supported for RelocatableContainer
   for(std::vector<BoardLevel>::size_type i = 0, j = 0 ; i < v.size() ;++i )
   {
      if(type == ContainerType::NonContainer ||
              LevelsWrapper::isActiveLevelExternal(v[i].idLevel))
      {
         comboBox->addItem(v[i].desc);
         m_levels[static_cast<int>(j)] = v[i].idLevel;
         if(ids.find(v[i].idLevel) != ids.end())
            activeIndex = static_cast<int>(j);
         //increment combo index counter
         ++j;
      }
   }
   //set current layer for container
   if(activeIndex != -1)
      comboBox->setCurrentIndex(activeIndex);
   */
   connect(layersCombo,SIGNAL(activated(int)),this,SLOT(processLevel(int)));
}




GraphicalItemPropsDlg::~GraphicalItemPropsDlg()
{

}

bool GraphicalItemPropsDlg::getResult(BOARD_LEVEL_ID &iLevel,
                                      shared_ptr<PointF>& pos,
                                      shared_ptr<GeomCommonProps>& props,
                                      ContainerType& containerType,
                                      ITEMS_ORIENTATION& o)
{
   //set position of item
   pos.reset(new PointF(static_cast<float>(posXSpinBox->value()),
                           static_cast<float>(posYSpinBox->value())));

   //check type of grapfical item
   //and set geometry values
   containerType = ContainerType::NonContainer;
   if(dynamic_cast<RectGraphicalItem*>(m_pItem.get()))
   {
      props.reset(makeRectGeom(static_cast<float>(geomASpinBox->value()),
                                      static_cast<float>(geomBSpinBox->value())));
   }
   else if(dynamic_cast<RectPackageGraphicalItem*>(m_pItem.get()))
   {
      props.reset(makeRectGeom(static_cast<float>(geomASpinBox->value()),
                               static_cast<float>(geomBSpinBox->value())));
   }
   else if(dynamic_cast<RoundPackageGraphicalItem*>(m_pItem.get()))
   {
      props.reset(makeRoundShapeGeom(static_cast<float>(geomASpinBox->value()),
                                        static_cast<int>(geomStAngSpin->value()),
                                        static_cast<int>(geomSpAngSpin->value()),
                                        chkChord->checkState() == Qt::CheckState::Checked));
   }
   else if(dynamic_cast<RoundPlateGraphicalItem*>(m_pItem.get()))
   {
      props.reset(makeRoundPlateGeom(static_cast<float>(geomASpinBox->value()),
                                      static_cast<float>(geomBSpinBox->value())));
   }
   else if(dynamic_cast<ConnectorGraphicalItem*>(m_pItem.get()))
   {
      LINE_STYLE st = radioSquared->isChecked() ? LINE_STYLE::LINE_SQUARED : LINE_STYLE::LINE_ROUNDED;
      props.reset(makeLineGeom(static_cast<float>(geomASpinBox->value()),st));
   }
   else if(dynamic_cast<CapGraphicalItem*>(m_pItem.get()))
   {
      props.reset(makeCapGeom(static_cast<float>(geomASpinBox->value())));
   }
   else if(TextGraphicalItem *pText = dynamic_cast<TextGraphicalItem*>(m_pItem.get()))
   {
      pText->setText(pTextEdit->toPlainText());
      props.reset(makeTextGeom(fontSizes[fntCombo->currentIndex()]));
      updPropsChk->setCheckState(Qt::CheckState::Checked);
   }

   //set layer(level)
   set<BOARD_LEVEL_ID> idLevels;
   containerType = checkContainerType(m_pItem.get(),idLevels);
   if(containerType == ContainerType::RelocatableDipType ||
           containerType == ContainerType::RelocatableSoType ||
           containerType == ContainerType::NonContainer)
       iLevel = layersCombo->getSelectedLayerId();

   return updPropsChk->checkState() == Qt::CheckState::Checked;
}



void GraphicalItemPropsDlg::processLevel(int )
{
}

void GraphicalItemPropsDlg::createFntCombo(int rowCounter,int initFnt)
{

   QLabel *fonts = new QLabel(gridLayoutWidget);
   fonts->setObjectName(QString::fromUtf8("fonts"));
   fonts->setText(tr("Font sizes"));

   gridLayout->addWidget(fonts, rowCounter, 0, 1, 1);

    //create combo for selecting layer
   fntCombo = new QComboBox(gridLayoutWidget);
   fntCombo->setObjectName(QString::fromUtf8("orientationCombo"));

   gridLayout->addWidget(fntCombo, rowCounter, 1, 1, 1);
   auto selIndex = 0;
   auto index = 0;
   for(auto item:fontSizes)
   {
      fntCombo->addItem(QString::number(item));
      if(item == initFnt)
          selIndex = index;
      ++index;
   }
   fntCombo->setCurrentIndex(selIndex);
}
void GraphicalItemPropsDlg::createOrientationCombo(int rowCounter,ITEMS_ORIENTATION initOrient)
{
    QLabel *orientation = new QLabel(gridLayoutWidget);
    orientation->setObjectName(QString::fromUtf8("orientation"));
    orientation->setText(tr(ORIENTATION_DEF));

    gridLayout->addWidget(orientation, rowCounter, 0, 1, 1);

    //create combo for selecting layer
    orientationCombo = new QComboBox(gridLayoutWidget);
    orientationCombo->setObjectName(QString::fromUtf8("orientationCombo"));

    gridLayout->addWidget(orientationCombo, rowCounter, 1, 1, 1);
    for(auto item:orientation_angles)
    {
       orientationCombo->addItem(QString::number(item));
    }
/*
    int selIndex;
    switch(initOrient)
    {
      case O_VERTICAL_TOP:
        selIndex = 1;
      case O_HORIZONTAL_LEFT:
        selIndex =
      case O_HORIZONTAL_RIGHT:
      case O_VERTICAL_BOTTOM:
    }
*/
    orientationCombo->setCurrentIndex(initOrient);
}


void GraphicalItemPropsDlg::setFieldsForText(int fntSize,
                                             ITEMS_ORIENTATION o,
                                             const QString& text,
                                             int& rowCounter)
{
   QLabel *label = new QLabel(gridLayoutWidget);
   label->setObjectName(QString::fromUtf8("Text"));
   label->setText(QString::fromUtf8("Text"));

   gridLayout->addWidget(label, rowCounter, 0, 1, 1);

   pTextEdit = new QPlainTextEdit(text,gridLayoutWidget);
   pTextEdit->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
   pTextEdit->setObjectName(QString::fromUtf8("TextEdit"));
   gridLayout->addWidget(pTextEdit, rowCounter, 1, 1, 1);
   rowCounter++;

   createFntCombo(rowCounter++,fntSize);
}

void GraphicalItemPropsDlg::setFieldsForRects(float width, float height,
                                              int& rowCounter)
{
    createOneRow(WIDTH_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(width),rowCounter++);

    createOneRow(HEIGHT_DEF,"geomBSpinBox",&geomBSpinBox,
                 static_cast<double>(height),rowCounter++);
}

void GraphicalItemPropsDlg::setFieldsForRounds(float d_ex,float ,
                                               float d_in,int& rowCounter)
{
    createOneRow(D_EX_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(d_ex),rowCounter++);

    if(d_in > 0)
       createOneRow(D_IN_DEF,"geomBSpinBox",&geomBSpinBox,
                 static_cast<double>(d_in),rowCounter++);

}

void GraphicalItemPropsDlg::setFieldsForRoundShape(float d_ex,float ,
                                               float d_in,
                                               int a_start,
                                               int a_span,
                                               bool closed,
                                               int& rowCounter)
{
    createOneRow(D_EX_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(d_ex),rowCounter++);

    if(d_in > 0)
       createOneRow(D_IN_DEF,"geomBSpinBox",&geomBSpinBox,
                 static_cast<double>(d_in),rowCounter++);

    createOneRow(ANGLE_ST_DEF,"geomASpinBox",&geomStAngSpin,
                 a_start,rowCounter++,-180,180);

    createOneRow(ANGLE_SP_DEF,"geomASpinBox",&geomSpAngSpin,
                 a_span,rowCounter++,-180,270);

    chkChord = new QCheckBox(gridLayoutWidget);
    chkChord->setObjectName(QString::fromUtf8("checkBoxChord"));
    chkChord->setText(tr(CHORD));
    chkChord->setCheckState(closed ? Qt::CheckState::Checked : Qt::CheckState::Unchecked);
    gridLayout->addWidget(chkChord); //, 3, 0, 1, 1);

    rowCounter++;

}



void GraphicalItemPropsDlg::changedProperties(double )
{
   updPropsChk->setCheckState(Qt::CheckState::Checked);
}

void GraphicalItemPropsDlg::drawLineForRadio(QPixmap *pBuf,LINE_STYLE st)
{
   QPainter painter(pBuf);
   int w = pBuf->width();
   int h = pBuf->height();
   pBuf->fill(palette().color(QPalette::Window));
   QPen pen;
   pen.setColor(Qt::black);
   pen.setWidth(2);
   painter.setPen(pen);
   if(st == LINE_STYLE::LINE_ROUNDED)
   {
      painter.drawArc(0,h/4,w/4,h/2,90 * 16,180 * 16);
      painter.drawLine(w/4,h/4,w * 3/4,h/4);
      painter.drawLine(w/4,h * 3/4,w * 3/4,h * 3/4);
      painter.drawArc(w * 3/4,h/4,w/4,h/2,90 * 16,-180 * 16);
   }
   else
   {
      painter.drawRect(0,h/4,w,h/2);
   }
}

void GraphicalItemPropsDlg::setFieldsForLine(float w,int& rowCounter)
{
    createOneRow(WIDTH_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(w),rowCounter++);

   posYSpinBox->setEnabled(false);
   posXSpinBox->setEnabled(false);

   ConnectorGraphicalItem *pLine = static_cast<ConnectorGraphicalItem*>(m_pItem.get());

   QGroupBox *pgBox = new QGroupBox(tr("Connector style"));
   QVBoxLayout *vBox = new QVBoxLayout;
   QPixmap px1(32,32);
   drawLineForRadio(&px1,LINE_STYLE::LINE_ROUNDED);
   QPixmap px2(32,32);
   drawLineForRadio(&px2,LINE_STYLE::LINE_SQUARED);
   radioRound = new QRadioButton(tr("Rounded"));
   radioSquared = new QRadioButton(tr("Squared"));
   pLine->getStyle() == LINE_STYLE::LINE_SQUARED ? radioSquared->setChecked(true) :
                                                   radioRound->setChecked(true);
   radioRound->setIcon(px1);
   radioSquared->setIcon(px2);
   vBox->addWidget(radioRound);
   vBox->addWidget(radioSquared);
   pgBox->setLayout(vBox);
   gridLayout->addWidget(pgBox, rowCounter++, 0);
/*
   checkSqStyle = new QCheckBox(tr("Squared"));
   checkSqStyle->setObjectName(QString::fromUtf8("Squared"));
   checkSqStyle->setChecked(pLine->getStyle() == LINE_STYLE::LINE_SQUARED);
   gridLayout->addWidget(checkSqStyle, rowCounter++, 0, 1, 1);
*/
}

void GraphicalItemPropsDlg::accept()
{
   QDialog::accept();
}


ContainerType GraphicalItemPropsDlg::checkContainerType(GraphicalItem *p,set<BOARD_LEVEL_ID>& levelIds)
{
   ContainerType type = ContainerType::NonContainer;
   if(GenericGraphicalItemsContainer *pCont = dynamic_cast<GenericGraphicalItemsContainer*>(p))
   {
      pCont->getLevels(levelIds);
      if(levelIds.size() > 1)
      {
         auto item = std::find_if(levelIds.begin(),levelIds.end(),
                         [](BOARD_LEVEL_ID level)
                            {
                               return LevelsWrapper::isActiveLevelExternal(level);
                            });
         if(item != levelIds.end())
         {
            if(levelIds.find(LevelsWrapper::geLevelForDip(*item)) != levelIds.end())
            {
               type = ContainerType::RelocatableDipType;
               return type;
            }
            else if(levelIds.find(LevelsWrapper::geLevelForSO(*item)) != levelIds.end())
            {
               type = ContainerType::RelocatableSoType;
               return type;
            }
         }
      }
      //check for multiplate
      set<BOARD_LEVEL_ID> levelIds;
      vector<SmartPtr<GraphicalItem> > *pV = pCont->getChildren();
      if(pV->size() <= LevelsWrapper:: getActiveLevelsSize())
      {
         type = ContainerType::Multiplate;
         for(std::vector<SmartPtr<GraphicalItem> >::size_type i = 0; i < pV->size(); ++i)
         {
            RoundPlateGraphicalItem *rndplt = dynamic_cast<RoundPlateGraphicalItem*>((*pV)[i].get());
            if(!rndplt ||
                (i > 0 && ((*pV)[i]->getPos() != (*pV)[i-1]->getPos())))
            {
               levelIds.clear();
               type = ContainerType::NonRelocatableCont;
               break;
            }
            levelIds.insert((*pV)[i]->getLevel());
         }
      }
   }
   return type;
}
