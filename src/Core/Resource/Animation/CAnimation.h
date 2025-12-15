#ifndef CANIMATION_H
#define CANIMATION_H

#include "Core/Resource/CResource.h"
#include "Core/Resource/TResPtr.h"

#include <array>
#include <cstdint>
#include <vector>

class CAnimEventData;
class CQuaternion;
class CVector3f;

class CAnimation : public CResource
{
    DECLARE_RESOURCE_TYPE(Animation)
    friend class CAnimationLoader;

    using TScaleChannel = std::vector<CVector3f>;
    using TRotationChannel = std::vector<CQuaternion>;
    using TTranslationChannel = std::vector<CVector3f>;

    float mDuration = 0.0f;
    float mTickInterval = 0.0333333f;
    uint32_t mNumKeys = 0;

    std::vector<TScaleChannel> mScaleChannels;
    std::vector<TRotationChannel> mRotationChannels;
    std::vector<TTranslationChannel> mTranslationChannels;

    struct SBoneChannelInfo
    {
        uint8_t ScaleChannelIdx = 0xFF;
        uint8_t RotationChannelIdx = 0xFF;
        uint8_t TranslationChannelIdx = 0xFF;
    };
    std::array<SBoneChannelInfo, 100> mBoneInfo;

    TResPtr<CAnimEventData> mpEventData;

public:
    explicit CAnimation(CResourceEntry *pEntry = nullptr);
    ~CAnimation() override;

    std::unique_ptr<CDependencyTree> BuildDependencyTree() const override;
    void EvaluateTransform(float Time, uint32_t BoneID, CVector3f *pOutTranslation, CQuaternion *pOutRotation, CVector3f *pOutScale) const;
    bool HasTranslation(uint32_t BoneID) const;

    float Duration() const               { return mDuration; }
    uint32_t NumKeys() const             { return mNumKeys; }
    float TickInterval() const           { return mTickInterval; }
    CAnimEventData* EventData() const    { return mpEventData; }
};

#endif // CANIMATION_H
