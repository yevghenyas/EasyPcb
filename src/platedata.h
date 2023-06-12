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

#ifndef PLATEDATA_H
#define PLATEDATA_H
#include "graphicalitem.h"

class plateData : public GraphicalItem
{
  bool m_selected;
  public:
  enum PlateType{TYPE_ROUND,TYPE_RECT};
  //round,rectangular
  int type;
  // coordinates ,size 
  int x,y,w,h;
  //diameter of the hole
  int d;
    plateData(int p1,int p2,int p3,int p4,int p5,int p6);
    plateData(const plateData& other);
    plateData& operator*=(int rhs);
    bool isPointIn(int x,int y);
    void paintItem(QPainter& p,QColor& c);
    bool isSelected();
    void setSelected(bool select);
    virtual void zoom(int k){};
    virtual void zoomIn(){};
    virtual void zoomOut(){};
};

#endif // PLATEDATA_H
