#ifndef CSPLINEPROPERTY_H
#define CSPLINEPROPERTY_H

#include "Core/Resource/Script/Property/IProperty.h"
#include <vector>

class CSplineProperty : public TTypedProperty<std::vector<char>, EPropertyType::Spline>
{
    friend class IProperty;

protected:
    explicit CSplineProperty(EGame Game)
        : TTypedProperty(Game)
    {}

public:
    void SerializeValue(void* pData, IArchive& Arc) const override
    {
        Arc << SerialParameter("Data", ValueRef(pData));
    }
};

#endif // CSPLINEPROPERTY_H
