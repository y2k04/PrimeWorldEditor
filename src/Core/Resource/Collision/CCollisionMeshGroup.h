#ifndef CCOLLISIONMESHGROUP_H
#define CCOLLISIONMESHGROUP_H

#include "Core/Resource/CResource.h"
#include "Core/Resource/Collision/CCollisionMesh.h"

#include <memory>
#include <span>
#include <vector>

class CCollisionMeshGroup : public CResource
{
    DECLARE_RESOURCE_TYPE(DynamicCollision)
    std::vector<std::unique_ptr<CCollisionMesh>> mMeshes;

public:
    explicit CCollisionMeshGroup(CResourceEntry *pEntry = nullptr) : CResource(pEntry) {}
    ~CCollisionMeshGroup() override = default;

    std::span<const std::unique_ptr<CCollisionMesh>> Meshes() const { return mMeshes; }
    void AddMesh(std::unique_ptr<CCollisionMesh>&& pMesh) { mMeshes.push_back(std::move(pMesh)); }

    void BuildRenderData()
    {
        for (auto& mesh : mMeshes)
            mesh->BuildRenderData();
    }

    void Draw()
    {
        for (auto& mesh : mMeshes)
            mesh->GetRenderData().Render(false);
    }

    void DrawWireframe()
    {
        for (auto& mesh : mMeshes)
            mesh->GetRenderData().Render(true);
    }
};

#endif // CCOLLISIONMESHGROUP_H
