#ifndef CUNSUPPORTEDFORMATLOADER_H
#define CUNSUPPORTEDFORMATLOADER_H

#include <memory>

class CAudioMacro;
class CDependencyGroup;
class CMapArea;
class CResourceEntry;
class IInputStream;

// This class is responsible for loading formats that aren't yet fully supported.
// This is needed so we have access to the full dependency list of all resource types.
class CUnsupportedFormatLoader
{
public:
    CUnsupportedFormatLoader() = delete;

    static std::unique_ptr<CAudioMacro>      LoadCAUD(IInputStream& rCAUD, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadCSNG(IInputStream& rCSNG, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadDUMB(IInputStream& rDUMB, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadFRME(IInputStream& rFRME, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadFSM2(IInputStream& rFSM2, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadFSMC(IInputStream& rFSMC, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadHIER(IInputStream& rHIER, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadHINT(IInputStream& rHINT, CResourceEntry *pEntry);
    static std::unique_ptr<CMapArea>         LoadMAPA(IInputStream& rMAPA, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadMAPW(IInputStream& rMAPW, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadMAPU(IInputStream& rMAPU, CResourceEntry *pEntry);
    static std::unique_ptr<CDependencyGroup> LoadRULE(IInputStream& rRULE, CResourceEntry *pEntry);
};

#endif // CUNSUPPORTEDFORMATLOADER_H
