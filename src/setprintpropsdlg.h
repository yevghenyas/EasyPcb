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

#ifndef SETPRINTPROPSDLG_H
#define SETPRINTPROPSDLG_H

#include "common.h"
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QCheckBox>
#include <QDialog>
#include <QFormLayout>
#include <QGridLayout>
#include <QHeaderView>
#include <QListWidget>
#include <QVBoxLayout>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QScrollArea>
#include "pcboard.h"
#include "printpreviewwidget.h"
#include <QDialogButtonBox>


class SetPrintPropsDlg :  public QDialog
{
    Q_OBJECT
    QScrollArea *m_scrollArea;
    PcBoard *m_pBoard;
    PrintPreviewWidget *m_prtWidget;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    QVBoxLayout *verticalLayout;
    QListWidget *listWidget;
    QFormLayout *formLayout_2;
    QFormLayout *formLayout_3;    
    QFormLayout *formLayout_4;        
    vector<QCheckBox*> levelBoxes;
    vector<QCheckBox*> transBoxes;
    QCheckBox *checkBoxMirrow;
    QCheckBox *checkBoxRotate;
    QSpinBox  *spinX;
    QSpinBox  *spinY;
    QDialogButtonBox *buttonBox;
//    QCheckBox *checkBox_2;
//    QCheckBox *checkBox_3;
//    QCheckBox *checkBox_4;

public:
    SetPrintPropsDlg(PcBoard *p);
    ~SetPrintPropsDlg();
    virtual void accept();
public slots:
    void updateLayersStates(int);
    void updateTransStates(int);
    void spinxchanged(int);
    void spinychanged(int);
};

#endif // SETPRINTPROPSDLG_H
