#ifndef CSEQUENCEPROPERTY_H
#define CSEQUENCEPROPERTY_H

#include "Core/Resource/Script/Property/IProperty.h"

class CSequenceProperty : public TTypedProperty<int32_t, EPropertyType::Sequence>
{
    friend class IProperty;

protected:
    explicit CSequenceProperty(EGame Game)
        : TTypedProperty(Game)
    {}

    void SerializeValue(void* pData, IArchive& rArc) const override
    {}
};

#endif // CSEQUENCEPROPERTY_H
