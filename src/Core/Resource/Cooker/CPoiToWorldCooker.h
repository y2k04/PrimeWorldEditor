#ifndef CPOITOWORLDCOOKER_H
#define CPOITOWORLDCOOKER_H

class CPoiToWorld;
class IOutputStream;

class CPoiToWorldCooker
{
public:
    CPoiToWorldCooker() = delete;
    static bool CookEGMC(const CPoiToWorld* pPoiToWorld, IOutputStream& rOut);
};

#endif // CPOITOWORLDCOOKER_H
