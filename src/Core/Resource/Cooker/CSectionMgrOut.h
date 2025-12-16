#ifndef CBLOCKMGROUT_H
#define CBLOCKMGROUT_H

#include <Common/FileIO/IOutputStream.h>
#include <cstdint>
#include <vector>

// Small class to manage file sections for CMDL/MREA output
class CSectionMgrOut
{
    uint32_t mSectionCount = 0;
    uint32_t mCurSectionStart = 0;
    uint32_t mCurSectionIndex = 0;
    std::vector<uint32_t> mSectionSizes;

public:
    CSectionMgrOut() = default;

    void SetSectionCount(uint32_t Count)
    {
        mSectionCount = Count;
        mSectionSizes.resize(Count);
    }

    void Init(const IOutputStream& rOut)
    {
        mCurSectionStart = rOut.Tell();
        mCurSectionIndex = 0;
    }

    void AddSize(IOutputStream& rOut)
    {
        mSectionSizes[mCurSectionIndex] = rOut.Tell() - mCurSectionStart;
        mCurSectionIndex++;
        mCurSectionStart = rOut.Tell();
    }

    void WriteSizes(IOutputStream& rOut)
    {
        for (size_t iSec = 0; iSec < mSectionCount; iSec++)
            rOut.WriteULong(mSectionSizes[iSec]);
    }
};

#endif // CBLOCKMGROUT_H
