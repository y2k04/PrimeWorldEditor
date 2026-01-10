#ifndef CSCRIPTOBJECT_H
#define CSCRIPTOBJECT_H

#include <Common/Math/CVector3f.h>
#include "Core/Resource/TResPtr.h"
#include "Core/Resource/Script/EVolumeShape.h"
#include "Core/Resource/Script/CInstanceID.h"
#include "Core/Resource/Script/Property/TPropertyRef.h"

#include <cstdint>
#include <vector>

class CCollisionMeshGroup;
class CGameArea;
class CGameTemplate;
class CLink;
class CScriptLayer;
class CScriptTemplate;

enum class ELinkType
{
    Incoming,
    Outgoing
};

class CScriptObject
{
    friend class CScriptLoader;
    friend class CAreaLoader;

    CScriptTemplate *mpTemplate;
    CGameArea *mpArea;
    CScriptLayer *mpLayer;
    uint32_t mVersion = 0;

    CInstanceID mInstanceID;
    std::vector<CLink*> mOutLinks;
    std::vector<CLink*> mInLinks;
    std::vector<char> mPropertyData;

    CStringRef mInstanceName;
    CVectorRef mPosition;
    CVectorRef mRotation;
    CVectorRef mScale;
    CBoolRef mActive;
    CStructRef mLightParameters;

    TResPtr<CResource> mpDisplayAsset;
    TResPtr<CCollisionMeshGroup> mpCollision;
    uint32_t mActiveCharIndex = 0;
    uint32_t mActiveAnimIndex = 0;
    bool mHasInGameModel = false;

    EVolumeShape mVolumeShape{};
    float mVolumeScale = 0.0f;

    // Recursion guard
    mutable bool mIsCheckingNearVisibleActivation = false;

public:
    CScriptObject(CInstanceID ID, CGameArea *pArea, CScriptLayer *pLayer, CScriptTemplate *pTemplate);
    ~CScriptObject();

    void CopyProperties(CScriptObject* pObject);
    void EvaluateProperties();
    void EvaluateDisplayAsset();
    void EvaluateCollisionModel();
    void EvaluateVolume();
    bool IsEditorProperty(const IProperty *pProp) const;
    void SetLayer(CScriptLayer *pLayer, uint32_t NewLayerIndex = UINT32_MAX);
    uint32_t LayerIndex() const;
    bool HasNearVisibleActivation() const;

    void AddLink(ELinkType Type, CLink *pLink, uint32_t Index = UINT32_MAX);
    void RemoveLink(ELinkType Type, CLink *pLink);
    void BreakAllLinks();

    // Accessors
    CScriptTemplate* Template() const               { return mpTemplate; }
    CGameTemplate* GameTemplate() const;
    CGameArea* Area() const                         { return mpArea; }
    CScriptLayer* Layer() const                     { return mpLayer; }
    uint32_t Version() const                        { return mVersion; }
    uint32_t ObjectTypeID() const;
    CInstanceID InstanceID() const                  { return mInstanceID; }
    size_t NumLinks(ELinkType Type) const           { return (Type == ELinkType::Incoming ? mInLinks.size() : mOutLinks.size()); }
    CLink* Link(ELinkType Type, size_t Index) const { return (Type == ELinkType::Incoming ? mInLinks[Index] : mOutLinks[Index]); }
    void* PropertyData()                            { return mPropertyData.data(); }
    const void* PropertyData() const                { return mPropertyData.data(); }

    CVector3f Position() const                  { return mPosition.IsValid() ? mPosition.Get() : CVector3f::Zero(); }
    CVector3f Rotation() const                  { return mRotation.IsValid() ? mRotation.Get() : CVector3f::Zero(); }
    CVector3f Scale() const                     { return mScale.IsValid() ? mScale.Get() : CVector3f::One(); }
    TString InstanceName() const                { return mInstanceName.IsValid() ? mInstanceName.Get() : ""; }
    bool IsActive() const                       { return mActive.IsValid() ? mActive.Get() : false; }
    bool HasInGameModel() const                 { return mHasInGameModel; }
    CStructRef LightParameters() const          { return mLightParameters; }
    CResource* DisplayAsset() const             { return mpDisplayAsset; }
    uint32_t ActiveCharIndex() const            { return mActiveCharIndex; }
    uint32_t ActiveAnimIndex() const            { return mActiveAnimIndex; }
    CCollisionMeshGroup* Collision() const      { return mpCollision; }
    EVolumeShape VolumeShape() const            { return mVolumeShape; }
    float VolumeScale() const                   { return mVolumeScale; }
    void SetPosition(const CVector3f& rkNewPos) { mPosition.Set(rkNewPos); }
    void SetRotation(const CVector3f& rkNewRot) { mRotation.Set(rkNewRot); }
    void SetScale(const CVector3f& rkNewScale)  { mScale.Set(rkNewScale); }
    void SetName(const TString& rkNewName)      { mInstanceName.Set(rkNewName); }
    void SetActive(bool Active)                 { mActive.Set(Active); }

    bool HasPosition() const        { return mPosition.IsValid(); }
    bool HasRotation() const        { return mRotation.IsValid(); }
    bool HasScale() const           { return mScale.IsValid(); }
    bool HasInstanceName() const    { return mInstanceName.IsValid(); }
    bool HasActive() const          { return mActive.IsValid(); }
    bool HasLightParameters() const { return mLightParameters.IsValid(); }
};

#endif // CSCRIPTOBJECT_H
