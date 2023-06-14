#ifndef ROTATEITEMCOMMAND_H
#define ROTATEITEMCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"


class RotateItemCommand : public QUndoCommand
{
    SmartPtr<GraphicalItem> m_pItem;
    PcBoard *m_pBoard{nullptr};
public:
    RotateItemCommand(SmartPtr<GraphicalItem>& p,PcBoard *pBoard);
    ~RotateItemCommand() override;
    void redo() override;
    void undo() override;
};

#endif // ROTATEITEMCOMMAND_H
