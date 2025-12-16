#ifndef CSHORTPROPERTY_H
#define CSHORTPROPERTY_H

#include "Core/Resource/Script/Property/IProperty.h"
#include <cstdint>

class CShortProperty : public TNumericalProperty<int16_t, EPropertyType::Short>
{
    friend class IProperty;

protected:
    explicit CShortProperty(EGame Game)
        : TNumericalProperty(Game)
    {}

public:
    void SerializeValue(void* pData, IArchive& Arc) const override
    {
        Arc.SerializePrimitive(ValueRef(pData), 0);
    }

    TString ValueAsString(void* pData) const override
    {
        return TString::FromInt32((int32_t)Value(pData), 0, 10);
    }
};

#endif // CSHORTPROPERTY_H
