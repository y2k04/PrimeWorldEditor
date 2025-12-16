#ifndef CTEXTUREDECODER_H
#define CTEXTUREDECODER_H

#include "Core/Resource/ETexelFormat.h"
#include <Common/FileIO/CMemoryInStream.h>

#include <memory>
#include <vector>

class CColor;
class CResourceEntry;
class CTexture;
class IInputStream;
class IOutputStream;

class CTextureDecoder
{
    CResourceEntry *mpEntry;
    ETexelFormat mTexelFormat;
    uint16_t mWidth, mHeight;
    uint32_t mNumMipMaps;

    bool mHasPalettes;
    EGXPaletteFormat mPaletteFormat;
    std::vector<uint8_t> mPalettes;
    CMemoryInStream mPaletteInput;

    struct SDDSInfo
    {
        enum { DXT1, DXT2, DXT3, DXT4, DXT5, RGBA } Format;
        uint32_t Flags;
        uint32_t BitCount;
        uint32_t RBitMask, GBitMask, BBitMask, ABitMask;
        uint32_t RShift, GShift, BShift, AShift;
        uint32_t RSize, GSize, BSize, ASize;
    } mDDSInfo;

    uint8_t *mpDataBuffer;
    uint32_t mDataBufferSize;

    // Private Functions
    CTextureDecoder();
    ~CTextureDecoder();
    std::unique_ptr<CTexture> CreateTexture();

    // Read
    void ReadTXTR(IInputStream& rTXTR);
    void ReadDDS(IInputStream& rDDS);

    // Decode
    void PartialDecodeGXTexture(IInputStream& rTXTR);
    void FullDecodeGXTexture(IInputStream& rTXTR);
    void DecodeDDS(IInputStream& rDDS);

    // Decode Pixels (preserve compression)
    void ReadPixelsI4(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelI8(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelIA4(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelIA8(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelsC4(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelC8(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelRGB565(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelRGB5A3(IInputStream& rSrc, IOutputStream& rDst);
    void ReadPixelRGBA8(IInputStream& rSrc, IOutputStream& rDst);
    void ReadSubBlockCMPR(IInputStream& rSrc, IOutputStream& rDst);

    // Decode Pixels (convert to RGBA8)
    CColor DecodePixelI4(uint8_t Byte, uint8_t WhichPixel);
    CColor DecodePixelI8(uint8_t Byte);
    CColor DecodePixelIA4(uint8_t Byte);
    CColor DecodePixelIA8(uint16_t Short);
    CColor DecodePixelC4(uint8_t Byte, uint8_t WhichPixel, IInputStream& rPaletteStream);
    CColor DecodePixelC8(uint8_t Byte, IInputStream& rPaletteStream);
    CColor DecodePixelRGB565(uint16_t Short);
    CColor DecodePixelRGB5A3(uint16_t Short);
    void DecodeSubBlockCMPR(IInputStream& rSrc, IOutputStream& rDst, uint16_t Width);

    void DecodeBlockBC1(IInputStream& rSrc, IOutputStream& rDst, uint32_t Width);
    void DecodeBlockBC2(IInputStream& rSrc, IOutputStream& rDst, uint32_t Width);
    void DecodeBlockBC3(IInputStream& rSrc, IOutputStream& rDst, uint32_t Width);
    CColor DecodeDDSPixel(IInputStream& rDDS);

    // Static
public:
    static std::unique_ptr<CTexture> LoadTXTR(IInputStream& rTXTR, CResourceEntry *pEntry);
    static std::unique_ptr<CTexture> LoadDDS(IInputStream& rDDS, CResourceEntry *pEntry);
    static std::unique_ptr<CTexture> DoFullDecode(IInputStream& rTXTR, CResourceEntry *pEntry);
    static CTexture* DoFullDecode(CTexture *pTexture);
};

#endif // CTEXTUREDECODER_H
