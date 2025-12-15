#ifndef CPOITOWORLDLOADER_H
#define CPOITOWORLDLOADER_H

#include "Core/Resource/TResPtr.h"
#include <memory>

class CPoiToWorld;

class CPoiToWorldLoader
{
    CPoiToWorldLoader() = delete;

public:
    static std::unique_ptr<CPoiToWorld> LoadEGMC(IInputStream& rEGMC, CResourceEntry *pEntry);
};

#endif // CPOITOWORLDLOADER_H
