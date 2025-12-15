#ifndef CWORLDCOOKER_H
#define CWORLDCOOKER_H

#include <Common/EGame.h>

class CWorld;
class IOutputStream;

class CWorldCooker
{
    CWorldCooker();
public:
    static bool CookMLVL(CWorld *pWorld, IOutputStream& rOut);
    static uint32_t GetMLVLVersion(EGame Version);
};

#endif // CWORLDCOOKER_H
