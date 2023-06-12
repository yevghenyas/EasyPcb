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

#ifndef PCBOARDPROPSDLG_H
#define PCBOARDPROPSDLG_H

#include <QDialog>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QComboBox>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include "pcboard.h"


class PcBoardPropsDlg :  public QDialog
{
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QLabel *label;
    QDoubleSpinBox *doubleSpinBox_2;
    QLabel *label_2;
    QDoubleSpinBox *doubleSpinBox;
    PcBoard *m_board;
public:
    PcBoardPropsDlg(PcBoard *p,int scaleFactor);
    ~PcBoardPropsDlg();
    bool getResult(int& w,int&h);
};

#endif // PCBOARDPROPSDLG_H
