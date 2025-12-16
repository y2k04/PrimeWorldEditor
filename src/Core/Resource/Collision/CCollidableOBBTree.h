#ifndef CCOLLIDABLEOBBTREE_H
#define CCOLLIDABLEOBBTREE_H

#include "Core/Resource/Collision/CCollisionMesh.h"
#include "Core/Resource/Collision/SOBBTreeNode.h"

#include <memory>

/** A collision mesh with an OBB tree for spatial queries. Represents one mesh from a DCLN file */
class CCollidableOBBTree : public CCollisionMesh
{
    friend class CCollisionLoader;
    std::unique_ptr<SOBBTreeNode> mpOBBTree;

public:
    void BuildRenderData() override;

    void BuildOBBTree();

    /** Accessors */
    SOBBTreeNode* GetOBBTree() const
    {
        return mpOBBTree.get();
    }
};

#endif // CCOLLIDABLEOBBTREE_H
