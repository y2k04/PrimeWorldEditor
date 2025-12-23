#include "Editor/Undo/CDeleteSelectionCommand.h"

#include "Editor/CSelectionIterator.h"
#include "Editor/WorldEditor/CWorldEditor.h"
#include <Common/FileIO.h>
#include <Core/Resource/Cooker/CScriptCooker.h>
#include <Core/Resource/Factory/CScriptLoader.h>
#include <Core/Resource/Script/CScriptLayer.h>
#include <Core/Scene/CSceneNode.h>

#include <vector>

struct CDeleteSelectionCommand::SDeletedNode
{
    // Node Info
    CNodePtr NodePtr;
    uint32_t NodeID;
    CVector3f Position;
    CQuaternion Rotation;
    CVector3f Scale;

    // Instance Info
    CGameArea* pArea;
    CScriptLayer* pLayer;
    uint32_t LayerIndex;
    std::vector<char> InstanceData;
};

struct CDeleteSelectionCommand::SDeletedLink
{
    uint32_t State;
    uint32_t Message;
    uint32_t SenderID;
    uint32_t SenderIndex;
    uint32_t ReceiverID;
    uint32_t ReceiverIndex;
    CInstancePtr pSender;
    CInstancePtr pReceiver;
};

CDeleteSelectionCommand::CDeleteSelectionCommand(CWorldEditor *pEditor, const QString& rkCommandName)
    : IUndoCommand(rkCommandName)
    , mpEditor(pEditor)
{
    QSet<CLink*> Links;
    QList<CScriptObject*> LinkedInstances;

    for (CSelectionIterator It(pEditor->Selection()); It; ++It)
    {
        mOldSelection.push_back(*It);

        if (It->NodeType() == ENodeType::Script)
        {
            CScriptNode *pScript = static_cast<CScriptNode*>(*It);
            CScriptObject *pInst = pScript->Instance();

            mDeletedNodes.push_back(SDeletedNode());
            SDeletedNode& rNode = mDeletedNodes.back();

            rNode.NodePtr = CNodePtr(pScript);
            rNode.NodeID = pScript->ID();
            rNode.Position = pScript->LocalPosition();
            rNode.Rotation = pScript->LocalRotation();
            rNode.Scale = pScript->LocalScale();

            rNode.pArea = pInst->Area();
            rNode.pLayer = pInst->Layer();
            rNode.LayerIndex = pInst->LayerIndex();

            for (size_t iType = 0; iType < 2; iType++)
            {
                ELinkType Type = (iType == 0 ? ELinkType::Outgoing : ELinkType::Incoming);

                for (size_t iLink = 0; iLink < pInst->NumLinks(Type); iLink++)
                {
                    CLink *pLink = pInst->Link(Type, iLink);

                    if (!Links.contains(pLink))
                    {
                        mDeletedLinks.push_back({
                            .State = pLink->State(),
                            .Message = pLink->Message(),
                            .SenderID = pLink->SenderID(),
                            .SenderIndex = pLink->SenderIndex(),
                            .ReceiverID = pLink->ReceiverID(),
                            .ReceiverIndex = pLink->ReceiverIndex(),
                            .pSender = pLink->Sender(),
                            .pReceiver = pLink->Receiver(),
                        });
                        Links.insert(pLink);

                        if (!LinkedInstances.contains(pLink->Sender()))
                            LinkedInstances.push_back(pLink->Sender());
                        if (!LinkedInstances.contains(pLink->Receiver()))
                            LinkedInstances.push_back(pLink->Receiver());
                    }
                }
            }

            CVectorOutStream PropertyDataOut(&rNode.InstanceData, std::endian::big);
            CScriptCooker Cooker(pEditor->CurrentGame());
            Cooker.WriteInstance(PropertyDataOut, pInst);
        }
        else
        {
            mNewSelection.push_back(*It);
        }
    }

    // Remove selected objects from the linked instances list.
    LinkedInstances.removeAll(nullptr);

    for (const CScriptObject* pInst : LinkedInstances)
    {
        if (mpEditor->Scene()->NodeForInstance(pInst)->IsSelected())
            LinkedInstances.removeOne(pInst);
    }

    mLinkedInstances = LinkedInstances;
}

CDeleteSelectionCommand::~CDeleteSelectionCommand() = default;

void CDeleteSelectionCommand::undo()
{
    QList<CSceneNode*> NewNodes;
    QList<uint32> NewInstanceIDs;

    // Spawn nodes
    for (SDeletedNode& rNode : mDeletedNodes)
    {
        mpEditor->NotifyNodeAboutToBeSpawned();

        CMemoryInStream Mem(rNode.InstanceData.data(), rNode.InstanceData.size(), std::endian::big);
        CScriptObject *pInstance = CScriptLoader::LoadInstance(Mem, rNode.pArea, rNode.pLayer, rNode.pArea->Game(), rNode.pArea->Game() > EGame::Prime);
        CScriptNode *pNode = mpEditor->Scene()->CreateScriptNode(pInstance, rNode.NodeID);
        rNode.pArea->AddInstanceToArea(pInstance);
        rNode.pLayer->AddInstance(pInstance, rNode.LayerIndex);

        pNode->SetPosition(rNode.Position);
        pNode->SetRotation(rNode.Rotation);
        pNode->SetScale(rNode.Scale);

        NewNodes.push_back(pNode);
        NewInstanceIDs.push_back(pInstance->InstanceID());
        mpEditor->NotifyNodeSpawned(*rNode.NodePtr);
    }

    // Sort links by sender index, add outgoing
    std::sort(mDeletedLinks.begin(), mDeletedLinks.end(), [](const SDeletedLink& rLeft, const SDeletedLink& rRight) {
        return rLeft.SenderIndex < rRight.SenderIndex;
    });

    for (SDeletedLink& rLink : mDeletedLinks)
    {
        // Adding to the sender is only needed if the sender is not one of the nodes we just spawned. If it is, it already has this link.
        if (!NewInstanceIDs.contains(rLink.SenderID) && *rLink.pSender)
        {
            CLink *pLink = new CLink(rLink.pSender->Area(), rLink.State, rLink.Message, rLink.SenderID, rLink.ReceiverID);
            rLink.pSender->AddLink(ELinkType::Outgoing, pLink, rLink.SenderIndex);
        }
    }

    // Sort links by receiver index, add incoming
    std::sort(mDeletedLinks.begin(), mDeletedLinks.end(), [](const SDeletedLink& rLeft, const SDeletedLink& rRight) {
        return rLeft.ReceiverIndex < rRight.ReceiverIndex;
    });

    for (SDeletedLink& rLink : mDeletedLinks)
    {
        if (*rLink.pReceiver)
        {
            CLink *pLink = (*rLink.pSender ? rLink.pSender->Link(ELinkType::Outgoing, rLink.SenderIndex) : new CLink(rLink.pReceiver->Area(), rLink.State, rLink.Message, rLink.SenderID, rLink.ReceiverID));
            rLink.pReceiver->AddLink(ELinkType::Incoming, pLink, rLink.ReceiverIndex);
        }
    }

    // Run OnLoadFinished
    for (CSceneNode *pNode : NewNodes)
        pNode->OnLoadFinished();

    // Add selection and done
    mpEditor->Selection()->SetSelectedNodes(mOldSelection.DereferenceList());
    mpEditor->OnLinksModified(mLinkedInstances.DereferenceList());
}

void CDeleteSelectionCommand::redo()
{
    mpEditor->Selection()->SetSelectedNodes(mNewSelection.DereferenceList());

    for (SDeletedNode& rNode : mDeletedNodes)
    {
        CSceneNode *pNode = *rNode.NodePtr;
        CScriptObject *pInst = static_cast<CScriptNode*>(pNode)->Instance();

        mpEditor->NotifyNodeAboutToBeDeleted(pNode);
        mpEditor->Scene()->DeleteNode(pNode);
        mpEditor->ActiveArea()->DeleteInstance(pInst);
        mpEditor->NotifyNodeDeleted();
    }

    mpEditor->OnLinksModified(mLinkedInstances.DereferenceList());
}
