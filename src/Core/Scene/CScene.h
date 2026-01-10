#ifndef CSCENE_H
#define CSCENE_H

#include "Core/Resource/TResPtr.h"
#include "Core/Resource/Script/CInstanceID.h"
#include <Core/Scene/ENodeType.h>
#include "Core/Scene/FShowFlags.h"

#include <cstdint>
#include <unordered_map>
#include <vector>

class CAreaAttributes;
class CCollisionNode;
class CCollisionMeshGroup;
class CGameArea;
class CLight;
class CLightNode;
class CModel;
class CModelNode;
class CRay;
class CRenderer;
class CRootNode;
class CSceneNode;
class CScriptNode;
class CScriptObject;
class CStaticNode;
class CStaticModel;
class CWorld;

struct SRayIntersection;
struct SViewInfo;

/** Needs lots of changes, see CSceneNode for most of my thoughts on this */
class CScene
{
    friend class CSceneIterator;
    
    bool mSplitTerrain = true;
    bool mRanPostLoad = false;

    uint32_t mNumNodes = 0;
    CRootNode *mpSceneRootNode = nullptr;
    std::unordered_map<ENodeType, std::vector<CSceneNode*>> mNodes;

    TResPtr<CGameArea> mpArea;
    TResPtr<CWorld> mpWorld;
    CRootNode *mpAreaRootNode = nullptr;

    // Environment
    std::vector<CAreaAttributes> mAreaAttributesObjects;

    // Node Management
    std::unordered_map<uint32_t, CSceneNode*> mNodeMap;
    std::unordered_map<CInstanceID, CScriptNode*> mScriptMap;

public:
    CScene();
    ~CScene();

    // Scene Management
    bool IsNodeIDUsed(uint32_t ID) const;
    uint32_t CreateNodeID(uint32_t SuggestedID = UINT32_MAX) const;

    CModelNode* CreateModelNode(CModel *pModel, uint32_t NodeID = UINT32_MAX);
    CStaticNode* CreateStaticNode(CStaticModel *pModel, uint32_t NodeID = UINT32_MAX);
    CCollisionNode* CreateCollisionNode(CCollisionMeshGroup *pMesh, uint32_t NodeID = UINT32_MAX);
    CScriptNode* CreateScriptNode(CScriptObject *pObj, uint32_t NodeID = UINT32_MAX);
    CLightNode* CreateLightNode(CLight *pLight, uint32_t NodeID = UINT32_MAX);
    void DeleteNode(CSceneNode *pNode);
    void SetActiveArea(CWorld *pWorld, CGameArea *pArea);
    void PostLoad();
    void ClearScene();
    void AddSceneToRenderer(CRenderer *pRenderer, const SViewInfo& rkViewInfo);
    SRayIntersection SceneRayCast(const CRay& rkRay, const SViewInfo& rkViewInfo);
    CSceneNode* NodeByID(uint32_t NodeID);
    CScriptNode* NodeForInstanceID(CInstanceID ID);
    CScriptNode* NodeForInstance(const CScriptObject *pObj);
    CLightNode* NodeForLight(const CLight *pLight);
    CModel* ActiveSkybox();
    CGameArea* ActiveArea();

    // Static
    static FShowFlags ShowFlagsForNodeFlags(FNodeFlags NodeFlags);
    static FNodeFlags NodeFlagsForShowFlags(FShowFlags ShowFlags);
};

#endif // CSCENE_H
