#ifndef CMODELLOADER_H
#define CMODELLOADER_H

#include <Common/EGame.h>
#include <Common/Flags.h>
#include <Common/Math/CAABox.h>
#include <Common/Math/CVector2f.h>
#include <Common/Math/CVector3f.h>
#include <Core/Resource/TResPtr.h>

#include <assimp/scene.h>

#include <memory>
#include <vector>

class CColor;
class CMaterialSet;
class CModel;
class CResourceEntry;
class CSectionMgrIn;
class IInputStream;
struct SSurface;

enum class EModelLoaderFlag
{
    None                    = 0x0,
    HalfPrecisionPositions  = 0x1,
    HalfPrecisionNormals    = 0x2,
    LightmapUVs             = 0x4,
    VisibilityGroups        = 0x8,
    Skinned                 = 0x10
};
DECLARE_FLAGS_ENUMCLASS(EModelLoaderFlag, FModelLoaderFlags)

class CModelLoader
{
private:
    TResPtr<CModel> mpModel;
    std::vector<CMaterialSet*> mMaterials;
    CSectionMgrIn *mpSectionMgr = nullptr;
    CAABox mAABox;
    EGame mVersion{};

    uint32_t mNumVertices = 0;
    std::vector<CVector3f> mPositions;
    std::vector<CVector3f> mNormals;
    std::vector<CColor> mColors;
    std::vector<CVector2f> mTex0;
    std::vector<CVector2f> mTex1;
    bool mSurfaceUsingTex1 = false;

    uint32_t mSurfaceCount = 0;
    std::vector<uint32_t> mSurfaceOffsets;

    FModelLoaderFlags mFlags{EModelLoaderFlag::None};

    CModelLoader();
    ~CModelLoader();

    void LoadWorldMeshHeader(IInputStream& rModel);
    void LoadAttribArrays(IInputStream& rModel);
    void LoadSurfaceOffsets(IInputStream& rModel);
    SSurface* LoadSurface(IInputStream& rModel);
    void LoadSurfaceHeaderPrime(IInputStream& rModel, SSurface *pSurf);
    void LoadSurfaceHeaderDKCR(IInputStream& rModel, SSurface *pSurf);
    SSurface* LoadAssimpMesh(const aiMesh *pkMesh, CMaterialSet *pSet);

public:
    static std::unique_ptr<CModel> LoadCMDL(IInputStream& rCMDL, CResourceEntry *pEntry);
    static std::unique_ptr<CModel> LoadWorldModel(IInputStream& rMREA, CSectionMgrIn& rBlockMgr, CMaterialSet& rMatSet, EGame Version);
    static std::unique_ptr<CModel> LoadCorruptionWorldModel(IInputStream& rMREA, CSectionMgrIn& rBlockMgr, CMaterialSet& rMatSet, uint32_t HeaderSecNum, uint32_t GPUSecNum, EGame Version);
    static void BuildWorldMeshes(std::vector<std::unique_ptr<CModel>>& rkIn, std::vector<std::unique_ptr<CModel>>& rOut, bool DeleteInputModels);
    static CModel* ImportAssimpNode(const aiNode *pkNode, const aiScene *pkScene, CMaterialSet& rMatSet);
    static EGame GetFormatVersion(uint32_t Version);
};

#endif // CMODELLOADER_H
