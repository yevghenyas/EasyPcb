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

#include "drawwrapper.h"
#include <iostream>

DrawWrapper::DrawWrapper()
{

}


void DrawWrapper::drawRectPackage(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                              int x,int y,int width,int height,int zoom,int zoom_d)
{
   QPen pen;       
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom/zoom_d);
   p.setPen(pen);   
   if(type == O_HORIZONTAL_LEFT || type == O_HORIZONTAL_RIGHT)
      p.drawRect((x - width/2) * zoom/zoom_d,(y - height/2) * zoom/zoom_d,
               (width - PACK_LINE_DEF_W) * zoom/zoom_d,(height - PACK_LINE_DEF_W) * zoom/zoom_d);      
   else
      p.drawRect((x - height/2) * zoom/zoom_d,(y - width/2) * zoom/zoom_d,
               (height - PACK_LINE_DEF_W) * zoom/zoom_d,(width - PACK_LINE_DEF_W) * zoom/zoom_d);      
      
}

void DrawWrapper::drawFilledRectPackage(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                              int x,int y,int width,int height,int zoom,int zoom_d)
{
   if(type == O_HORIZONTAL_LEFT || type == O_HORIZONTAL_RIGHT)
      p.fillRect((x - width/2) * zoom/zoom_d,(y - height/2) * zoom/zoom_d,
               (width - PACK_LINE_DEF_W) * zoom/zoom_d,(height - PACK_LINE_DEF_W) * zoom/zoom_d,c);
   else
      p.drawRect((x - height/2) * zoom/zoom_d,(y - width/2) * zoom/zoom_d,
               (height - PACK_LINE_DEF_W) * zoom/zoom_d,(width - PACK_LINE_DEF_W) * zoom/zoom_d);

}


void DrawWrapper::drawRectPackageF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                              float x,float y,float width,float height,int zoom,int zoom_d)
{
   QPen pen;
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * 2);
   p.setPen(pen);
   if(type == O_HORIZONTAL_LEFT || type == O_HORIZONTAL_RIGHT)
   {
      int xi = static_cast<int>(PIXELS_PER_MM * (x - width/2) * zoom/zoom_d);
      int yi = static_cast<int>( PIXELS_PER_MM * (y - height/2) * zoom/zoom_d);
      int wi = static_cast<int>(PIXELS_PER_MM * (width - pklnwf) * zoom/zoom_d);
      int hi = static_cast<int>(PIXELS_PER_MM * (height - pklnwf) * zoom/zoom_d);
      p.drawRect(xi,yi,wi,hi);
   }
   else
   {
      int xi = static_cast<int>(PIXELS_PER_MM * (x - height/2) * zoom/zoom_d);
      int yi = static_cast<int>(PIXELS_PER_MM * (y - width/2) * zoom/zoom_d);
      int wi = static_cast<int>(PIXELS_PER_MM * (height - pklnwf) * zoom/zoom_d);
      int hi = static_cast<int>(PIXELS_PER_MM * (width - pklnwf) * zoom/zoom_d);

      p.drawRect(xi,yi,wi,hi);
   }

}

void DrawWrapper::drawFilledRectPackageF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                              float x,float y,float width,float height,int zoom,int zoom_d)
{
   if(type == O_HORIZONTAL_LEFT || type == O_HORIZONTAL_RIGHT)
   {
      int xi = static_cast<int>(PIXELS_PER_MM * (x - width/2) * zoom/zoom_d);
      int yi = static_cast<int>( PIXELS_PER_MM * (y - height/2) * zoom/zoom_d);
      int wi = static_cast<int>(PIXELS_PER_MM * (width - pklnwf) * zoom/zoom_d);
      int hi = static_cast<int>(PIXELS_PER_MM * (height - pklnwf) * zoom/zoom_d);
      p.fillRect(xi,yi,wi,hi,c);
   }
   else
   {
      int xi = static_cast<int>(PIXELS_PER_MM * (x - height/2) * zoom/zoom_d);
      int yi = static_cast<int>(PIXELS_PER_MM * (y - width/2) * zoom/zoom_d);
      int wi = static_cast<int>(PIXELS_PER_MM * (height - pklnwf) * zoom/zoom_d);
      int hi = static_cast<int>(PIXELS_PER_MM * (width - pklnwf) * zoom/zoom_d);\
      p.fillRect(xi,yi,wi,hi,c);
   }

}


void DrawWrapper::drawPackege(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                              int x,int y,int width,int height,int zoom,int zoom_d)
{
   QPen pen;       
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom/zoom_d);
   p.setPen(pen);   
   if(type == O_VERTICAL_TOP || type == O_VERTICAL_BOTTOM)
      p.drawRect((x - width/2) * zoom/zoom_d,(y - height/2) * zoom/zoom_d,
               (width - PACK_LINE_DEF_W) * zoom/zoom_d,(height - PACK_LINE_DEF_W) * zoom/zoom_d);      
   else
      p.drawRect((x - height/2) * zoom/zoom_d,(y - width/2) * zoom/zoom_d,
               (height - PACK_LINE_DEF_W) * zoom/zoom_d,(width - PACK_LINE_DEF_W) * zoom/zoom_d);      
      
   switch(type)
   {
      case O_VERTICAL_TOP:
      {
         int xx = (x - width/2) * zoom/zoom_d;
         int yy = (y - height/2) * zoom/zoom_d;
         p.drawRect(xx + width/4 * zoom/zoom_d,yy,width/2 * zoom/zoom_d,PIXELS_PER_MM * zoom/zoom_d);            
         break;
      }
      case O_VERTICAL_BOTTOM:
      {
         int xx = (x - width/2) * zoom/zoom_d;
         int yy = (y + height/2 - PACK_LINE_DEF_W) * zoom/zoom_d;            
         p.drawRect(xx + width/4 * zoom/zoom_d,yy - PIXELS_PER_MM * zoom/zoom_d,width/2 * zoom/zoom_d,PIXELS_PER_MM * zoom/zoom_d);
         break;
      }              
      case O_HORIZONTAL_LEFT:
      {
         int xx = (x - height/2) * zoom/zoom_d;
         int yy = (y - width/2) * zoom/zoom_d;                        
         p.drawRect(xx ,yy + width/4  * zoom/zoom_d,PIXELS_PER_MM * zoom/zoom_d,width/2 * zoom/zoom_d);
         break;
      }
      case O_HORIZONTAL_RIGHT:
      {
         int xx = (x + height/2 - PACK_LINE_DEF_W) * zoom/zoom_d;
         int yy = (y - width/2) * zoom/zoom_d;                        
         p.drawRect(xx - PIXELS_PER_MM * zoom/zoom_d,yy + width/4 * zoom/zoom_d,PIXELS_PER_MM * zoom/zoom_d,width/2 * zoom/zoom_d);
         break;
      }
      default:
         break;
   }
}

void DrawWrapper::drawPackageF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                              float x,float y,float width,float height,int zoom,int zoom_d)
{
   QPen pen;
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom/zoom_d);
   p.setPen(pen);
   if(type == O_VERTICAL_TOP || type == O_VERTICAL_BOTTOM)
      p.drawRect(castFloatToIntPxls((x - width/2) * zoom/zoom_d),
                 castFloatToIntPxls((y - height/2) * zoom/zoom_d),
                 castFloatToIntPxls((width - pklnwf) * zoom/zoom_d),
                 castFloatToIntPxls((height - pklnwf) * zoom/zoom_d));
   else
      p.drawRect(castFloatToIntPxls((x - height/2) * zoom/zoom_d),
                 castFloatToIntPxls((y - width/2) * zoom/zoom_d),
                 castFloatToIntPxls((height - pklnwf) * zoom/zoom_d),
                 castFloatToIntPxls((width - pklnwf) * zoom/zoom_d)) ;

   switch(type)
   {
      case O_VERTICAL_TOP:
      {
         float xx = (x - width/2) * zoom/zoom_d;
         float yy = (y - height/2) * zoom/zoom_d;
         p.drawRect(castFloatToIntPxls(xx + width/4 * zoom/zoom_d),
                    castFloatToIntPxls(yy),
                    castFloatToIntPxls(width/2 * zoom/zoom_d),
                    PIXELS_PER_MM * zoom/zoom_d);
         break;
      }
      case O_VERTICAL_BOTTOM:
      {
         float xx = (x - width/2) * zoom/zoom_d;
         float yy = (y + height/2 - pklnwf ) * zoom/zoom_d;
         p.drawRect(castFloatToIntPxls(xx + width/4 * zoom/zoom_d),
                    castFloatToIntPxls(yy - 1.0f * zoom/zoom_d),
                    castFloatToIntPxls(width/2 * zoom/zoom_d),
                    PIXELS_PER_MM * zoom/zoom_d);
         break;
      }
      case O_HORIZONTAL_LEFT:
      {
         float xx = (x - height/2) * zoom/zoom_d;
         float yy = (y - width/2) * zoom/zoom_d;
         p.drawRect(castFloatToIntPxls(xx) ,
                    castFloatToIntPxls(yy + width/4  * zoom/zoom_d),
                    PIXELS_PER_MM * zoom/zoom_d,
                    castFloatToIntPxls(width/2 * zoom/zoom_d));
         break;
      }
      case O_HORIZONTAL_RIGHT:
      {
         float xx = (x + height/2 - pklnwf) * zoom/zoom_d;
         float yy = (y - width/2) * zoom/zoom_d;
         p.drawRect(castFloatToIntPxls(xx - 1.0f * zoom/zoom_d),
                    castFloatToIntPxls(yy + width/4 * zoom/zoom_d),
                    PIXELS_PER_MM * zoom/zoom_d,
                    castFloatToIntPxls(width/2 * zoom/zoom_d));
         break;
      }
   }
}


//(x,y) - center dD -external diameter,d- internal diameter
void DrawWrapper::drawPlate(QPainter& p, QColor& c, int x, int y, int dD, int d,int zoom,int zoom_d)
{
   QPainterPath OuterPath;
   QPoint point(x * zoom/zoom_d,y * zoom/zoom_d);
   OuterPath.addEllipse(point,dD/2 * zoom/zoom_d,dD/2 * zoom/zoom_d);
   QPainterPath InnerPath;
   InnerPath.addEllipse(point, d/2 * zoom/zoom_d, d/2 * zoom/zoom_d);
   QColor c1(80,80,80);
   p.fillPath(InnerPath,p.background().color());
   QPainterPath FillPath = OuterPath.subtracted(InnerPath);
   p.fillPath(FillPath,c)  ;

}

//(x,y) - center dD -external diameter,d- internal diameter
void DrawWrapper::drawPlateF(QPainter& p, QColor& c, float x, float y, float dD, float d,int zoom,int zoom_d)
{
   QPainterPath OuterPath;
   QPoint point(castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d),
                castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d));
   OuterPath.addEllipse(point,castFloatToInt(PIXELS_PER_MM * dD/2 * zoom/zoom_d),
                              castFloatToInt(PIXELS_PER_MM * dD/2 * zoom/zoom_d));
   QPainterPath InnerPath;
   InnerPath.addEllipse(point, castFloatToInt(PIXELS_PER_MM * d/2 * zoom/zoom_d),
                        castFloatToInt(PIXELS_PER_MM *d/2 * zoom/zoom_d));
   p.fillPath(InnerPath,p.background().color());
   QPainterPath FillPath = OuterPath.subtracted(InnerPath);
   p.fillPath(FillPath,c)  ;
}



void DrawWrapper::drawDip(QPainter& p,QColor& c, ITEMS_ORIENTATION type, int x, int y, int n, int width, int zoom,int zoom_d)
{
   if (width == 0)
      width = getWidthForDipPack(n);
   int height = getHeightForDipPack(n);
   QPen pen;       
   pen.setColor(c);
   p.setPen(pen);
   drawPackege(p,c,type,x,y,width,height,zoom,zoom_d);
   
   int D = CHIP_PLATE_D_EX * PIXELS_PER_MM;
   int d = CHIP_PLATE_D_IN * PIXELS_PER_MM;
   for(int j = 0; j < 2; ++j)
   {
      if(type == O_VERTICAL_BOTTOM || type == O_VERTICAL_TOP)
      {
         int xx = (x - width/2 - D/2 - PACK_LINE_DEF_W) * zoom/zoom_d + j * (D + width + PACK_LINE_DEF_W) * zoom/zoom_d;
         for(int i = 0; i < n/2 ; ++i)
         {
//            int yy = (y - 2.5 * PIXELS_PER_MM * n/4) + (2.5 * PIXELS_PER_MM/2 + i * 2.5 * PIXELS_PER_MM);
            int yy = (y - height/2 + 2.5 * PIXELS_PER_MM/2) * zoom/zoom_d + i * PIXELS_PER_MM * 2.5 * zoom/zoom_d;
            drawPlate(p,c,xx,yy,D,d,zoom,zoom_d);     
         }
      }
      else
      {
         int yy = (y - width/2 - D/2 - PACK_LINE_DEF_W) * zoom/zoom_d + j * (D + width + PACK_LINE_DEF_W) * zoom/zoom_d;
         for(int i = 0; i < n/2 ; ++i)
         {
//            int xx = (x - 2.5 * PIXELS_PER_MM * n/4) + (2.5 * PIXELS_PER_MM/2 + i * 2.5 * PIXELS_PER_MM);
            int xx = (x - height/2 + 2.5 * PIXELS_PER_MM/2) * zoom/zoom_d + i * PIXELS_PER_MM * 2.5 * zoom/zoom_d;
            drawPlate(p,c,xx,yy,D,d,zoom,zoom_d);     
         }         
      }
   }

}

void DrawWrapper::drawRoundPackage(QPainter& p, QColor& c, int x, int y, int dD,int dd, int zoom,int zoom_d)
{
   QPen pen;       
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom/zoom_d);
   p.setPen(pen);
   p.drawEllipse(QPoint(x * zoom/zoom_d,y * zoom/zoom_d),dD/2 * zoom/zoom_d,dd/2 * zoom/zoom_d);
}


void DrawWrapper::drawRoundPackageF(QPainter& p, QColor& c, float x, float y, float rx, float ry, int zoom,int zoom_d)
{
   QPen pen;
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom/zoom_d);
   p.setPen(pen);
   p.drawEllipse(QPoint(castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d),
                        castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d)),
                        castFloatToInt(PIXELS_PER_MM * rx * zoom/zoom_d),
                        castFloatToInt(PIXELS_PER_MM * ry * zoom/zoom_d));
}


void DrawWrapper::drawArchedPackageF(QPainter& p, QColor& c, float x, float y, float d,int stAngle,
                                     int spAngle,bool closed,int zoom,int zoom_d)
{
   QPen pen;
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom/zoom_d);
   p.setPen(pen);
   int xi = castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d);
   int yi = castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d);
   int di_2 = castFloatToInt(PIXELS_PER_MM * d/2 * zoom/zoom_d);
   QRect rectangle(xi - di_2,
                   yi - di_2,
                   di_2 * 2,
                   di_2 * 2);
   stAngle *= 16;
   spAngle *= 16;
   if(closed)
   {
      p.drawChord(rectangle,stAngle,spAngle);
   }
   else
   {
      p.drawArc(rectangle,stAngle,spAngle);
   }
}


void DrawWrapper::drawRectPlate(QPainter& p, QColor& c, int x, int y, int w, int h, int zoom,int zoom_d)
{
   QRect rect((x - w/2) * zoom/zoom_d,(y - h/2) * zoom/zoom_d,w * zoom/zoom_d,h * zoom/zoom_d);
   p.fillRect(rect,c);
}

void DrawWrapper::drawRectPlateF(QPainter& p, QColor& c, ITEMS_ORIENTATION type, float x, float y, float w, float h, int zoom,int zoom_d)
{
   if(type == O_VERTICAL_TOP || type == O_VERTICAL_BOTTOM)
   {
      QRect rect(static_cast<int>(PIXELS_PER_MM * (x - w/2) * zoom/zoom_d),
                 static_cast<int>(PIXELS_PER_MM * (y - h/2) * zoom/zoom_d),
                 static_cast<int>(PIXELS_PER_MM * w * zoom/zoom_d),
                 static_cast<int>(PIXELS_PER_MM * h * zoom/zoom_d));
      p.fillRect(rect,c);
   }
   else
   {
      QRect rect(static_cast<int>(PIXELS_PER_MM * (x - h/2) * zoom/zoom_d),
                 static_cast<int>(PIXELS_PER_MM * (y - w/2) * zoom/zoom_d),
                 static_cast<int>(PIXELS_PER_MM * h * zoom/zoom_d),
                 static_cast<int>(PIXELS_PER_MM * w * zoom/zoom_d));
      p.fillRect(rect,c);
   }
}


void DrawWrapper::DrawCap(QPainter& p, QColor& c, ITEMS_ORIENTATION type, int x, int y, int f, int zoom,int zoom_d)
{
   QPen pen;       
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom);   
   p.setPen(pen);
   if(type == O_HORIZONTAL_LEFT || type == O_HORIZONTAL_RIGHT)
   {
      p.drawLine((x - f/2) * zoom/zoom_d,y * zoom/zoom_d ,(x - f/8) * zoom/zoom_d,y * zoom/zoom_d);
      p.drawLine((x + f/8) * zoom/zoom_d,y * zoom/zoom_d ,(x + f/2) * zoom/zoom_d,y * zoom/zoom_d);
      p.drawLine((x - f/8) * zoom/zoom_d,(y - f/4) * zoom/zoom_d,(x - f/8) * zoom/zoom_d,(y + f/4) * zoom/zoom_d);
      p.drawLine((x + f/8) * zoom/zoom_d,(y - f/4) * zoom/zoom_d,(x + f/8) * zoom/zoom_d,(y + f/4) * zoom/zoom_d);
   }
   else
   {
      p.drawLine(x * zoom/zoom_d,(y - f/2) * zoom/zoom_d,x * zoom/zoom_d,(y - f/8) * zoom/zoom_d);
      p.drawLine(x * zoom/zoom_d,(y + f/8) * zoom/zoom_d,x * zoom/zoom_d,(y + f/2) * zoom/zoom_d);
      p.drawLine((x - f/4) * zoom/zoom_d,(y - f/8) * zoom/zoom_d,(x + f/4) * zoom/zoom_d,(y - f/8) * zoom/zoom_d);
      p.drawLine((x - f/4) * zoom/zoom_d,(y + f/8) * zoom/zoom_d,(x + f/4) * zoom/zoom_d,(y + f/8) * zoom/zoom_d);      
   }
}

void DrawWrapper::DrawCapF(QPainter& p, QColor& c, ITEMS_ORIENTATION type, float x, float y, float f, int zoom,int zoom_d)
{
   QPen pen;
   pen.setColor(c);
   pen.setWidth(PACK_LINE_DEF_W * zoom);
   p.setPen(pen);
   if(type == O_HORIZONTAL_LEFT || type == O_HORIZONTAL_RIGHT)
   {
      p.drawLine(castFloatToInt(PIXELS_PER_MM * (x - f/2) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (x - f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d));
      p.drawLine(castFloatToInt(PIXELS_PER_MM * (x + f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (x + f/2) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * y * zoom/zoom_d));
      p.drawLine(castFloatToInt(PIXELS_PER_MM * (x - f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y - f/4) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (x - f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y + f/4) * zoom/zoom_d));
      p.drawLine(castFloatToInt(PIXELS_PER_MM * (x + f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y - f/4) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (x + f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y + f/4) * zoom/zoom_d));
   }
   else
   {
      p.drawLine(castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y - f/2) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y - f/8) * zoom/zoom_d));
      p.drawLine(castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y + f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * x * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y + f/2) * zoom/zoom_d));
      p.drawLine(castFloatToInt(PIXELS_PER_MM * (x - f/4) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y - f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (x + f/4) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y - f/8) * zoom/zoom_d));
      p.drawLine(castFloatToInt(PIXELS_PER_MM * (x - f/4) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y + f/8) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (x + f/4) * zoom/zoom_d),
                 castFloatToInt(PIXELS_PER_MM * (y + f/8) * zoom/zoom_d));
   }
}

void DrawWrapper::draWArrow(QPainter& p, QColor& c,int x, int y, int l, 
                            ITEMS_ORIENTATION type, int zoom, int zoom_d)
{
   QPen pen;       
   pen.setColor(c);
   int dif = abs(l)/10;
   if(dif < 10)
      dif = 10;
   
   switch (type)
   {
      case O_VERTICAL_TOP:
      {
         p.drawLine(x,y,x,y - l);
         p.drawPie(x - dif/2,y - l - dif/2,dif,dif,250 * 16, 40 * 16);                  
         break;
      }
      case O_VERTICAL_BOTTOM:
      {
         p.drawLine(x,y,x,y + l);
         p.drawPie(x - dif/2,y + l - dif/2,dif,dif,70 * 16 , 40 * 16);         
         break;
      }              
      case O_HORIZONTAL_LEFT:
      {
         p.drawLine(x,y,x - l,y);
         p.drawPie(x - l - dif/2,y - dif/2,dif,dif,340 * 16 , 40 * 16);         
         break;
      }
      case O_HORIZONTAL_RIGHT:
      {
         p.drawLine(x,y,x + l,y);
         p.drawPie(x + l - dif/2,y - dif/2,dif,dif,160 * 16 , 40 * 16);
         break;
      }
      default:
         break;      
   }
}

void DrawWrapper::drawRuler(QPainter& p,int x, int y, int x1, int y1,int k_zoom)
{
   QPen pen;
   pen.setWidth(1);
   QFont font = p.font();
   int size = font.pixelSize();
   font.setPixelSize(16 * k_zoom);
   p.setFont(font);
   pen.setColor(rulerColor);
   p.setPen(pen);
   int w = x1 - x;
   int h = y1 - y;
   int l = static_cast<int>(std::sqrt(w * w + h * h));
//   p.drawRect(x,y,w,h);
   p.drawLine(x,y,x1,y1);
   char buf[64];
   sprintf(buf,"x=%.3f,y=%.3f,l=%.3f",(float)w/PIXELS_PER_MM/k_zoom,(float)h/PIXELS_PER_MM/k_zoom,(float)l/PIXELS_PER_MM/k_zoom);
   p.drawText(x1 - w/2,y1 - 20,270 * k_zoom,16 * k_zoom,Qt::AlignCenter,buf);
   font.setPixelSize(size);
   p.setFont(font);
}

QPoint DrawWrapper::drawMultiPlate(QPainter& p, QColor& c, int x, int y, int dD, int d, int zoom, int zoom_d)
{
   int diff = dD/6;
   drawPlate(p,c,x+diff,y+diff,dD,d,zoom,zoom_d);
   drawPlate(p,c,x-diff,y-diff,dD,d,zoom,zoom_d);
   return QPoint(diff,diff);
}

PointF DrawWrapper::drawMultiPlateF(QPainter& p, QColor& c, float x, float y, float dD, float d, int zoom, int zoom_d)
{
   float diff = dD/6;
   drawPlateF(p,c,x+diff,y+diff,dD,d,zoom,zoom_d);
   drawPlateF(p,c,x-diff,y-diff,dD,d,zoom,zoom_d);
   return PointF(diff,diff);
}

void DrawWrapper::drawLine(QPainter& p, const QColor& c, vector<PointF>& points ,PointF *ptParent,bool bSelected, float w,
                           LINE_STYLE style,int zoom, int zoom_d)
{
    if(points.size() > 1)
    {
        QPen pen;
        pen.setWidth(static_cast<int>(w * PIXELS_PER_MM * zoom));
        pen.setCapStyle(style == LINE_STYLE::LINE_ROUNDED ? Qt::RoundCap : Qt::SquareCap);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setColor(c);
        p.setPen(pen);
        for (std::vector<PointF>::size_type i = 1; i < points.size(); ++i)
        {
           float x,y,x1,y1;
           if(ptParent)
           {
              x = points[ i -1 ].x() + ptParent->x();
              y = points[ i -1 ].y() + ptParent->y();
              x1 = points[ i ].x() + ptParent->x();
              y1 = points[ i ].y() + ptParent->y();
           }
           else
           {
              x = points[ i -1 ].x();
              y = points[ i -1 ].y();
              x1 = points[ i ].x();
              y1 = points[ i ].y();

           }
           int xx = static_cast<int>(x * zoom * PIXELS_PER_MM);
           int yy = static_cast<int>(y * zoom * PIXELS_PER_MM);
           int xx1 = static_cast<int>(x1 * zoom * PIXELS_PER_MM);
           int yy1 = static_cast<int>(y1 * zoom * PIXELS_PER_MM);
           p.drawLine(xx,yy,xx1,yy1);
        }

        if(bSelected)
        {
           QPen pen1;
           pen1.setWidth(1);
           pen1.setColor(Qt::black);
           p.setPen(pen1);
           for (std::vector<PointF>::size_type i = 0; i < points.size(); ++i)
           {
              float x,y;
              if(ptParent)
              {
                 x = points[ i ].x() + ptParent->x();
                 y = points[ i ].y() + ptParent->y();
              }
              else
              {
                 x = points[ i ].x();
                 y = points[ i ].y();
              }
              int xx = static_cast<int>(x * zoom * PIXELS_PER_MM);
              int yy = static_cast<int>(y * zoom * PIXELS_PER_MM);
              QPoint pt(xx,yy);
              int ww = static_cast<int>(w * zoom * PIXELS_PER_MM);
              p.drawEllipse(pt,ww/2,ww/2);
           }
        }

        p.setPen(Qt::NoPen);
    }
}

void DrawWrapper::drawLine(QPainter& p, const QColor& c, vector<PointF>& points ,
                     PointF *ptParent,
                     PointF& dif,
                     bool bSelected,float w,LINE_STYLE style,
                     int zoom, int zoom_d)
{
   if(points.size() > 1)
   {
      QPen pen;
      pen.setWidth(static_cast<int>(w * PIXELS_PER_MM * zoom));
      pen.setCapStyle(style == LINE_STYLE::LINE_ROUNDED ? Qt::RoundCap : Qt::SquareCap);
      pen.setJoinStyle(Qt::RoundJoin);
      pen.setColor(c);
      p.setPen(pen);
      for (std::vector<PointF>::size_type i = 1; i < points.size(); ++i)
      {
         float x,y,x1,y1;
         x = points[ i -1 ].x() + dif.x();
         y = points[ i -1 ].y() + dif.y();
         x1 = points[ i ].x() + dif.x();
         y1 = points[ i ].y() + dif.y();

         if(ptParent)
         {
            x += ptParent->x();
            y += ptParent->y();
            x1 += ptParent->x();
            y1 += ptParent->y();
         }
         int xx = static_cast<int>(x * zoom * PIXELS_PER_MM);
         int yy = static_cast<int>(y * zoom * PIXELS_PER_MM);
         int xx1 = static_cast<int>(x1 * zoom * PIXELS_PER_MM);
         int yy1 = static_cast<int>(y1 * zoom * PIXELS_PER_MM);
         p.drawLine(xx,yy,xx1,yy1);
      }

      if(bSelected)
      {
         QPen pen1;
         pen1.setWidth(1);
         pen1.setColor(Qt::black);
         p.setPen(pen1);
         for (std::vector<PointF>::size_type i = 0; i < points.size(); ++i)
         {
            float x,y;
            x = points[ i ].x() + dif.x();
            y = points[ i ].y() + dif.y();
            if(ptParent)
            {
               x += ptParent->x();
               y += ptParent->y();
            }
            int xx = static_cast<int>(x * zoom * PIXELS_PER_MM);
            int yy = static_cast<int>(y * zoom * PIXELS_PER_MM);
            QPoint pt(xx,yy);
            int ww = static_cast<int>(w * zoom * PIXELS_PER_MM);
            p.drawEllipse(pt,ww/2,ww/2);
         }
      }
      p.setPen(Qt::NoPen);
   }
}

void DrawWrapper::drawConnector(QPainter& p, const QColor& c, vector<VcConCunstrInfo>& points ,
                                 PointF *ptParent,bool bSelected,float w,int zoom, int )
{
    bool bDrawPlate = false;
    if(points.size() > 1)
    {
        QPen pen;
        pen.setWidth(static_cast<int>(w * PIXELS_PER_MM * zoom));
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::RoundJoin);
        pen.setColor(c);
        p.setPen(pen);
        for (std::vector<VcConCunstrInfo>::size_type i = 1; i < points.size(); ++i)
        {
           float x,y,x1,y1;
           if(points[i - 1].isConnected() || points[i].isConnected())
               bDrawPlate = true;
           if(ptParent)
           {
              x = points[ i -1 ].m_pt.x() + ptParent->x();
              y = points[ i -1 ].m_pt.y() + ptParent->y();
              x1 = points[ i ].m_pt.x() + ptParent->x();
              y1 = points[ i ].m_pt.y() + ptParent->y();
           }
           else
           {
              x = points[ i -1 ].m_pt.x();
              y = points[ i -1 ].m_pt.y();
              x1 = points[ i ].m_pt.x();
              y1 = points[ i ].m_pt.y();

           }
           int xx = static_cast<int>(x * zoom * PIXELS_PER_MM);
           int yy = static_cast<int>(y * zoom * PIXELS_PER_MM);
           int xx1 = static_cast<int>(x1 * zoom * PIXELS_PER_MM);
           int yy1 = static_cast<int>(y1 * zoom * PIXELS_PER_MM);
           p.drawLine(xx,yy,xx1,yy1);
        }
        if(bDrawPlate)
        {
            p.setBrush(QBrush(c));
            for (std::vector<VcConCunstrInfo>::size_type i = 0; i < points.size(); ++i)
            {
               if(points[i].isConnected())
               {
                  float x = ptParent ? ptParent->x() + points[i].m_pt.x() :
                                      points[i].m_pt.x();
                  float y = ptParent ? ptParent->y() + points[i].m_pt.y() :
                                      points[i].m_pt.y();

                  p.drawEllipse(QPoint(castFloatToInt(x * PIXELS_PER_MM * zoom),
                                       castFloatToInt(y * PIXELS_PER_MM * zoom)),
                                       castFloatToInt(fDefVirtConPlateD/2 * PIXELS_PER_MM * zoom),
                                       castFloatToInt(fDefVirtConPlateD/2 * PIXELS_PER_MM * zoom));
               }
            }
            p.setBrush(Qt::NoBrush);
        }
        p.setPen(Qt::NoPen);
        if(bSelected)
        {
           QPen pen1;
           pen1.setWidth(1);
           pen1.setColor(Qt::black);
           p.setPen(pen1);
           for (std::vector<VcConCunstrInfo>::size_type i = 0; i < points.size(); ++i)
           {
              if(!points[i].isConnected())
              {
                  float x,y;
                  if(ptParent)
                  {
                     x = points[ i ].m_pt.x() + ptParent->x();
                     y = points[ i ].m_pt.y() + ptParent->y();
                  }
                  else
                  {
                     x = points[ i ].m_pt.x();
                     y = points[ i ].m_pt.y();
                  }
                  int xx = static_cast<int>(x * zoom * PIXELS_PER_MM);
                  int yy = static_cast<int>(y * zoom * PIXELS_PER_MM);
                  QPoint pt(xx,yy);
                  int ww = static_cast<int>(w * zoom * PIXELS_PER_MM);
                  p.drawEllipse(pt,ww/2,ww/2);
              }
           }
        }
        p.setPen(Qt::NoPen);
    }
}

void DrawWrapper::drawConnectorPlate(QPainter& p, const QColor& c,float x,float y,int zoom)
{
   p.setBrush(QBrush(c));
   QPen pen(c);
   p.setPen(pen);
   p.drawEllipse(QPoint(castFloatToInt(x * PIXELS_PER_MM * zoom),
                        castFloatToInt(y * PIXELS_PER_MM * zoom)),
                        castFloatToInt(fDefVirtConPlateD/2 * PIXELS_PER_MM * zoom),
                        castFloatToInt(fDefVirtConPlateD/2 * PIXELS_PER_MM * zoom));
   p.setBrush(Qt::NoBrush);
   p.setPen(Qt::NoPen);
/*
   QPainterPath path;
   path.addEllipse(QPoint(castFloatToInt(x * PIXELS_PER_MM * zoom),
                           castFloatToInt(y * PIXELS_PER_MM * zoom)),
                           castFloatToInt(fDefVirtConPlateD/2 * PIXELS_PER_MM * zoom),
                           castFloatToInt(fDefVirtConPlateD/2 * PIXELS_PER_MM * zoom));
   p.fillPath(path,c);
*/
}

void DrawWrapper::drawRulerPoint(QPainter& p, const QColor& c,float x,float y,int zoom)
{
    drawConnectorPlate(p,c,x,y,zoom);
}


//centerX,centerY used when the text should be drawn vertically , we use for QPainter for rotating.
//in contrast to the other items(package ,plate etc...) for whome we calculate rotation coordinates.
void DrawWrapper::drawText(QPainter& p, const QColor& c, QString& text,int fontSize,float centerX,float centerY,float x,float y,
                           ITEMS_ORIENTATION o,int zoom, int zoom_d)
{
/*
    painter.save();
    painter.translate(p.x()-10,p.y()+2*lineH);
    painter.rotate(-90);
    painter.drawText(0,0,4*R,2*R,Qt::AlignCenter,QString::number(rightF.at(i).dur));
    painter.restore();
*/

    QScreen* pScreen = QGuiApplication::primaryScreen();
    auto ratioX = pScreen->physicalDotsPerInchX()/72;
    auto ratioY = pScreen->physicalDotsPerInchY()/72;
    int xx = static_cast<int>((centerX > 0 ? centerX : x) * zoom * PIXELS_PER_MM);
    int yy = static_cast<int>((centerY > 0 ? centerY : y) * zoom * PIXELS_PER_MM);
    int angle = -1;

    QFont font("Arial",fontSize * zoom);
//    font.setPixelSize(fontSize);
    auto dx = castFloatToInt(text.length() * (fontSize * static_cast<float>(ratioX) * zoom)/2 + 1);
    auto dy = castFloatToInt((fontSize * static_cast<float>(ratioY) * zoom)/2 + 1);
    auto xx2 = 0;
    auto yy2 = 0;
    if(centerX > 0)
        xx2 = static_cast<int>(x * zoom * PIXELS_PER_MM);

    if(centerY > 0)
        yy2 = static_cast<int>(y * zoom * PIXELS_PER_MM);

    switch(o)
    {
       case O_VERTICAL_TOP:
         angle = 90;
         break;
       case O_HORIZONTAL_RIGHT:
         angle = 180;
         break;
       case O_VERTICAL_BOTTOM:
         angle = 270;
         break;
       default:
         break;
    }
    p.save();
    p.setFont(font);
    QPen pen;
    pen.setColor(c);
    pen.setStyle(Qt::PenStyle::SolidLine);
    p.setPen(pen);
    p.translate(xx,
                yy);

    if(angle != -1)
    {
       p.rotate(angle);
    }

    p.drawText(xx2 - dx,
               yy2 - dy,2 * dx,3 * dy  ,Qt::AlignCenter | Qt::AlignVCenter,text);
    p.translate(-xx,
                -yy);
    p.rotate(-angle);
    p.restore();
}

void DrawWrapper::drawHorText(QPainter& p, const QColor& c, QString& text,int fontSize,float centerX,float centerY,int zoom)
{
    QScreen* pScreen = QGuiApplication::primaryScreen();
    auto ratioX = pScreen->physicalDotsPerInchX()/72;
    auto ratioY = pScreen->physicalDotsPerInchY()/72;
    auto dx = castFloatToInt(text.length() * (fontSize * static_cast<float>(ratioX) * zoom)/2 + 1);
    auto dy = castFloatToInt((fontSize * static_cast<float>(ratioY) * zoom)/2 + 1);
    cout<<"dy="<<dy<<endl;
    auto x = static_cast<int>(centerX * zoom * PIXELS_PER_MM);
    auto y = static_cast<int>(centerY * zoom * PIXELS_PER_MM);
    if(x - dx < 0)
        x = dx;
    if(y - dy < 0)
        y = dy;
    QPen pen;
    pen.setColor(c);
    pen.setStyle(Qt::PenStyle::SolidLine);
    p.setPen(pen);
    p.drawText(x - dx,
               y - dy,2 * dx,3 * dy  ,Qt::AlignCenter | Qt::AlignVCenter,text);

}

void DrawWrapper::drawLine(QPainter& p, const QColor& c,float x1,float y1,float x2,float y2,
                     bool ,float w,LINE_STYLE style,int zoom, int )
{
    QPen pen;
    pen.setWidth(static_cast<int>(w * PIXELS_PER_MM * zoom));
    pen.setCapStyle(style == LINE_STYLE::LINE_ROUNDED ? Qt::RoundCap : Qt::SquareCap);
    pen.setJoinStyle(Qt::RoundJoin);
    pen.setColor(c);
    p.setPen(pen);
    int xx1 = static_cast<int>(x1 * zoom * PIXELS_PER_MM);
    int yy1 = static_cast<int>(y1 * zoom * PIXELS_PER_MM);
    int xx2 = static_cast<int>(x2 * zoom * PIXELS_PER_MM);
    int yy2 = static_cast<int>(y2 * zoom * PIXELS_PER_MM);

    p.drawLine(xx1,yy1,xx2,yy2);
}


DrawWrapper::~DrawWrapper()
{
   
}
