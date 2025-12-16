#ifndef CLINK_H
#define CLINK_H

#include "Core/Resource/Area/CGameArea.h"
#include "Core/Resource/Script/CScriptObject.h"
#include <Common/TString.h>
#include <cstdint>

struct SState
{
    union {
        uint32_t ID;
        CFourCC ID_4CC;
    };
    TString Name;

    SState()
    {}

    SState(uint32_t InID, const TString& kInName)
        : ID(InID)
        , Name(kInName)
    {}

    void Serialize(IArchive& Arc)
    {
        if (Arc.Game() <= EGame::Prime)
            Arc << SerialParameter("ID", ID, SH_Attribute | SH_HexDisplay);
        else
            Arc << SerialParameter("ID", ID_4CC, SH_Attribute);

        Arc << SerialParameter("Name", Name, SH_Attribute);
    }
};

struct SMessage
{
    union {
        uint32_t ID;
        CFourCC ID_4CC;
    };
    TString Name;

    SMessage()
    {}

    SMessage(uint32_t InID, const TString& kInName)
        : ID(InID)
        , Name(kInName)
    {}

    void Serialize(IArchive& Arc)
    {
        if (Arc.Game() <= EGame::Prime)
            Arc << SerialParameter("ID", ID, SH_Attribute | SH_HexDisplay);
        else
            Arc << SerialParameter("ID", ID_4CC, SH_Attribute);

        Arc << SerialParameter("Name", Name, SH_Attribute);
    }
};

class CLink
{
    CGameArea *mpArea;
    uint32_t mStateID = UINT32_MAX;
    uint32_t mMessageID = UINT32_MAX;
    uint32_t mSenderID = UINT32_MAX;
    uint32_t mReceiverID = UINT32_MAX;

public:
    explicit CLink(CGameArea *pArea)
        : mpArea(pArea)
    {}

    CLink(CGameArea *pArea, uint32_t StateID, uint32_t MessageID, uint32_t SenderID, uint32_t ReceiverID)
        : mpArea(pArea)
        , mStateID(StateID)
        , mMessageID(MessageID)
        , mSenderID(SenderID)
        , mReceiverID(ReceiverID)
    {}

    void SetSender(uint32_t NewSenderID, uint32_t Index = UINT32_MAX)
    {
        const uint32_t OldSenderID = mSenderID;
        CScriptObject *pOldSender = mpArea->InstanceByID(OldSenderID);
        CScriptObject *pNewSender = mpArea->InstanceByID(NewSenderID);

        mSenderID = NewSenderID;

        if (pOldSender)
            pOldSender->RemoveLink(ELinkType::Outgoing, this);

        pNewSender->AddLink(ELinkType::Outgoing, this, Index);
    }

    void SetReceiver(uint32_t NewReceiverID, uint32_t Index = UINT32_MAX)
    {
        const uint32_t OldReceiverID = mSenderID;
        CScriptObject *pOldReceiver = mpArea->InstanceByID(OldReceiverID);
        CScriptObject *pNewReceiver = mpArea->InstanceByID(NewReceiverID);

        mReceiverID = NewReceiverID;

        if (pOldReceiver)
            pOldReceiver->RemoveLink(ELinkType::Incoming, this);

        pNewReceiver->AddLink(ELinkType::Incoming, this, Index);
    }

    uint32_t SenderIndex() const
    {
        if (const CScriptObject* pSender = mpArea->InstanceByID(mSenderID))
        {
            for (uint32_t iLink = 0; iLink < pSender->NumLinks(ELinkType::Outgoing); iLink++)
            {
                if (pSender->Link(ELinkType::Outgoing, iLink) == this)
                    return iLink;
            }
        }

        return UINT32_MAX;
    }

    uint32_t ReceiverIndex() const
    {
        if (const CScriptObject* pReceiver = mpArea->InstanceByID(mReceiverID))
        {
            for (uint32_t iLink = 0; iLink < pReceiver->NumLinks(ELinkType::Incoming); iLink++)
            {
                if (pReceiver->Link(ELinkType::Incoming, iLink) == this)
                    return iLink;
            }
        }

        return UINT32_MAX;
    }

    // Operators
    bool operator==(const CLink&) const = default;

    // Accessors
    CGameArea* Area() const          { return mpArea; }
    uint32_t State() const           { return mStateID; }
    uint32_t Message() const         { return mMessageID; }
    uint32_t SenderID() const        { return mSenderID; }
    uint32_t ReceiverID() const      { return mReceiverID; }
    CScriptObject* Sender() const    { return mpArea->InstanceByID(mSenderID); }
    CScriptObject* Receiver() const  { return mpArea->InstanceByID(mReceiverID); }

    void SetState(uint32_t StateID)       { mStateID = StateID; }
    void SetMessage(uint32_t MessageID)   { mMessageID = MessageID; }
};


#endif // CLINK_H
