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

#include "platedata.h"
#include "common.h"
#include <cmath>
#include <QPainterPath>
#include <cstdio>
#include <iostream>
using namespace std;


plateData::plateData(int p1,int p2,int p3,int p4,int p5,int p6):type(p1),x(p2),y(p3),w(p4),h(p5),d(p6),m_selected(false)
{

}


plateData::plateData(const plateData& other)
{
   type = other.type;
   x = other.x;
   y = other.y;
   w = other.w;
   h = other.h;
   d = other.d;
   m_selected = other.m_selected;
}

plateData& plateData::operator*=(int rhs)
{
   x *= rhs; 
   y *= rhs; 
   w *= rhs; 
   h *= rhs; 
   d *= rhs; 
   return *this;
}

bool plateData::isPointIn(int xx, int yy)
{
  char buf[128];
  int dif_x = x - xx;
  int dif_y = y - yy;
  sprintf(buf,"dif_x=%d dif_y=%d sum=%d",dif_x,dif_y,dif_x * dif_x + dif_y * dif_y);
  cout<<buf<<endl;  
  int dd = sqrt(dif_x * dif_x + dif_y * dif_y);
  sprintf(buf,"x=%d y=%d xx=%d yy=%d",x,y,xx,yy);
  cout<<buf<<endl;
  sprintf(buf,"dd=%d w/2=%d",dd,w/2);
  cout<<buf<<endl;
  return dd <= w/2;
}

void plateData::paintItem(QPainter& p,QColor& c)
{
   QPainterPath OuterPath;
   OuterPath.addEllipse(QPointF(x,y ),w/2,h/2);
   QPainterPath InnerPath;
   InnerPath.addEllipse(QPointF(x,y), d/2, d/2);
   QPainterPath FillPath = OuterPath.subtracted(InnerPath);
   p.fillPath(FillPath, c);

}


bool plateData::isSelected()
{
   return m_selected;
}

void plateData::setSelected(bool select)
{
   m_selected = select;
}


