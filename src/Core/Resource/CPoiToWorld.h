#ifndef CPOITOWORLD_H
#define CPOITOWORLD_H

#include "Core/Resource/CResource.h"
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
        uint32_t PoiID;
        std::list<uint32_t> ModelIDs;
    };

private:
    std::vector<std::unique_ptr<SPoiMap>> mMaps;
    std::map<uint32_t, SPoiMap*> mPoiLookupMap;

public:
    explicit CPoiToWorld(CResourceEntry *pEntry = nullptr);
    ~CPoiToWorld() override;

    void AddPoi(uint32_t PoiID);
    void AddPoiMeshMap(uint32_t PoiID, uint32_t ModelID);
    void RemovePoi(uint32_t PoiID);
    void RemovePoiMeshMap(uint32_t PoiID, uint32_t ModelID);

    size_t NumMappedPOIs() const
    {
        return mMaps.size();
    }

    const SPoiMap* MapByIndex(size_t Index) const
    {
        return mMaps[Index].get();
    }

    const SPoiMap* MapByID(uint32_t InstanceID) const
    {
        auto it = mPoiLookupMap.find(InstanceID);

        if (it != mPoiLookupMap.end())
            return it->second;
        else
            return nullptr;
    }

    bool HasPoiMappings(uint32_t InstanceID) const
    {
        auto it = mPoiLookupMap.find(InstanceID);
        return (it != mPoiLookupMap.end());
    }
};

#endif // CPOITOWORLD_H
