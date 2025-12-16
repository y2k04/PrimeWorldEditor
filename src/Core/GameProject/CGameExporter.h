#ifndef CGAMEEXPORTER_H
#define CGAMEEXPORTER_H

#include "Core/GameProject/CAssetNameMap.h"
#include "Core/GameProject/CGameInfo.h"
#include "Core/GameProject/CGameProject.h"
#include "Core/GameProject/CResourceStore.h"
#include <Common/CAssetID.h>
#include <Common/Flags.h>
#include <Common/TString.h>
#include <map>
#include <memory>
#include <nod/DiscBase.hpp>

enum class EDiscType
{
    Normal,
    WiiDeAsobu,
    Trilogy
};

class CGameExporter
{
    // Project Data
    std::unique_ptr<CGameProject> mpProject;
    TString mProjectPath;
    CResourceStore *mpStore;
    EGame mGame;
    ERegion mRegion;
    TString mGameName;
    TString mGameID;
    float mBuildVersion;

    // Directories
    TString mExportDir;
    TString mDiscDir;
    TString mResourcesDir;

    TString mWorldsDirName;

    // Files
    nod::DiscBase *mpDisc = nullptr;
    EDiscType mDiscType;
    bool mFrontEnd;

    // Resources
    TStringList mPaks;
    std::map<CAssetID, bool> mAreaDuplicateMap;
    CAssetNameMap *mpNameMap = nullptr;
    CGameInfo *mpGameInfo = nullptr;

    struct SResourceInstance
    {
        TString PakFile;
        CAssetID ResourceID;
        CFourCC ResourceType;
        uint32_t PakOffset;
        uint32_t PakSize;
        bool Compressed;
        bool Exported;
    };
    std::map<CAssetID, SResourceInstance> mResourceMap;

    // Progress
    IProgressNotifier *mpProgress = nullptr;

public:
    enum EExportStep
    {
        eES_ExtractDisc,
        eES_ExportCooked,
        eES_GenerateRaw,
        // eES_Max must be last
        eES_NumSteps
    };

    CGameExporter(EDiscType DiscType, EGame Game, bool FrontEnd, ERegion Region, const TString& rkGameName, const TString& rkGameID, float BuildVersion);
    bool Export(nod::DiscBase *pDisc, const TString& rkOutputDir, CAssetNameMap *pNameMap, CGameInfo *pGameInfo, IProgressNotifier *pProgress);
    void LoadResource(const CAssetID& rkID, std::vector<uint8_t>& rBuffer);
    bool ShouldExportDiscNode(const nod::Node *pkNode, bool IsInRoot) const;

    const TString& ProjectPath() const  { return mProjectPath; }

protected:
    bool ExtractDiscData();
    bool ExtractDiscNodeRecursive(const nod::Node *pkNode, const TString& rkDir, bool RootNode, const nod::ExtractionContext& rkContext);
    void LoadPaks();
    void LoadResource(const SResourceInstance& rkResource, std::vector<uint8_t>& rBuffer);
    void ExportCookedResources();
    void ExportResourceEditorData();
    void ExportResource(SResourceInstance& rRes);
    TString MakeWorldName(const CAssetID& WorldID);

    // Convenience Functions
    SResourceInstance* FindResourceInstance(const CAssetID& rkID)
    {
        const uint64_t IntegralID = rkID.ToLongLong();
        const auto Found = mResourceMap.find(IntegralID);
        return (Found == mResourceMap.end() ? nullptr : &Found->second);
    }
};

#endif // CGAMEEXPORTER_H
