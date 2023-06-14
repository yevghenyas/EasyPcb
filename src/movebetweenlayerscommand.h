#ifndef MOVEBETWEENLAYERSCOMMAND_H
#define MOVEBETWEENLAYERSCOMMAND_H

#include <QUndoCommand>
#include "pcboard.h"


class MoveBetweenLayersCommand : public QUndoCommand
{
    SmartPtr<GraphicalItem> m_pItem;
    QString name;
    PcBoard *m_pBoard{nullptr};
    BOARD_LEVEL_ID level{LEVEL_NONE};
    ContainerType m_containerTyoe;
public:
    MoveBetweenLayersCommand(SmartPtr<GraphicalItem>& p,const QString& n,PcBoard *pBoard,
                             BOARD_LEVEL_ID newLevel,ContainerType type);
    virtual ~MoveBetweenLayersCommand() override;
    void undo() override;
    void redo() override;
};

#endif // MOVEBETWEENLAYERSCOMMAND_H
