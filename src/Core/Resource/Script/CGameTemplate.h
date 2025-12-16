#ifndef CGAMETEMPLATE_H
#define CGAMETEMPLATE_H

#include "Core/Resource/Script/CLink.h"
#include "Core/Resource/Script/CScriptTemplate.h"
#include "Core/Resource/Script/Property/Properties.h"
#include <Common/EGame.h>
#include <map>
#include <memory>

/** Serialization aid
 *  Retro switched from using integers to fourCCs to represent IDs in several cases (states/messages, object IDs).
 *  This struct is functionally an integer but it serializes as an int for MP1 and a fourCC for MP2 and on.
 */
struct SObjId
{
    union {
        uint32_t ID;
        CFourCC ID_4CC;
    };

    SObjId()                             {}
    SObjId(uint32_t InID)  : ID(InID)    {}
    SObjId(CFourCC InID) : ID_4CC(InID)  {}

    operator uint32_t() const  { return ID; }
    operator CFourCC() const { return ID_4CC; }

    void Serialize(IArchive& Arc)
    {
        if (Arc.Game() <= EGame::Prime)
            Arc.SerializePrimitive(ID, SH_HexDisplay);
        else
            Arc.SerializePrimitive(ID_4CC, 0);
    }
};

/** Struct holding a reference to a template */
template<typename TemplateT>
struct TTemplatePath
{
    /** File path to the template file, relative to the game directory */
    TString Path;

    /** Template in memory */
    std::shared_ptr<TemplateT> pTemplate;

    /** Constructor */
    TTemplatePath()
    {}

    TTemplatePath(const TString& kInPath, TemplateT* pInTemplate)
        : Path(kInPath)
        , pTemplate( std::shared_ptr<TemplateT>(pInTemplate) )
    {}

    /** Serializer */
    void Serialize(IArchive& Arc)
    {
        Arc << SerialParameter("Path", Path, SH_Attribute);
    }
};

using SScriptTemplatePath = TTemplatePath<CScriptTemplate>;
using SPropertyTemplatePath = TTemplatePath<IProperty>;

/** CGameTemplate - Per-game template data */
class CGameTemplate
{
    EGame mGame{};
    TString mSourceFile;
    bool mFullyLoaded = false;
    bool mDirty = false;

    /** Template arrays */
    std::map<SObjId,  SScriptTemplatePath>    mScriptTemplates;
    std::map<TString, SPropertyTemplatePath>  mPropertyTemplates;
    std::map<TString, SScriptTemplatePath>    mMiscTemplates;

    std::map<SObjId, TString> mStates;
    std::map<SObjId, TString> mMessages;

    /** Internal function for loading a property template from a file. */
    void Internal_LoadPropertyTemplate(SPropertyTemplatePath& Path);

public:
    CGameTemplate();
    void Serialize(IArchive& Arc);
    void Load(const TString& kFilePath);
    void Save();
    void SaveGameTemplates(bool ForceAll = false);

    uint32_t GameVersion(TString VersionName);
    CScriptTemplate* TemplateByID(uint32_t ObjectID);
    CScriptTemplate* TemplateByID(const CFourCC& ObjectID);
    CScriptTemplate* TemplateByIndex(uint32_t Index);
    SState StateByID(uint32_t StateID) const;
    SState StateByID(const CFourCC& StateID) const;
    SState StateByIndex(uint32_t Index) const;
    SMessage MessageByID(uint32_t MessageID) const;
    SMessage MessageByID(const CFourCC& MessageID) const;
    SMessage MessageByIndex(uint32_t Index) const;
    IProperty* FindPropertyArchetype(const TString& kTypeName);
    TString GetPropertyArchetypeFilePath(const TString& kTypeName) const;
    bool RenamePropertyArchetype(const TString& kTypeName, const TString& kNewTypeName);
    CScriptTemplate* FindMiscTemplate(const TString& kTemplateName);
    TString GetGameDirectory() const;

    // Accessors
    EGame Game() const                   { return mGame; }
    uint32_t NumScriptTemplates() const  { return mScriptTemplates.size(); }
    uint32_t NumStates() const           { return mStates.size(); }
    uint32_t NumMessages() const         { return mMessages.size(); }
    bool IsLoadedSuccessfully() const    { return mFullyLoaded; }
};

#endif // CGAMETEMPLATE_H
