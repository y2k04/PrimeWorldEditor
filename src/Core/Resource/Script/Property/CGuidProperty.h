#ifndef CGUIDPROPERTY_H
#define CGUIDPROPERTY_H

#include "Core/Resource/Script/Property/IProperty.h"
#include <vector>

class CGuidProperty : public TTypedProperty<std::vector<char>, EPropertyType::Guid>
{
    friend class IProperty;

protected:
    explicit CGuidProperty(EGame Game)
        : TTypedProperty(Game)
    {}

public:
    void SerializeValue(void* pData, IArchive& Arc) const override
    {
        Arc << SerialParameter("Data", ValueRef(pData));
    }
};

#endif // CSPLINEPROPERTY_H
