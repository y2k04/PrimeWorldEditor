#ifndef CBASICMODEL_H
#define CBASICMODEL_H

#include "SSurface.h"
#include "Core/Resource/CResource.h"
#include "Core/OpenGL/CVertexBuffer.h"
#include <Common/Math/CAABox.h>

class CBasicModel : public CResource
{
    DECLARE_RESOURCE_TYPE(Model)
protected:
    CAABox mAABox;
    uint32 mVertexCount = 0;
    uint32 mTriangleCount = 0;
    bool mBuffered = false;
    bool mHasOwnMaterials = false;
    bool mHasOwnSurfaces = false;

    CVertexBuffer mVBO;
    std::vector<SSurface*> mSurfaces;

public:
    explicit CBasicModel(CResourceEntry *pEntry = nullptr);
    ~CBasicModel() override;

    size_t GetVertexCount() const   { return mVertexCount; }
    size_t GetTriangleCount() const { return mTriangleCount; }
    const CAABox& AABox() const     { return mAABox; }
    bool IsBuffered() const         { return mBuffered; }
    size_t GetSurfaceCount() const  { return mSurfaces.size(); }
    const CAABox& GetSurfaceAABox(size_t Surface) const;
    SSurface* GetSurface(size_t Surface);
    const SSurface* GetSurface(size_t Surface) const;
    virtual void ClearGLBuffer() = 0;
};

#endif // CBASICMODEL_H
