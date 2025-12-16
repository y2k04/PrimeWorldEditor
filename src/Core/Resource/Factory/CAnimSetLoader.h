#ifndef CCHARACTERLOADER_H
#define CCHARACTERLOADER_H

#include <Common/EGame.h>
#include <Core/Resource/TResPtr.h>

#include <cstdint>
#include <memory>

class CAnimSet;
class CResourceEntry;
class CSourceAnimData;
class IInputStream;
struct SSetCharacter;

class CAnimSetLoader
{
    TResPtr<CAnimSet> pSet;
    EGame mGame{};

    CAnimSetLoader();
    void LoadCorruptionCHAR(IInputStream& rCHAR);
    void LoadReturnsCHAR(IInputStream& rCHAR);
    void LoadPASDatabase(IInputStream& rPAS4);
    void LoadParticleResourceData(IInputStream& rFile, SSetCharacter *pChar, uint16_t Version);

    void LoadAnimationSet(IInputStream& rANCS);
    void ProcessPrimitives();

public:
    static std::unique_ptr<CAnimSet> LoadANCS(IInputStream& rANCS, CResourceEntry *pEntry);
    static std::unique_ptr<CAnimSet> LoadCHAR(IInputStream& rCHAR, CResourceEntry *pEntry);
    static std::unique_ptr<CSourceAnimData> LoadSAND(IInputStream& rSAND, CResourceEntry *pEntry);
};

#endif // CCHARACTERLOADER_H
