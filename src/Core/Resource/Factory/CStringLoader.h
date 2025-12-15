#ifndef CSTRINGLOADER_H
#define CSTRINGLOADER_H

#include <Common/EGame.h>
#include <memory>

class CResourceEntry;
class CStringTable;
class IInputStream;

class CStringLoader
{
    CStringTable *mpStringTable = nullptr;
    EGame mVersion{};

    CStringLoader() = default;
    void LoadPrimeDemoSTRG(IInputStream& STRG);
    void LoadPrimeSTRG(IInputStream& STRG);
    void LoadCorruptionSTRG(IInputStream& STRG);
    void LoadNameTable(IInputStream& STRG);

public:
    static std::unique_ptr<CStringTable> LoadSTRG(IInputStream& STRG, CResourceEntry* pEntry);
    static EGame GetFormatVersion(uint Version);
};

#endif // CSTRINGLOADER_H
