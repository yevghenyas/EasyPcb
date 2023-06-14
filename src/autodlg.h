#ifndef AUTODLG_H
#define AUTODLG_H

#include <QDialog>
#include "common.h"
#include <QScrollArea>
#include <QPainter>
#include <QFont>

using PcbLayoutVec = vector<vector<ITEM_ID> >;
using VecIdsInc = std::vector<int>::size_type;


class PcbLayout:public QWidget
{
   PcbLayoutVec *m_pCells;
   int m_ax,m_ay,m_bx,m_by;
public:
   PcbLayout(PcbLayoutVec *layCells,int width,int height,
             int ax,int ay,int bx,int by):m_pCells(layCells),
             m_ax(ax),m_ay(ay),m_bx(bx),m_by(by)
   {
      resize(width * PIXELS_PER_MM * 4,height * PIXELS_PER_MM * 4);
   }
   virtual void paintEvent(QPaintEvent*)
   {
       QPainter p( this );
       QColor color(Qt::black);
       QPen pen(color);
       p.setPen(pen);
       QFont font("Arial",4);
       p.setFont(font);
       for (VecIdsInc i = 0 ; i < m_pCells->size(); ++i)
       {
          for(VecIdsInc j = 0; j < m_pCells->at(i).size() ; ++j)
          {
             int cellSize = static_cast<int>(fSizeOfMatrixCell * PIXELS_PER_MM * 4);
             int y = cellSize * i;
             int x = cellSize * j;
             QRect rect(x,y,cellSize,cellSize);
             int value = (*m_pCells)[i][j];
             if(value == ID_NONE)
                p.fillRect(rect,Qt::cyan);
             if(value != ID_NONE)
             {
                if(value >= 0)
                {
                   const QString text = QString::number(value);
                   p.drawText(rect,text);
    //               p.fillRect(rect,Qt::yellow);
                }
                else if(value == WALL)
                {
                   p.drawText(rect,"X");
                }
                else
                {
//                   if((j != m_ax || i != m_ay))
                   p.fillRect(rect,Qt::yellow);
                   if(value < WALL)
                   {
                      const QString txt = QString::number(WALL - value);
                      p.drawText(rect,txt);
                   }
                }
             }
//             else
//             {
//                p.fillRect(rect,Qt::cyan);
//                const QString txt = QString::number(value);
//                p.drawText(rect,txt);
//                continue;
//             }
             if(j == m_ax && i == m_ay)
                p.fillRect(rect,Qt::blue);
             if(j == m_bx && i == m_by)
                p.fillRect(rect,Qt::blue);

          }
       }

   }

};


class AutoDlg : public QDialog
{
   PcbLayoutVec *m_pCells;
   QScrollArea* m_pArea;
   PcbLayout* m_pWidget;
public:
    AutoDlg(PcbLayoutVec *layCells,int width,int height,int ax,int ay,int bx,int by);
    virtual void paintEvent(QPaintEvent*);
};

#endif // AUTODLG_H
