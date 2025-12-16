#ifndef CMATERIALCOOKER_H
#define CMATERIALCOOKER_H

#include <Common/EGame.h>

#include <cstdint>
#include <vector>

class CMaterial;
class CMaterialSet;
class IOutputStream;

class CMaterialCooker
{
    CMaterialSet* mpSet = nullptr;
    CMaterial* mpMat = nullptr;
    EGame mVersion{};
    std::vector<uint32_t> mTextureIDs;
    std::vector<uint64_t> mMaterialHashes;

    CMaterialCooker();
    void WriteMatSetPrime(IOutputStream& rOut);
    void WriteMatSetCorruption(IOutputStream& rOut);
    void WriteMaterialPrime(IOutputStream& rOut);
    void WriteMaterialCorruption(IOutputStream& rOut);

public:
    static void WriteCookedMatSet(CMaterialSet* pSet, EGame Version, IOutputStream& rOut);
    static void WriteCookedMaterial(CMaterial* pMat, EGame Version, IOutputStream& rOut);
};

#endif // CMATERIALCOOKER_H
