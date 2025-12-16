#ifndef IRENDERABLE_H
#define IRENDERABLE_H

#include "Core/Render/ERenderCommand.h"
#include "Core/Render/FRenderOptions.h"
#include "Core/Render/SViewInfo.h"

class CRenderer;

class IRenderable
{
public:
    IRenderable() = default;
    virtual ~IRenderable() = default;
    virtual void AddToRenderer(CRenderer* pRenderer, const SViewInfo& rkViewInfo) = 0;
    virtual void Draw(FRenderOptions /*Options*/, int /*ComponentIndex*/, ERenderCommand /*Command*/, const SViewInfo& /*rkViewInfo*/) {}
    virtual void DrawSelection() {}
};

#endif // IRENDERABLE_H
