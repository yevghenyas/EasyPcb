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

#include "setprintpropsdlg.h"
#include <QPrinter>
#include <QPrintDialog>


SetPrintPropsDlg::SetPrintPropsDlg(PcBoard *p):m_pBoard(p)
{
   if (objectName().isEmpty())
      setObjectName(QString::fromUtf8("Dialog"));
   resize(862, 650);
   gridLayoutWidget = new QWidget(this);
   gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
   gridLayoutWidget->setGeometry(QRect(1, 0, 861, 611));
   gridLayout = new QGridLayout(gridLayoutWidget);
   gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
   gridLayout->setContentsMargins(0, 0, 0, 0);
   verticalLayout = new QVBoxLayout();
   verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));

   gridLayout->addLayout(verticalLayout, 1,0);

   

   vector<BoardLevel> v;  
   LevelsWrapper::getAvalableLevels(v);

   vector<bool> v_states;
   
   //Layers
   formLayout_2 = new QFormLayout();
   formLayout_2->setObjectName(QString::fromUtf8("formLayout_2"));
   formLayout_2->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
   
   for(size_t i = 0; i < v.size(); ++i)
   {
      QCheckBox *p = new QCheckBox(gridLayoutWidget);
      p->setObjectName(QString::fromUtf8(v[i].desc));
      p->setText(v[i].desc);
      p->setCheckState(Qt::Checked);
      const QVariant layerId(v[i].idLevel);
      p->setProperty("layer_id",layerId);
      formLayout_2->setWidget(i, QFormLayout::LabelRole, p);      
      levelBoxes.push_back(p);
      v_states.push_back(true);
      QObject::connect(p, SIGNAL(stateChanged(int)), this, SLOT(updateLayersStates(int)));      
   }
   
   m_prtWidget = new PrintPreviewWidget(gridLayoutWidget,m_pBoard->getBoardLayers(),v_states,
                                        m_pBoard->getWidth(),m_pBoard->getHeight());
   m_prtWidget->setObjectName(QString::fromUtf8("listWidget")); 
   m_scrollArea = new QScrollArea(this);
   m_scrollArea->setWidget(m_prtWidget);
   
   gridLayout->addWidget(m_scrollArea, 1, 1);
   
   
   QLabel *label1 = new QLabel();
   label1->setObjectName(QString::fromUtf8("label1*"));
   label1->setText(tr(SEL_LAYERS));
   label1->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
   verticalLayout->addWidget(label1);
   verticalLayout->addLayout(formLayout_2);
   
   //Transformations
   
   QLabel *label2 = new QLabel();
   label2->setObjectName(QString::fromUtf8("label2*"));
   label2->setText(tr(SEL_TR));
   label2->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
   verticalLayout->addWidget(label2);

   formLayout_3 = new QFormLayout();
   formLayout_3->setObjectName(QString::fromUtf8("formLayout_3"));
   formLayout_3->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
   
   vector<TransInfo> v_trans;
   getAvailableTransformations(v_trans);
   
   for(size_t i = 0; i < v_trans.size(); ++i)
   {
      QCheckBox *pChk = new QCheckBox(gridLayoutWidget);
      pChk->setObjectName(QString::fromUtf8(v_trans[i].desc));
      pChk->setText(tr(v_trans[i].desc));
      formLayout_3->setWidget(static_cast<int>(i), QFormLayout::LabelRole, pChk);
      transBoxes.push_back(pChk);
      QObject::connect(pChk, SIGNAL(stateChanged(int)), this, SLOT(updateTransStates(int)));
   }
   
   /*
   
   checkBoxMirrow = new QCheckBox(gridLayoutWidget);
   checkBoxMirrow->setObjectName(QString::fromUtf8("checkBoxMirrow"));
   checkBoxMirrow->setText(tr(MIRROW_HOR_STRING));

   formLayout_3->setWidget(0, QFormLayout::LabelRole, checkBoxMirrow);

   checkBoxRotate = new QCheckBox(gridLayoutWidget);
   checkBoxRotate->setObjectName(QString::fromUtf8("checkBoxRotat   QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));e"));
   checkBoxRotate->setText(tr(ROTATE_STRING));
   
   formLayout_3->setWidget(1, QFormLayout::LabelRole, checkBoxRotate);
   
*/
   
   verticalLayout->addLayout(formLayout_3);    
   
   //Number of copies
   
   formLayout_4 = new QFormLayout();
   formLayout_4->setObjectName(QString::fromUtf8("formLayout_4"));
   formLayout_4->setFieldGrowthPolicy(QFormLayout::ExpandingFieldsGrow);
   formLayout_4->setContentsMargins(5,10,10,10);
   
   QLabel *label3 = new QLabel();
   label3->setObjectName(QString::fromUtf8("label3*"));
   label3->setText(tr(NUM_OF_COPIES));
   label3->setSizePolicy(QSizePolicy::Minimum,QSizePolicy::Fixed);
   verticalLayout->addWidget(label3);
   
   spinX = new QSpinBox(gridLayoutWidget);
   spinX->setObjectName(QString::fromUtf8("spinX"));

   formLayout_4->addRow(tr(X_DEF), spinX);
   spinX->setMinimum(1);

   spinY = new QSpinBox(gridLayoutWidget);
   spinY->setObjectName(QString::fromUtf8("spinY"));
   spinY->setMinimum(1);

   formLayout_4->addRow(tr(Y_DEF), spinY);
   
   verticalLayout->addLayout(formLayout_4);   
   
   buttonBox = new QDialogButtonBox(gridLayoutWidget);
   buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
   buttonBox->setGeometry(QRect(310, 10, 81, 131));
   buttonBox->setOrientation(Qt::Vertical);
   buttonBox->addButton(tr(PRINT_DEF),QDialogButtonBox::AcceptRole);
//   buttonBox->addButton(tr(PRINTER_PROPS_DEF),QDialogButtonBox::ActionRole);
   buttonBox->addButton(QDialogButtonBox::Cancel);
//   buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

   gridLayout->addWidget(buttonBox,1,2);
   
   gridLayoutWidget->raise();

   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
   connect(spinX,SIGNAL(valueChanged(int)),this,SLOT(spinxchanged(int)));
   connect(spinY,SIGNAL(valueChanged(int)),this,SLOT(spinychanged(int)));
   
//   setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UnicodeUTF8));

   QMetaObject::connectSlotsByName(this);
}

void SetPrintPropsDlg::accept()
{
   QPrinter printer(QPrinter::HighResolution);
   QPrintDialog printDialog(&printer);
   if (printDialog.exec() == QDialog::Accepted) 
   {
      QPainter painter;
      painter.begin(&printer);
      //scale it in accordance with resolution
      //ratio = dots_per_inch / PIXELS_PER_MM
      auto ratio = printer.resolution()/(25.4 * PIXELS_PER_MM);
//      ratio /= 0.95;
      for(int j = 0; j < spinY->value() ; ++j)
      {
         for(int i = 0; i < spinX->value() ; ++i)
         {
            painter.save();
            if(transBoxes[TR_MIRROW]->checkState() == Qt::Checked)
            {
               painter.scale(-ratio,ratio);
               painter.translate(-m_pBoard->getWidth() * (i + 1),m_pBoard->getHeight() * j);              
            }
            else
            {
               painter.scale(ratio,ratio);
            }
            if(transBoxes[TR_ROTATE]->checkState() == Qt::Checked)
            {
               //wee need to update translation to set the center of rotation
               if(transBoxes[TR_MIRROW]->checkState() == Qt::Checked)
               {
                  painter.translate(m_pBoard->getWidth() + m_pBoard->getWidth()/4 * i,m_pBoard->getHeight() * 43/128 * j);                        
               }
               else
               {
                  painter.translate(m_pBoard->getHeight() * (i + 1),m_pBoard->getWidth() * j);                        
               }
               painter.rotate(90.0);
            }
            if(transBoxes[TR_ROTATE]->checkState() != Qt::Checked && 
               transBoxes[TR_MIRROW]->checkState() != Qt::Checked)
               painter.translate(m_pBoard->getWidth() * i,m_pBoard->getHeight() * j); 
           
            painter.drawRect(0,0,m_pBoard->getWidth() ,m_pBoard->getHeight());

            for(size_t k = 0; k < levelBoxes.size(); ++k)
               if(levelBoxes[k]->checkState() == Qt::Checked)
               {
                  const QVariant layerId = levelBoxes[k]->property("layer_id");
                  auto pLayer = m_pBoard->getBoardLayers()->getLayer(static_cast<BOARD_LEVEL_ID>(layerId.toInt()));
                  pLayer->paintItems(painter,MODE_PRINT,1);
               }
            painter.restore();         
         }
      }
      painter.end();    
   }
   QDialog::accept();
}


SetPrintPropsDlg::~SetPrintPropsDlg()
{

}
void SetPrintPropsDlg::updateLayersStates(int)
{
   vector<bool> v;
   for(auto item:levelBoxes)
   {
      v.push_back(item->checkState() == Qt::Checked);
   }
   m_prtWidget->updateLayersState(v);
}

void SetPrintPropsDlg::updateTransStates(int)
{
   vector<bool> v;
   for(auto item:transBoxes)
   {
      if(item->checkState() == Qt::Checked)
         cout<<"Rotating was checked"<<endl;
      v.push_back(item->checkState() == Qt::Checked);
   }
   m_prtWidget->updateTransState(v);

}

void SetPrintPropsDlg::spinxchanged(int)
{
   m_prtWidget->updateNumOfCopies(spinX->value(),spinY->value());
}

void SetPrintPropsDlg::spinychanged(int)
{
   m_prtWidget->updateNumOfCopies(spinX->value(),spinY->value());
}


//#include "setprintpropsdlg.moc"
