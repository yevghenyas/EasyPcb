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

#include "pcboardpropsdlg.h"
#include "common.h"

PcBoardPropsDlg::PcBoardPropsDlg(PcBoard *p,int scaleFactor):m_board(p)
{
   if (objectName().isEmpty())
      setObjectName(QString::fromUtf8("Dialog"));
   resize(420, 200);
   buttonBox = new QDialogButtonBox(this);
   buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
   buttonBox->setGeometry(QRect(310, 10, 81, 131));
   buttonBox->setOrientation(Qt::Vertical);
   buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
   gridLayoutWidget = new QWidget(this);
   gridLayoutWidget->setObjectName(QString::fromUtf8("gridLayoutWidget"));
   gridLayoutWidget->setGeometry(QRect(9, 10, 251, 121));
   gridLayout = new QGridLayout(gridLayoutWidget);
   
   label = new QLabel(gridLayoutWidget);
   label->setObjectName(QString::fromUtf8("label"));
   label->setText(QString::fromUtf8(WIDTH_DEF));

   gridLayout->addWidget(label, 0, 0, 1, 1);

   doubleSpinBox = new QDoubleSpinBox(gridLayoutWidget);
   doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
   doubleSpinBox->setMaximum(static_cast<double>(fMaxBoardWidth));
   doubleSpinBox->setValue(static_cast<double>(p->getWidth())/PIXELS_PER_MM/scaleFactor);

   gridLayout->addWidget(doubleSpinBox, 0, 1, 1, 1);

   label_2 = new QLabel(gridLayoutWidget);
   label_2->setObjectName(QString::fromUtf8("label_2"));
   label_2->setText(QString::fromUtf8(HEIGHT_DEF));

   gridLayout->addWidget(label_2, 1, 0, 1, 1);

   doubleSpinBox_2 = new QDoubleSpinBox(gridLayoutWidget);
   doubleSpinBox_2->setObjectName(QString::fromUtf8("doubleSpinBox_2"));
   doubleSpinBox_2->setMaximum(150.00);
   doubleSpinBox_2->setValue(static_cast<double>(p->getHeight())/PIXELS_PER_MM/scaleFactor);

   gridLayout->addWidget(doubleSpinBox_2, 1, 1, 1, 1);
   
   connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
   

}

bool PcBoardPropsDlg::getResult(int& w,int& h)
{
   w = castFloatToInt(static_cast<float>(doubleSpinBox->value()) * PIXELS_PER_MM);
   h = castFloatToInt(static_cast<float>(doubleSpinBox_2->value()) * PIXELS_PER_MM);
}


PcBoardPropsDlg::~PcBoardPropsDlg()
{

}
