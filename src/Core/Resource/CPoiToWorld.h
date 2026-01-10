#ifndef CPOITOWORLD_H
#define CPOITOWORLD_H

#include "Core/Resource/CResource.h"
#include "Core/Resource/Script/CInstanceID.h"
#include <cstdint>
#include <list>
#include <map>
#include <memory>
#include <vector>

class CPoiToWorld : public CResource
{
    DECLARE_RESOURCE_TYPE(StaticGeometryMap)

public:
    struct SPoiMap
    {
        CInstanceID PoiID;
        std::list<uint32_t> ModelIDs;
    };

private:
    std::vector<std::unique_ptr<SPoiMap>> mMaps;
    std::map<CInstanceID, SPoiMap*> mPoiLookupMap;

public:
    explicit CPoiToWorld(CResourceEntry *pEntry = nullptr);
    ~CPoiToWorld() override;

    void AddPoi(CInstanceID PoiID);
    void AddPoiMeshMap(CInstanceID PoiID, uint32_t ModelID);
    void RemovePoi(CInstanceID PoiID);
    void RemovePoiMeshMap(CInstanceID PoiID, uint32_t ModelID);

    size_t NumMappedPOIs() const
    {
        return mMaps.size();
    }

    const SPoiMap* MapByIndex(size_t Index) const
    {
        return mMaps[Index].get();
    }

    const SPoiMap* MapByID(CInstanceID InstanceID) const
    {
        auto it = mPoiLookupMap.find(InstanceID);
        if (it == mPoiLookupMap.end())
            return nullptr;

        return it->second;
    }

    bool HasPoiMappings(CInstanceID InstanceID) const
    {
        return mPoiLookupMap.contains(InstanceID);
    }
};

#endif // CPOITOWORLD_H
