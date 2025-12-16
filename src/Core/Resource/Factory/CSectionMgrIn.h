#ifndef CSECTIONMGRIN_H
#define CSECTIONMGRIN_H

#include <Common/FileIO/IInputStream.h>
#include <cstdint>
#include <vector>

// The purpose of this class is to keep track of data block navigation - required to read CMDL and MREA files correctly
class CSectionMgrIn
{
    IInputStream *mpInputStream;
    std::vector<uint32_t> mSectionSizes;
    uint32_t mCurSec = 0;
    uint32_t mCurSecStart = 0;
    uint32_t mSecsStart = 0;

public:
    CSectionMgrIn(size_t Count, IInputStream* pSrc)
        : mpInputStream(pSrc), mSectionSizes(Count)
    {
        for (auto& size : mSectionSizes)
            size = pSrc->ReadULong();
    }

    void Init()
    {
        // Initializes the block manager and marks the start of the first block
        mCurSec = 0;
        mCurSecStart = mpInputStream->Tell();
        mSecsStart = mCurSecStart;
    }

    void ToSection(uint32_t SecNum)
    {
        uint32_t Offset = mSecsStart;
        for (uint32_t iSec = 0; iSec < SecNum; iSec++)
            Offset += mSectionSizes[iSec];

        mpInputStream->Seek(Offset, SEEK_SET);
        mCurSec = SecNum;
        mCurSecStart = mpInputStream->Tell();
    }

    void ToNextSection()
    {
        mCurSecStart += mSectionSizes[mCurSec];
        mpInputStream->Seek(mCurSecStart, SEEK_SET);
        mCurSec++;
    }

    uint32_t NextOffset() const              { return mCurSecStart + mSectionSizes[mCurSec]; }
    uint32_t CurrentSection() const          { return mCurSec; }
    uint32_t CurrentSectionSize() const      { return mSectionSizes[mCurSec]; }
    size_t NumSections() const               { return mSectionSizes.size(); }
    void SetInputStream(IInputStream *pIn)   { mpInputStream = pIn; }
};

#endif // CSECTIONMGRIN_H
