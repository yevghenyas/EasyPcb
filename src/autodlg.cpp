#include "autodlg.h"
#include <QPainter>
#include <QGridLayout>

AutoDlg::AutoDlg(PcbLayoutVec *lCells,int width,int height,int ax,int ay,int bx,int by):m_pCells(lCells)
{
   resize(1920,1080);

   m_pWidget = new PcbLayout(lCells,width,height,ax,ay,bx,by);
   m_pArea = new QScrollArea(this);
   m_pArea->setWidget(m_pWidget);
   m_pArea->verticalScrollBar();
   m_pArea->horizontalScrollBar();
   QGridLayout *gridLayout = new QGridLayout();
   setLayout(gridLayout);
   gridLayout->addWidget(m_pArea, 1, 1);

}


void AutoDlg::paintEvent(QPaintEvent *e)
{
/*
   QPainter p( this );
   QColor color(Qt::black);
   QPen pen(color);
   p.setPen(pen);
   for (VecIdsInc i = 0 ; i < m_pCells->size(); ++i)
   {
      for(VecIdsInc j = 0; j < m_pCells->at(i).size() ; ++j)
      {
         int value = (*m_pCells)[i][j];
         if(value != ID_NONE)
         {
            int cellSize = static_cast<int>(fSizeOfMatrixCell * PIXELS_PER_MM * 4);
            int y = cellSize * i;
            int x = cellSize * j;
            QRect rect(x,y,cellSize,cellSize);
            if(value > 0)
            {
               const QString text = QString::number(value);
               p.drawText(rect,text);
//               p.fillRect(rect,Qt::yellow);
            }
            else
            {
               p.fillRect(rect,Qt::blue);
            }
         }
      }
   }
*/
}
