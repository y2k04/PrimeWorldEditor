#ifndef CRENDERBUCKET_H
#define CRENDERBUCKET_H

#include <cstdint>
#include <vector>

class CCamera;
struct SRenderablePtr;
struct SViewInfo;

class CRenderBucket
{
    bool mEnableDepthSortDebugVisualization = false;

    class CSubBucket
    {
        std::vector<SRenderablePtr> mRenderables;
        uint32_t mEstSize = 0;
        uint32_t mSize = 0;

    public:
        CSubBucket();
        ~CSubBucket();

        void Add(const SRenderablePtr &rkPtr);
        void Sort(const CCamera *pkCamera, bool DebugVisualization);
        void Clear();
        void Draw(const SViewInfo& rkViewInfo);
    };

    CSubBucket mOpaqueSubBucket;
    CSubBucket mTransparentSubBucket;

public:
    CRenderBucket();
    ~CRenderBucket();

    void Add(const SRenderablePtr& rkPtr, bool Transparent);
    void Clear();
    void Draw(const SViewInfo& rkViewInfo);
};

#endif // CRENDERBUCKET_H
