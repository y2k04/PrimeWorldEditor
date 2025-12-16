#ifndef CSOUNDPROPERTY_H
#define CSOUNDPROPERTY_H

#include "Core/Resource/Script/Property/IProperty.h"

class CSoundProperty : public TSerializeableTypedProperty<int32_t, EPropertyType::Sound>
{
    friend class IProperty;

protected:
    explicit CSoundProperty(EGame Game)
        : TSerializeableTypedProperty(Game)
    {}

public:
    void SerializeValue(void* pData, IArchive& Arc) const override
    {
        Arc.SerializePrimitive(ValueRef(pData), 0);
    }

    TString ValueAsString(void* pData) const override
    {
        return TString::FromInt32(Value(pData), 0, 10);
    }
};

#endif // CSOUNDPROPERTY_H
