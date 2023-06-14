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

#ifndef PRINTPREVIEWWIDGET_H
#define PRINTPREVIEWWIDGET_H

#include "common.h"
#include <QLabel>
#include "pcboard.h"
#include "boardlayerswrapper.h"

class PrintPreviewWidget :  public QLabel
{
   BoardLayersWrapper* m_pLayers;
   vector<bool> m_states;
   vector<bool> m_transformStates;
   vector<BoardLevel> m_availLayers;
   int m_nX;
   int m_nY;
   int m_iBoardWidth;
   int m_iBoardHeight;
protected:
    QSizePolicy sizePolicy() const;
public:
    PrintPreviewWidget(QWidget *parent,BoardLayersWrapper *m_pLayers,
                       vector<bool> states,int iBoardWidth,int iBoardHeight);
    ~PrintPreviewWidget();
    virtual void paintEvent(QPaintEvent*);
    void updateLayersState(vector<bool>& states);
    void updateTransState(vector<bool>& states);
    void updateNumOfCopies(int x,int y);
};

#endif // PRINTPREVIEWWIDGET_H
