#ifndef CSCANLOADER_H
#define CSCANLOADER_H

#include <Core/Resource/TResPtr.h>
#include <memory>

class CScan;

class CScanLoader
{
    TResPtr<CScan> mpScan;

    CScanLoader() = default;
    std::unique_ptr<CScan> LoadScanMP1(IInputStream& SCAN,  CResourceEntry* pEntry);
    std::unique_ptr<CScan> LoadScanMP2(IInputStream& SCAN,  CResourceEntry* pEntry);

public:
    static std::unique_ptr<CScan> LoadSCAN(IInputStream& rSCAN, CResourceEntry *pEntry);
};

#endif // CSCANLOADER_H
