#ifndef CTWEAKDATA_H
#define CTWEAKDATA_H

#include "Core/Resource/CResource.h"
#include "Core/Resource/Script/CScriptTemplate.h"
#include "Core/Resource/Script/Property/TPropertyRef.h"
#include <cstdint>
#include <vector>

/** Tweak data assets for MP1 */
class CTweakData : public CResource
{
    DECLARE_RESOURCE_TYPE(Tweaks)

    /** Script template specifying tweak data layout */
    CScriptTemplate* mpTemplate;

    /** Tweak ID for MP2+ */
    uint32_t mTweakID;

    /** Tweak data */
    std::vector<uint8_t> mTweakData;

public:
    CTweakData(CScriptTemplate* pTemplate, uint32_t TweakID, CResourceEntry* pEntry = nullptr)
        : CResource(pEntry)
        , mpTemplate(pTemplate)
        , mTweakID(TweakID)
    {
        CStructProperty* pProperties = pTemplate->Properties();
        mTweakData.resize(pProperties->DataSize());
        pProperties->Construct(mTweakData.data());
    }

    TString TweakName()
    {
        if (Entry() != nullptr)
        {
            return Entry()->Name();
        }
        else
        {
            IProperty* pNameProperty = mpTemplate->Properties()->ChildByID(0x7FDA1466);
            return CStringRef(mTweakData.data(), pNameProperty);
        }
    }

    CScriptTemplate* TweakTemplate() const
    {
        return mpTemplate;
    }

    uint32_t TweakID() const
    {
        return mTweakID;
    }

    CStructRef TweakData() const
    {
        return CStructRef((void*) mTweakData.data(), mpTemplate->Properties());
    }
};

#endif // CTWEAKDATA_H
