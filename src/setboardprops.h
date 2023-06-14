#ifndef SETBOARDPROPS_H
#define SETBOARDPROPS_H

#include <QUndoCommand>
#include "pcboard.h"

class SetBoardProps : public QUndoCommand
{
    PcBoard *m_pBoard;
    float m_w{0.0f};
    float m_h{0.0f};
    float m_wPrev{0.0f};
    float m_hPrev{0.0f};
public:
    SetBoardProps(PcBoard *p,float w,float h);
    virtual ~SetBoardProps() override;
    void undo() override;
    void redo() override;
};

#endif // SETBOARDPROPS_H
