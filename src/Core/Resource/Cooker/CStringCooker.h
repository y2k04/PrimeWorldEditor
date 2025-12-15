#ifndef CSTRINGCOOKER_H
#define CSTRINGCOOKER_H

#include "Core/Resource/TResPtr.h"

class CStringTable;
class IOutputStream;

/** Cooker class for writing game-compatible STRG assets */
class CStringCooker
{
    TResPtr<CStringTable> mpStringTable;

    explicit CStringCooker(CStringTable* pStringTable);

public:
    void WritePrimeDemoSTRG(IOutputStream& STRG);
    void WritePrimeSTRG(IOutputStream& STRG);
    void WriteCorruptionSTRG(IOutputStream& STRG);
    void WriteNameTable(IOutputStream& STRG);

    /** Static entry point */
    static bool CookSTRG(CStringTable* pStringTable, IOutputStream& STRG);
};

#endif // CSTRINGCOOKER_H
