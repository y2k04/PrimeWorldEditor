#ifndef CPASTENODESCOMMAND
#define CPASTENODESCOMMAND

#include "IUndoCommand.h"
#include "ObjReferences.h"
#include "Editor/CNodeCopyMimeData.h"
#include "Editor/WorldEditor/CWorldEditor.h"
#include <QClipboard>
#include <memory>

class CPasteNodesCommand : public IUndoCommand
{
    CWorldEditor *mpEditor;
    CScriptLayer *mpLayer;
    CVector3f mPastePoint;
    std::unique_ptr<CNodeCopyMimeData> mpMimeData;
    CNodePtrList mPastedNodes;
    CNodePtrList mOriginalSelection;
    CInstancePtrList mLinkedInstances;

public:
    CPasteNodesCommand(CWorldEditor *pEditor, CScriptLayer *pLayer, CVector3f PastePoint);
    ~CPasteNodesCommand() override;

    void undo() override;
    void redo() override;

    bool AffectsCleanState() const override { return true; }
};

#endif // CPASTENODESCOMMAND

