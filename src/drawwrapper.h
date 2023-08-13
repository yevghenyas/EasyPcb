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

#ifndef DRAWWRAPPER_H
#define DRAWWRAPPER_H

#include <QPainter>
#include "common.h"
#include <QColor>
#include "mousetrace.h"
#include <QGuiApplication>
#include <QScreen>


class DrawWrapper
{
public:
    DrawWrapper();
    static void drawPackege(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            int x,int y,int width,int height,int zoom = 1,int zoom_d = 1);

    static void drawPackageF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                                  float x,float y,float width,float height,int zoom,int zoom_d);

    
    static void drawRectPackage(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            int x,int y,int width,int height,int zoom = 1,int zoom_d = 1);

    static void drawFilledRectPackage(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            int x,int y,int width,int height,int zoom = 1,int zoom_d = 1);

    static void drawFilledRectPackageF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            float x,float y,float width,float height,int zoom = 1,int zoom_d = 1);

    
    static void drawRectPackageF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            float x,float y,float width,float height,int zoom = 1,int zoom_d = 1);
    
    static void drawPlate(QPainter& p, QColor& c, int x, int y, int dD, int d,int zoom = 1,int zoom_d = 1);

    static void drawPlateF(QPainter& p, QColor& c, float x, float y, float dD, float d,int zoom = 1,int zoom_d = 1);
  
    static void drawRectPlate(QPainter& p, QColor& c, int x, int y, int w, int h,int zoom = 1,int zoom_d = 1);    

    static void drawRectPlateF(QPainter& p, QColor& c, ITEMS_ORIENTATION type, float x, float y, float w, float h,int zoom = 1,int zoom_d = 1);
    
    static void drawRoundPackage(QPainter& p, QColor& c, int x, int y, int dD,int dd,int zoom,int zoom_d = 1);

    static void drawRoundPackageF(QPainter& p, QColor& c, float centerX, float centerY, float rx,float ry,int zoom,int zoom_d = 1);


    //draws arc that may be closed with line
    static void drawArchedPackageF(QPainter& p, QColor& c, float x, float y, float d,int stAngle,
                                         int spAngle,bool closed,int zoom,int zoom_d);
    //deprecated
    static void drawDip(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            int x,int y,int n,int width ,int zoom = 1,int zoom_d = 1);
    
    static void DrawCap(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            int x,int y,int f ,int zoom = 1,int zoom_d = 1);

    static void DrawCapF(QPainter& p,QColor& c,ITEMS_ORIENTATION type,
                            float x,float y,float f ,int zoom = 1,int zoom_d = 1);

    
    static void draWArrow(QPainter& p, QColor& c,int x,int y,
                          int l, ITEMS_ORIENTATION type,int zoom = 1, int zoom_d = 1);
    //draws ruler between 2 points
    static void drawRuler(QPainter& p,int x, int y, int x1, int y1,int k_zoom);
    //draws multi-layers plate metallyzed.This methoof intended to be used for drowing icons only.
    //
    static QPoint drawMultiPlate(QPainter& p, QColor& c, int x, int y, int dD, int d,int zoom = 1,int zoom_d = 1);
    static PointF drawMultiPlateF(QPainter& p, QColor& c, float x, float y, float dD, float d, int zoom, int zoom_d);
    static void drawLine(QPainter& p, const QColor& c, vector<PointF>& points ,PointF *ptParent,
                         bool bSelected,float w,LINE_STYLE style,int zoom, int zoom_d);

    static void drawLine(QPainter& p, const QColor& c, vector<PointF>& points ,
                         PointF *ptParent,
                         PointF& dif,
                         bool bSelected,float w,LINE_STYLE style,
                         int zoom, int zoom_d);


    static void drawLine(QPainter& p, const QColor& c,float x1,float y1,float x2,float y2,
                         bool bSelected,float w,LINE_STYLE style,int zoom, int zoom_d);


    static void drawLine(QPainter& p, const QColor& c, vector<MouseTrace> items,
                         PointF *ptParent,
                         bool bSelected,float w,LINE_STYLE style,int zoom, int zoom_d);

    //draws virtual connector plate
    static void drawConnectorPlate(QPainter& p, const QColor& c,float x,float y,int zoom);
    //draws ruler point
    static void drawRulerPoint(QPainter& p, const QColor& c,float x,float y,int zoom);
    static void drawConnector(QPainter& p, const QColor& c, vector<VcConCunstrInfo>& points ,PointF *ptParent,bool bSelected,float w,int zoom, int zoom_d);
    static void drawText(QPainter& p, const QColor& c, QString& text,int fontSize,float centerX,float centerY,float x,float y,ITEMS_ORIENTATION o,
                         int zoom, int zoom_d);
    static void drawHorText(QPainter& p, const QColor& c, QString& text,int fontSize,float centerX,float centerY,int zoom);
    ~DrawWrapper();
};

#endif // DRAWWRAPPER_H
