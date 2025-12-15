#ifndef CASSETNAMEMAP
#define CASSETNAMEMAP

#include <Common/CAssetID.h>
#include <Common/TString.h>

#include <map>
#include <set>

class CResourceStore;
class IArchive;

class CAssetNameMap
{
    struct SAssetNameInfo;

    std::set<SAssetNameInfo> mUsedSet; // Used to prevent name conflicts
    std::map<CAssetID, SAssetNameInfo> mMap;
    bool mIsValid = true;
    EIDLength mIDLength;

    // Private Methods
    void Serialize(IArchive& rArc);
    void PostLoadValidate();

public:
    explicit CAssetNameMap(EIDLength IDLength);
    explicit CAssetNameMap(EGame Game);
    ~CAssetNameMap();

    bool LoadAssetNames(TString Path = "");
    bool SaveAssetNames(TString Path = "");
    bool GetNameInfo(CAssetID ID, TString& rOutDirectory, TString& rOutName, bool& rOutAutoGenDir, bool& rOutAutoGenName);
    void CopyFromStore(CResourceStore *pStore);

    static TString DefaultNameMapPath(EIDLength IDLength);
    static TString DefaultNameMapPath(EGame Game);

    bool IsValid() const { return mIsValid; }
    static TString GetExtension();
};

#endif // CASSETNAMEMAP

