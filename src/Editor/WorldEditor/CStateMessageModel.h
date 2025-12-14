#ifndef CSTATEMESSAGEMODEL_H
#define CSTATEMESSAGEMODEL_H

#include "Editor/UICommon.h"
#include <Core/Resource/Script/CGameTemplate.h>
#include <Core/Resource/Script/CScriptTemplate.h>
#include <QAbstractListModel>

// todo: support pulling states/messages from script templates instead of game
class CStateMessageModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum class EType
    {
        States,
        Messages
    };

private:
    struct SEntry
    {
        uint32_t ID = 0;
        QString Name;

        SEntry() = default;
        SEntry(uint32_t _ID, QString rkName)
            : ID(_ID), Name(std::move(rkName)) {}

        bool operator<(const SEntry& rkOther) const
        {
            return Name < rkOther.Name;
        }
    };
    QList<SEntry> mEntries;

    CGameTemplate *mpGame = nullptr;
    CScriptTemplate *mpScript = nullptr;
    EType mType;

public:
    explicit CStateMessageModel(EType Type, QObject *pParent = nullptr)
        : QAbstractListModel(pParent)
        , mType(Type)
    {}

    int rowCount(const QModelIndex& /*rkParent*/) const override
    {
        return mEntries.size();
    }

    QVariant data(const QModelIndex& rkIndex, int Role) const override
    {
        if (Role == Qt::DisplayRole)
        {
            return mEntries[rkIndex.row()].Name;
        }

        return QVariant();
    }

    void SetGameTemplate(CGameTemplate *pGame)
    {
        beginResetModel();

        mpGame = pGame;
        mEntries.clear();

        if (mType == EType::States)
        {
            for (uint32_t iState = 0; iState < pGame->NumStates(); iState++)
            {
                SState State = pGame->StateByIndex(iState);
                mEntries.emplace_back(State.ID, TO_QSTRING(State.Name));
            }
        }
        else
        {
            for (uint32_t iMsg = 0; iMsg < pGame->NumMessages(); iMsg++)
            {
                SMessage Message = pGame->MessageByIndex(iMsg);
                mEntries.emplace_back(Message.ID, TO_QSTRING(Message.Name));
            }
        }

        std::sort(mEntries.begin(), mEntries.end());
        endResetModel();
    }

    uint32_t StateIndex(uint32_t StateID) const
    {
        if (mType == EType::Messages)
            return UINT32_MAX;

        for (uint32_t iState = 0; iState < mEntries.size(); iState++)
        {
            if (mEntries[iState].ID == StateID)
                return iState;
        }

        return UINT32_MAX;
    }

    uint32_t MessageIndex(uint32_t MessageID) const
    {
        if (mType == EType::States)
            return UINT32_MAX;

        for (uint32_t iMsg = 0; iMsg < mEntries.size(); iMsg++)
        {
            if (mEntries[iMsg].ID == MessageID)
                return iMsg;
        }

        return UINT32_MAX;
    }

    void SetScriptTemplate(CScriptTemplate *pScript)
    {
        mpScript = pScript;
    }

    uint32_t State(uint32_t Index) const
    {
        return (mType == EType::States ? mEntries[Index].ID : 0);
    }

    uint32_t Message(uint32_t Index) const
    {
        return (mType == EType::Messages ? mEntries[Index].ID : 0);
    }
};

#endif // CSTATEMESSAGEMODEL_H
