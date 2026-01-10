#include "Core/Resource/Factory/CPoiToWorldLoader.h"

#include "Core/Resource/CPoiToWorld.h"
#include <Common/FileIO/IInputStream.h>

std::unique_ptr<CPoiToWorld> CPoiToWorldLoader::LoadEGMC(IInputStream& rEGMC, CResourceEntry *pEntry)
{
    auto pOut = std::make_unique<CPoiToWorld>(pEntry);
    const uint32_t NumMappings = rEGMC.ReadULong();

    for (uint32_t iMap = 0; iMap < NumMappings; iMap++)
    {
        const auto MeshID = rEGMC.ReadULong();
        const auto InstanceID = CInstanceID(rEGMC.ReadULong() & 0x03FFFFFF);
        pOut->AddPoiMeshMap(InstanceID, MeshID);
    }

    return pOut;
}
