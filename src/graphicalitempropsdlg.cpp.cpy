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
#include "linegraphicalitem.h"
#include "capgraphicalitem.h"
#include "genericgraphicalitemscontainer.h"
#include <QMessageBox>

GraphicalItemPropsDlg::GraphicalItemPropsDlg(QWidget* parent, SmartPtr<GraphicalItem>& item): QDialog(parent),
                comboBox(nullptr),m_pItem(item)
{
   if (objectName().isEmpty())
      setObjectName(QString::fromUtf8("Dialog"));
   resize(400, 294);
   buttonBox = new QDialogButtonBox(this);
   buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
   buttonBox->setGeometry(QRect(290, 20, 81, 241));
   buttonBox->setOrientation(Qt::Vertical);
   buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
   gridLayoutWidget = new QWidget(this);
   gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
   gridLayoutWidget->setGeometry(QRect(9, 10, 251, 281));
   gridLayout = new QGridLayout(gridLayoutWidget);

   int rowCounter = 0;

   createOneRow(X_DEF,"posXSpinBox",&posXSpinBox,
                static_cast<double>(m_pItem->x()),rowCounter++);

   createOneRow(Y_DEF,"posYSpinBox",&posYSpinBox,
                static_cast<double>(m_pItem->y()),rowCounter++);

/*
   label = new QLabel(gridLayoutWidget);
   label->setObjectName(QString::fromUtf8("label"));
   label->setText(QString::fromUtf8(X_DEF));

   gridLayout->addWidget(label, 0, 0, 1, 1);

   posXSpinBox = new QDoubleSpinBox(gridLayoutWidget);
   posXSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
   oldX = static_cast<double>(m_pItem->x());
   posXSpinBox->setValue(oldX);

   gridLayout->addWidget(posXSpinBox, 0, 1, 1, 1);

   label_2 = new QLabel(gridLayoutWidget);
   label_2->setObjectName(QString::fromUtf8("label_2"));
   label_2->setText(QString::fromUtf8(Y_DEF));

   gridLayout->addWidget(label_2, 1, 0, 1, 1);


   posYSpinBox = new QDoubleSpinBox(gridLayoutWidget);
   posYSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
   //store initial value
   oldY = static_cast<double>(m_pItem->y());
   posYSpinBox->setValue(oldY);

   gridLayout->addWidget(posYSpinBox, 1, 1, 1, 1);

*/

 
   

   //check type of grapfical item
   RectGraphicalItem *rp = nullptr;
   RectPackageGraphicalItem *rpg = nullptr;
   RoundPackageGraphicalItem *rndp = nullptr;
   RoundPlateGraphicalItem *rndplt = nullptr;
   LineGraphicalItem *pln = nullptr;
   CapGraphicalItem *pc = nullptr;
   ContainerType type = ContainerType::NonContainer;
   set<BOARD_LEVEL_ID> ids;
   if((rp = dynamic_cast<RectGraphicalItem*>(m_pItem.get())))
   {

      setFieldsForRects(rp->width() ,rp->height(),rowCounter);
   }
   else if((rpg = dynamic_cast<RectPackageGraphicalItem*>(m_pItem.get())))
   {
      setFieldsForRects(rpg->width(),rpg->height(),rowCounter);
   }
   else if((rndp = dynamic_cast<RoundPackageGraphicalItem*>(m_pItem.get())))
   {
      setFieldsForRounds(rndp->d(),rndp->d1(),-1,rowCounter);
   }
   else if((rndplt = dynamic_cast<RoundPlateGraphicalItem*>(m_pItem.get())))
   {
      setFieldsForRounds(rndplt->d(),rndplt->d(),rndplt->d1(),rowCounter);
   }
   else if((pln = dynamic_cast<LineGraphicalItem*>(m_pItem.get())))
   {
      setFieldsForLine(pln->width(),rowCounter);
   }
   else if((pc = dynamic_cast<CapGraphicalItem*>(m_pItem.get())))
   {
      setFieldsForLine(pc->f(),rowCounter);
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
                                         int row)
{
    QLabel *label = new QLabel(gridLayoutWidget);
    label->setObjectName(QString::fromUtf8(szLabelName));
    label->setText(QString::fromUtf8(szLabelName));

    gridLayout->addWidget(label, row, 0, 1, 1);

    *spinBox = new QDoubleSpinBox(gridLayoutWidget);
    (*spinBox)->setObjectName(QString::fromUtf8(szSpinName));
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

   connect(comboBox,SIGNAL(activated(int)),this,SLOT(processLevel(int)));
}




GraphicalItemPropsDlg::~GraphicalItemPropsDlg()
{

}

bool GraphicalItemPropsDlg::getResult(BOARD_LEVEL_ID &iLevel,
                                      shared_ptr<PointF>& pos,
                                      shared_ptr<GeomCommonProps>& props,
                                      ContainerType& containerType)
{
   pos.reset(new PointF(static_cast<float>(posXSpinBox->value()),
                           static_cast<float>(posYSpinBox->value())));

   //check type of grapfical item
   RectGraphicalItem *rp = nullptr;
   RectPackageGraphicalItem *rpg = nullptr;
   RoundPackageGraphicalItem *rndp = nullptr;
   RoundPlateGraphicalItem *rndplt = nullptr;
   LineGraphicalItem *pln = nullptr;
   CapGraphicalItem *pc = nullptr;
   containerType = ContainerType::NonContainer;
   if((rp = dynamic_cast<RectGraphicalItem*>(m_pItem.get())))
   {
      props.reset(makeRectGeom(static_cast<float>(geomASpinBox->value()),
                                      static_cast<float>(geomBSpinBox->value())));
   }
   else if((rpg = dynamic_cast<RectPackageGraphicalItem*>(m_pItem.get())))
   {
      props.reset(makeRectGeom(static_cast<float>(geomASpinBox->value()),
                               static_cast<float>(geomBSpinBox->value())));
   }
   else if((rndp = dynamic_cast<RoundPackageGraphicalItem*>(m_pItem.get())))
   {
      props.reset(makeRoundShapeGeom(static_cast<float>(geomASpinBox->value())));
   }
   else if((rndplt = dynamic_cast<RoundPlateGraphicalItem*>(m_pItem.get())))
   {
      props.reset(makeRoundPlateGeom(static_cast<float>(geomASpinBox->value()),
                                      static_cast<float>(geomBSpinBox->value())));
   }
   else if((pln = dynamic_cast<LineGraphicalItem*>(m_pItem.get())))
   {
      props.reset(makeLineGeom(static_cast<float>(geomASpinBox->value())));

   }
   else if((pc = dynamic_cast<CapGraphicalItem*>(m_pItem.get())))
   {
      props.reset(makeCapGeom(static_cast<float>(geomASpinBox->value())));
   }

   set<BOARD_LEVEL_ID> idLevels;
   containerType = checkContainerType(m_pItem.get(),idLevels);
   if(containerType == ContainerType::Multiplate)
   {
      vector<SmartPtr<GraphicalItem> > *pItems = static_cast<GenericGraphicalItemsContainer*>
                                                                 (m_pItem.get())->getChildren();
      if(pItems->size() != 2 || idLevels.size() != 2)  //not supported
         return false;

      (*pItems)[0]->setLevel(*(idLevels.begin()));
      (*pItems)[1]->setLevel(*(++idLevels.begin()));
   }
   else if(containerType == ContainerType::RelocatableDipType ||
           containerType == ContainerType::RelocatableSoType ||
           containerType == ContainerType::NonContainer)
   {
       auto level = m_levels.find(comboBox->currentIndex());
       if(level != m_levels.end())
          iLevel = level->second;
   }
   return updPropsChk->checkState() == Qt::CheckState::Checked;
}



void GraphicalItemPropsDlg::processLevel(int )
{
}

void GraphicalItemPropsDlg::setFieldsForRects(float width, float height,
                                              int& rowCounter)
{
    createOneRow(WIDTH_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(width),rowCounter);

    createOneRow(HEIGHT_DEF,"geomBSpinBox",&geomBSpinBox,
                 static_cast<double>(height),rowCounter);
/*
   label_4 = new QLabel(gridLayoutWidget);
   label_4->setObjectName(QString::fromUtf8("label_4"));


   gridLayout->addWidget(label_4, 3, 0, 1, 1);
   
   geomASpinBox = new QDoubleSpinBox(gridLayoutWidget);
   geomASpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_3"));

   gridLayout->addWidget(geomASpinBox, 3, 1, 1, 1);
        
   label_5 = new QLabel(gridLayoutWidget);
   label_5->setObjectName(QString::fromUtf8("label_5"));


   gridLayout->addWidget(label_5, 4, 0, 1, 1);

   geomBSpinBox = new QDoubleSpinBox(gridLayoutWidget);
   geomBSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_4"));

   gridLayout->addWidget(geomBSpinBox, 4, 1, 1, 1);

   
   label_4->setText(tr(WIDTH_DEF));
   oldWidth = static_cast<double>(width);
   geomASpinBox->setValue(oldWidth);
   
   label_5->setText(tr(HEIGHT_DEF));
   oldHeight = static_cast<double>(height);
   geomBSpinBox->setValue(oldHeight);
*/
}

void GraphicalItemPropsDlg::setFieldsForRounds(float d_ex,float d_ex1,
                                               float d_in,int& rowCounter)
{
    createOneRow(D_EX_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(d_ex),rowCounter++);

    if(d_in > 0)
       createOneRow(D_IN_DEF,"geomBSpinBox",&geomBSpinBox,
                 static_cast<double>(d_in),rowCounter++);

/*
   label_4 = new QLabel(gridLayoutWidget);
   label_4->setObjectName(QString::fromUtf8("label_4"));


   gridLayout->addWidget(label_4, 3, 0, 1, 1);
   
   geomASpinBox = new QDoubleSpinBox(gridLayoutWidget);
   geomASpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_3"));

   gridLayout->addWidget(geomASpinBox, 3, 1, 1, 1);
   
   if(d_in > 0)
   {
        
      label_5 = new QLabel(gridLayoutWidget);
      label_5->setObjectName(QString::fromUtf8("label_5"));


      gridLayout->addWidget(label_5, 4, 0, 1, 1);

      geomBSpinBox = new QDoubleSpinBox(gridLayoutWidget);
      geomBSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_4"));

      gridLayout->addWidget(geomBSpinBox, 4, 1, 1, 1);
   }

   
   label_4->setText(D_EX_DEF);
   old_d_ex = static_cast<double>(d_ex);
   geomASpinBox->setValue(old_d_ex);
   old_d_in = static_cast<double>(d_in);
   if(d_in > 0)
     {

      label_5->setText(D_IN_DEF);
      geomBSpinBox->setValue(old_d_in);
   }
*/
}

void GraphicalItemPropsDlg::changedProperties(double )
{
   updPropsChk->setCheckState(Qt::CheckState::Checked);
}

void GraphicalItemPropsDlg::setFieldsForLine(float w,int& rowCounter)
{
    createOneRow(WIDTH_DEF,"geomASpinBox",&geomASpinBox,
                 static_cast<double>(w),rowCounter++);
/*
   label_4 = new QLabel(gridLayoutWidget);
   label_4->setObjectName(QString::fromUtf8("label_4"));


   gridLayout->addWidget(label_4, 3, 0, 1, 1);
   
   geomASpinBox = new QDoubleSpinBox(gridLayoutWidget);
   geomASpinBox->setObjectName(QString::fromUtf8("doubleSpinBox_3"));

   gridLayout->addWidget(geomASpinBox, 3, 1, 1, 1);
        
   label_5 = new QLabel(gridLayoutWidget);
   label_5->setObjectName(QString::fromUtf8("label_5"));
   
   label_4->setText(tr(WIDTH_DEF));
   oldWidth = static_cast<double>(w);
   geomASpinBox->setValue(oldWidth);
*/
   posYSpinBox->setEnabled(false);
   posXSpinBox->setEnabled(false);

}

void GraphicalItemPropsDlg::accept()
{
   if(!comboBox)
   {
      //multiplate
      int iLevelsNumber = 0;
      for(std::vector<QCheckBox*>::size_type i = 0; i < multiplateChks.size();++i)
      {         
         if(multiplateChks[i]->isChecked())
            iLevelsNumber++;
      }
      
       if(iLevelsNumber != 2) //at the moment only double plate
       {
          QMessageBox::critical(this, tr("Error"),
                                 tr("There must be 2 layers checked"));
          return;
       }
   }
   QDialog::accept();
}


ContainerType GraphicalItemPropsDlg::checkContainerType(GraphicalItem *p,set<BOARD_LEVEL_ID>& levelIds)
{
   ContainerType type = ContainerType::NonContainer;
   if(GenericGraphicalItemsContainer *pCont = dynamic_cast<GenericGraphicalItemsContainer*>(p))
   {
      pCont->getLevels(levelIds);
      if(levelIds.size() == 2)
      {
         auto item = std::find_if(levelIds.begin(),levelIds.end(),
                         [](BOARD_LEVEL_ID level)
                            {
                               return LevelsWrapper::isActiveLevelExternal(level);
                            });
         if(item != levelIds.end())
         {
            if(levelIds.find(LevelsWrapper::geLevelForDip(*item)) != levelIds.end())
               type = ContainerType::RelocatableDipType;
            else if(levelIds.find(LevelsWrapper::geLevelForSO(*item)) != levelIds.end())
               type = ContainerType::RelocatableSoType;
         }
      }
      else
      {
         //chaeck for multiplate
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
   }
   return type;
}
