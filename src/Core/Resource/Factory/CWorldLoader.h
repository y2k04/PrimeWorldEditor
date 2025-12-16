#ifndef CWORLDLOADER_H
#define CWORLDLOADER_H

#include <Common/EGame.h>
#include <Core/Resource/TResPtr.h>
#include <memory>

class CResourceEntry;
class CWorld;
class IInputStream;

class CWorldLoader
{
    TResPtr<CWorld> mpWorld;
    EGame mVersion{};

    CWorldLoader();
    void LoadPrimeMLVL(IInputStream& rMLVL);
    void LoadReturnsMLVL(IInputStream& rMLVL);
    void GenerateEditorData();

public:
    static std::unique_ptr<CWorld> LoadMLVL(IInputStream& rMLVL, CResourceEntry *pEntry);
    static EGame GetFormatVersion(uint32_t Version);
};

#endif // CWORLDLOADER_H
