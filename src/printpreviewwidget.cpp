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

#include "printpreviewwidget.h"
#include <QPainter>
#include <QPaintEvent>


PrintPreviewWidget::PrintPreviewWidget(QWidget *parent,BoardLayersWrapper *pLayers,vector<bool> states,int iBoardWidth,int iBoardHeight)
:QLabel(parent),m_pLayers(pLayers),m_states(states),m_iBoardWidth(iBoardWidth),m_iBoardHeight(iBoardHeight),m_nX(1),m_nY(1)
{
   resize(iPrintPreviewWidth,iPrintPreviewHeight);
   setMinimumWidth(iPrintPreviewWidth);
   setMinimumHeight(iPrintPreviewHeight);
   setMaximumWidth(iPrintPreviewWidth);
   setMaximumHeight(iPrintPreviewHeight);
   vector<TransInfo> v;
   getAvailableTransformations(v);
   for(size_t i = 0 ; i < v.size();++i)
      m_transformStates.push_back(false);
   LevelsWrapper::getAvalableLevels(m_availLayers);
}

PrintPreviewWidget::~PrintPreviewWidget()
{

}

QSizePolicy PrintPreviewWidget::sizePolicy() const
{
   return QSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
}


void PrintPreviewWidget::paintEvent(QPaintEvent* e)
{
   QPainter p(this);
   p.fillRect(e->rect(),Qt::white);
   auto ratio = static_cast<double>(PIXELS_PER_MM_PRTPREVIW)/
                static_cast<double>(PIXELS_PER_MM);
   for(int j = 0; j < m_nY ; ++j)
   {
      for(int i = 0; i < m_nX ; ++i)
      {
         p.save();
         if(m_transformStates[TR_MIRROW])
         {
            p.scale(-ratio,ratio);              
            p.translate(-m_iBoardWidth * (i + 1),m_iBoardHeight * j);              
         }
         else
         {
            p.scale(ratio,ratio); 
         }
         if(m_transformStates[TR_ROTATE])
         {
            //wee need to update translation to set the center of rotation
            if(m_transformStates[TR_MIRROW])
            {
               p.translate(m_iBoardWidth + m_iBoardWidth/4 * i,m_iBoardHeight * 43/128 * j);                        
            }
            else
            {
               p.translate(m_iBoardHeight * (i + 1),m_iBoardWidth * j);                        
            }
            p.rotate(90.0);
         }
         if(!m_transformStates[TR_ROTATE] && !m_transformStates[TR_MIRROW])
            p.translate(m_iBoardWidth * i,m_iBoardHeight * j); 
         
         p.drawRect(0,0,m_iBoardWidth,m_iBoardHeight);        



         for(size_t i = 0; i < m_availLayers.size(); ++i)
         {
            if(m_states[i])
               m_pLayers->getLayer(m_availLayers[i].idLevel)->paintItems(p,MODE_PRINT,1);
         }
         p.restore();         
      }
   }
   QLabel::paintEvent(e);
}

void PrintPreviewWidget::updateLayersState(vector< bool >& states)
{
   //copy states
   for (size_t i = 0; i < m_states.size();++i)
   {
      m_states[i] = states[i];
   }
   repaint();
}

void PrintPreviewWidget::updateTransState(vector< bool >& states)
{
   for (size_t i = 0; i < m_transformStates.size();++i)
   {
      m_transformStates[i] = states[i];
      if(m_transformStates[i])
         cout<<"The state is amended "<<std::to_string(i)<<endl;
   }
   repaint();

}

void PrintPreviewWidget::updateNumOfCopies(int x, int y)
{
   m_nX = x;
   m_nY = y;
   repaint();
}
