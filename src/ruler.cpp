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

#include "ruler.h"
#include <qevent.h>
#include <QPainter>
#include "common.h"
#include <iostream>


using namespace std;

Ruler::Ruler(QScrollBar *p,float boardSize,int t):type(t),cursorCoord(-1,-1),
    cursorPrevCoord(-1,-1),k_zoom(1),m_p(p),m_PcbSize(boardSize)
{
   setSizePolicy(sizePolicy());

}

QSizePolicy Ruler::sizePolicy() const
{
   if(type == TYPE_VERTICAL)
      return QSizePolicy(QSizePolicy::QSizePolicy::Maximum , QSizePolicy::Expanding );
   else
      return QSizePolicy(QSizePolicy::QSizePolicy::Expanding , QSizePolicy::Maximum );
}

QSize Ruler::sizeHint() const
{
   cout<<"sizeHint"<<endl;
   if(type == TYPE_VERTICAL)
      return QSize(25,50);
   else
      return QSize(50,25);
}


void Ruler::paintEvent(QPaintEvent* e)
{
   QPainter p(this);
   p.eraseRect(e->rect());
   int limit = 0;
   if (type == TYPE_VERTICAL)
      limit = e->rect().height();
   else
      limit = e->rect().width();

   float pos = 0.0f;

   if(m_p)
   {
      float min = m_p->minimum();
      float max = m_p->maximum();
      float step = m_p->pageStep();

      float length = max - min + step;
      //current relative position
      float ratio = static_cast<float>(m_pos)/length;

      pos = ratio * m_PcbSize * k_zoom * PIXELS_PER_MM;
   }


   int relPos = castFloatToInt(pos);
   for(int k = 0; k < limit; ++k)
   {
      int i = k + relPos;
      if(i%10 > 0)
          continue;
      int start = 9;
      //to draw 5mm ticks a bit larger
      if(i%(5*PIXELS_PER_MM * k_zoom) == 0)
         start -= 3;
      //to draw 10mm ticks the largest
      if(i%(10*PIXELS_PER_MM * k_zoom) == 0)         
         start = 0;
      //draw ticks
      if(type == TYPE_VERTICAL)
         p.drawLine(start,k,e->rect().width(),k);
      else
         p.drawLine(k,start,k,e->rect().height());
      //draw text
      if(start == 0)
      {
         char buf[16];
         if(!start)
         {
            sprintf(buf,"%d",i/100/k_zoom);
            p.setFont(QFont( "Arial", 8, QFont::AllLowercase));
            if(type == TYPE_VERTICAL)
               p.drawText(3,k+10,buf);
            else
               p.drawText(k+3,14,buf);
         }
      }
   }
   if(cursorCoord.x() != -1 && cursorCoord.y() != -1)
   {
      QPen pen;
      pen.setColor(Qt::red);
      pen.setWidth(2);
      p.setPen(pen);
      if(type == TYPE_VERTICAL)
         p.drawLine(9,cursorCoord.y() - relPos,e->rect().width(),cursorCoord.y() - relPos);
      else
         p.drawLine(cursorCoord.x() - relPos,9,cursorCoord.x() - relPos,e->rect().height());
   }
}

void Ruler::setDrawerCoordinates(QPoint p)
{
   if(cursorCoord != p)
   {
      cursorPrevCoord = cursorCoord;
      cursorCoord = p;
//      cout<<"setting coordinates"<<endl;    
      repaint();
   }
}

void Ruler::setZoom(int k)
{
   k_zoom = k;
   repaint();
}


Ruler::~Ruler()
{

}

void Ruler::setBoardSize(float size)
{
   m_PcbSize = size;
}

void Ruler::setValue(int value)
{
   m_pos = value;
   repaint();
}

void Ruler::setRange(int min,int max)
{
   repaint();
}


//#include "ruler.moc"
