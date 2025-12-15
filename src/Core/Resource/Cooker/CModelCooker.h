#ifndef CMODELCOOKER_H
#define CMODELCOOKER_H

#include <Common/EGame.h>
#include "Core/Resource/TResPtr.h"
#include "Core/Resource/Model/EVertexAttribute.h"

class CModel;
class CVertex;
class IOutputStream;

class CModelCooker
{
    TResPtr<CModel> mpModel;
    EGame mVersion{};
    uint32_t mNumMatSets = 0;
    uint32_t mNumSurfaces = 0;
    uint32_t mNumVertices = 0;
    uint8_t mVertexFormat = 0;
    std::vector<CVertex> mVertices;
    FVertexDescription mVtxAttribs{};

    CModelCooker();
    ~CModelCooker();

    void GenerateSurfaceData();
    void WriteEditorModel(IOutputStream& rOut);
    void WriteModelPrime(IOutputStream& rOut);

public:
    static bool CookCMDL(CModel *pModel, IOutputStream& rOut);
    static uint32_t GetCMDLVersion(EGame Version);
};

#endif // CMODELCOOKER_H
