#include "Core/Resource/Cooker/CScanCooker.h"

#include <Common/CFourCC.h>
#include "Core/GameProject/DependencyListBuilders.h"
#include "Core/Resource/Cooker/CScriptCooker.h"
#include "Core/Resource/Scan/CScan.h"

#include <vector>

bool CScanCooker::CookSCAN(const CScan* pScan, IOutputStream& SCAN)
{
    // File header
    if (pScan->Game() <= EGame::Prime)
    {
        // We currently do not support cooking for the MP1 demo build
        ASSERT(pScan->Game() != EGame::PrimeDemo);
        SCAN.WriteLong(5);                // Version number; must be 5
        SCAN.WriteLong(0x0BADBEEF);       // SCAN magic

        const CStructRef ScanProperties = pScan->ScanData();
        CScriptCooker Cooker(pScan->Game());
        Cooker.WriteProperty(SCAN, ScanProperties.Property(), ScanProperties.DataPointer(), true);
    }
    else
    {
        SCAN.WriteFourCC(FOURCC('SCAN'));   // SCAN magic
        SCAN.WriteLong(2);                  // Version number; must be 2
        SCAN.WriteByte(1) ;                 // Layer version number; must be 1
        SCAN.WriteLong(1);                  // Instance count

        // Scans in MP2/3 are saved with the script object data format
        // Write a dummy script object header here
        SCAN.WriteLong(FOURCC('SNFO'));     // ScannableObjectInfo object ID
        const uint32_t ScanInstanceSizeOffset = SCAN.Tell();
        SCAN.WriteShort(0);                 // Object size
        SCAN.WriteLong(0);                  // Instance ID
        SCAN.WriteShort(0);                 // Link count

        const CStructRef ScanProperties = pScan->ScanData();
        CScriptCooker Cooker(pScan->Game());
        Cooker.WriteProperty(SCAN, ScanProperties.Property(), ScanProperties.DataPointer(), false);

        const uint32_t ScanInstanceEnd = SCAN.Tell();
        const uint32_t ScanInstanceSize = ScanInstanceEnd - ScanInstanceSizeOffset - 2;
        SCAN.GoTo(ScanInstanceSizeOffset);
        SCAN.WriteUShort(static_cast<uint16>(ScanInstanceSize));
        SCAN.GoTo(ScanInstanceEnd);

        // Write dependency list
        // @todo this output may not be 100% correct. Some dependencies seem to be conditionally excluded in base game.
        // This may cause some assets to be unnecessarily loaded into memory ingame.
        std::vector<CAssetID> Dependencies;
        CAssetDependencyListBuilder Builder(pScan->Entry());
        Builder.BuildDependencyList(Dependencies);
        SCAN.WriteULong(static_cast<uint32_t>(Dependencies.size()));

        for (const CAssetID& kID : Dependencies)
        {
            CResourceEntry* pEntry = pScan->Entry()->ResourceStore()->FindEntry(kID);
            ASSERT(pEntry != nullptr);

            pEntry->CookedExtension().Write(SCAN);
            kID.Write(SCAN);
        }
    }

    return true;
}
