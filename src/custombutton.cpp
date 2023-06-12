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

#include "custombutton.h"
#include <QPaintEvent>
#include <QPainter>
#include "drawwrapper.h"

CustomButton::CustomButton(int type,const QString& name,QWidget *parent):QLabel(name,parent),m_type(type)
{

}



CustomButton::~CustomButton()
{

}

QSize CustomButton::sizeHint() const
{
      return QSize(60,60);
//    return QLabel::sizeHint();
}


void CustomButton::paintEvent(QPaintEvent* e)
{ 
   QLabel::paintEvent(e);
   QPainter p( this );
   QColor c(Qt::black);
   int width = this->geometry().width();
   int height = this->geometry().height();
   if(m_type == TYPE_LINE)
   {
      p.drawLine(0,height/2,width,height/2);
      
   }
   else
   {
      if(m_type == TYPE_PLATE_EXT)
      {
         p.drawLine(width/2 - height/2 - height/4,height/2,width/2 + height/2 + height/4,height/2);
         DrawWrapper::draWArrow(p,c,width/2 - height/2 - height/2,height/2,height/2,O_HORIZONTAL_RIGHT);
         DrawWrapper::draWArrow(p,c,width/2 + height/2 + height/2,height/2,height/2,O_HORIZONTAL_LEFT);	
         DrawWrapper::drawPlate(p,c,width/2,height/2,height,height/2,1,1);         
      }
      else if(m_type == TYPE_PLATE_INTERN)
      {
         p.drawLine(width/2,height/4,width/2 + height,height/4);
         p.drawLine(width/2,height/4 + height/2 ,width/2 + height,height/4 + height/2);
         DrawWrapper::draWArrow(p,c,width/2 + 3*height/4,0,height/4,O_VERTICAL_BOTTOM);
         DrawWrapper::draWArrow(p,c,width/2 + 3*height/4,height,height/4,O_VERTICAL_TOP);
         DrawWrapper::drawPlate(p,c,width/2,height/2,height,height/2,1,1);
      }
      else if(m_type == TYPE_PLATE_RECT_H)
      {
         DrawWrapper::drawRectPlate(p,c,width/2,height/2,height/2,height/2);
         p.drawLine(width/2 + height/4,height/4 + 1,width/2 + height,height/4 + 1 );
         p.drawLine(width/2 + height/4,3 * height/4 - 1,width/2 + height,3 * height/4 - 1 );         
         DrawWrapper::draWArrow(p,c,width/2 + 3*height/4,0,height/4,O_VERTICAL_BOTTOM);
         DrawWrapper::draWArrow(p,c,width/2 + 3*height/4,height,height/4,O_VERTICAL_TOP);
         
      }
      else if(m_type == TYPE_PLATE_RECT_W)
      {
         DrawWrapper::drawRectPlate(p,c,width/2,height/2,height/2,height/2);
         p.drawLine(width/2 - height/4,0,width/2 - height/4,height/4);
         p.drawLine(width/2 + height/4,0,width/2 + height/4,height/4);         
         DrawWrapper::draWArrow(p,c,width/2 - height/2,height/8,height/4,O_HORIZONTAL_RIGHT);
         DrawWrapper::draWArrow(p,c,width/2 + height/2,height/8,height/4,O_HORIZONTAL_LEFT);         
      }
      
   }
   
}
