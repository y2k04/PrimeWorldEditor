#ifndef CDELETESELECTIONCOMMAND_H
#define CDELETESELECTIONCOMMAND_H

#include "Editor/Undo/IUndoCommand.h"
#include "Editor/Undo/ObjReferences.h"

#include <QCoreApplication>

class CWorldEditor;

// todo: currently only supports deleting script nodes; needs support for light nodes as well
// plus maybe it should be extensible enough to support other possible types
class CDeleteSelectionCommand : public IUndoCommand
{
    CWorldEditor *mpEditor;
    CNodePtrList mOldSelection;
    CNodePtrList mNewSelection;
    CInstancePtrList mLinkedInstances;

    struct SDeletedNode;
    QList<SDeletedNode> mDeletedNodes;

    struct SDeletedLink;
    QList<SDeletedLink> mDeletedLinks;

public:
    explicit CDeleteSelectionCommand(CWorldEditor *pEditor, const QString& rkCommandName = QCoreApplication::translate("CDeleteSelectionCommand", "Delete"));
    ~CDeleteSelectionCommand() override;

    void undo() override;
    void redo() override;
    bool AffectsCleanState() const override { return true; }
};

#endif // CDELETESELECTIONCOMMAND_H
