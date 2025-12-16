#ifndef CANIMATIONPROPERTY_H
#define CANIMATIONPROPERTY_H

#include "Core/Resource/Script/Property/IProperty.h"

class CAnimationProperty : public TSerializeableTypedProperty<uint32_t, EPropertyType::Animation>
{
    friend class IProperty;

protected:
    explicit CAnimationProperty(EGame Game)
        : TSerializeableTypedProperty(Game)
    {}

public:
    void SerializeValue(void* pData, IArchive& rArc) const override
    {
        rArc.SerializePrimitive((uint32_t&)ValueRef(pData), SH_HexDisplay);
    }

    TString ValueAsString(void* pData) const override
    {
        return TString::HexString(static_cast<uint32_t>(Value(pData)));
    }
};

#endif // CANIMATIONPROPERTY_H
