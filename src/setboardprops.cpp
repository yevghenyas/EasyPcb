#include "setboardprops.h"

SetBoardProps::SetBoardProps(PcBoard *p,float w,float h):
                             m_pBoard(p),m_w(w),m_h(h)
{

}

SetBoardProps::~SetBoardProps()
{

}

void SetBoardProps::redo()
{
   m_wPrev = m_pBoard->getWidth()/PIXELS_PER_MM;
   m_hPrev = m_pBoard->getHeight()/PIXELS_PER_MM;
   m_pBoard->setSize(static_cast<int>(m_w * PIXELS_PER_MM),
                      static_cast<int>(m_h * PIXELS_PER_MM));
}

void SetBoardProps::undo()
{
   m_pBoard->setSize(static_cast<int>(m_wPrev * PIXELS_PER_MM),
                     static_cast<int>(m_hPrev * PIXELS_PER_MM));
}
