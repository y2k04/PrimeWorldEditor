#include "Core/Resource/Model/EVertexAttribute.h"

#include <Common/Log.h>

const uint32_t gkNumVertexAttribs = 22;

uint32_t VertexAttributeSize(EVertexAttribute Attrib)
{
    switch (Attrib)
    {
    case EVertexAttribute::Position:
    case EVertexAttribute::Normal:
        return 0x0C;
    case EVertexAttribute::Color0:
    case EVertexAttribute::Color1:
    case EVertexAttribute::BoneWeights:
        return 0x10;
    case EVertexAttribute::Tex0:
    case EVertexAttribute::Tex1:
    case EVertexAttribute::Tex2:
    case EVertexAttribute::Tex3:
    case EVertexAttribute::Tex4:
    case EVertexAttribute::Tex5:
    case EVertexAttribute::Tex6:
    case EVertexAttribute::Tex7:
        return 0x08;
    case EVertexAttribute::BoneIndices:
        return 0x04;
    default:
        NLog::Error("AttributeSize(): Unknown vertex attribute: {}", int(Attrib));
        return 0x00;
    }
}
