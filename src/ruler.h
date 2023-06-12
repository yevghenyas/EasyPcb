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

#ifndef RULER_H
#define RULER_H

#include <qwidget.h>
#include <QLabel>
#include "common.h"
#include <QAbstractSlider>
#include <QScrollBar>

class Ruler :  public QLabel
{
   Q_OBJECT
   short type;
   QPoint cursorCoord,cursorPrevCoord;
   int k_zoom{1};
   int m_pos{0};
   float m_PcbSize{0.0f};
   QScrollBar *m_p{nullptr};
public:
    enum RulerType{TYPE_VERTICAL,TYPE_HORIZONTAL};
    Ruler(QScrollBar *p,float boardSize,int t = TYPE_VERTICAL);
    QSizePolicy sizePolicy() const;
    virtual QSize sizeHint() const;
    virtual void paintEvent(QPaintEvent*);
    void setDrawerCoordinates(QPoint p);
    void setZoom(int k);
    void setBoardSize(float size);
    ~Ruler();
public slots:
    void setValue(int value);
    void setRange(int min,int max);
};

#endif // RULER_H
