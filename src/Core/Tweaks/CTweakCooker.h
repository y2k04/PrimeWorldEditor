#ifndef CTWEAKCOOKER_H
#define CTWEAKCOOKER_H

#include <vector>

class CTweakData;
class IOutputStream;

/** Class responsible for cooking tweak data */
class CTweakCooker
{
public:
    CTweakCooker() = delete;

    /** Cooker entry point */
    static bool CookCTWK(const CTweakData* pTweakData, IOutputStream& CTWK);
    static bool CookNTWK(const std::vector<CTweakData*>& kTweaks, IOutputStream& NTWK);
};

#endif // CTWEAKCOOKER_H
