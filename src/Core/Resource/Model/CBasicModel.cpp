#include "CBasicModel.h"

CBasicModel::CBasicModel(CResourceEntry *pEntry)
    : CResource(pEntry)
{
}

CBasicModel::~CBasicModel()
{
    if (!mHasOwnSurfaces)
        return;

    for (auto* surface : mSurfaces)
        delete surface;
}

const CAABox& CBasicModel::GetSurfaceAABox(size_t Surface) const
{
    return GetSurface(Surface)->AABox;
}

SSurface* CBasicModel::GetSurface(size_t Surface)
{
    return mSurfaces[Surface];
}

const SSurface* CBasicModel::GetSurface(size_t Surface) const
{
    return mSurfaces[Surface];
}
