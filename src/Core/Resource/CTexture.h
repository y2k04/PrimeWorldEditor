#ifndef CTEXTURE_H
#define CTEXTURE_H

#include "Core/Resource/CResource.h"
#include "Core/Resource/ETexelFormat.h"

#include <memory>

#include <GL/glew.h>

class CVector2f;

class CTexture : public CResource
{
    DECLARE_RESOURCE_TYPE(Texture)
    friend class CTextureDecoder;
    friend class CTextureEncoder;

    ETexelFormat mTexelFormat{ETexelFormat::RGBA8};       // Format of decoded image data
    ETexelFormat mSourceTexelFormat{ETexelFormat::RGBA8}; // Format of input TXTR file
    uint16_t mWidth = 0;                                    // Image width
    uint16_t mHeight = 0;                                   // Image height
    uint32_t mNumMipMaps = 0;                               // The number of mipmaps this texture has
    uint32_t mLinearSize = 0;                               // The size of the top level mipmap, in bytes

    bool mEnableMultisampling = false;  // Whether multisample should be enabled (if this texture is a render target).
    bool mBufferExists = false;         // Indicates whether image data buffer has valid data
    std::unique_ptr<uint8_t[]> mpImgDataBuffer; // Pointer to image data buffer
    uint32_t mImgDataSize = 0;                  // Size of image data buffer

    bool mGLBufferExists = false; // Indicates whether GL buffer has valid data
    GLuint mTextureID = 0;        // ID for texture GL buffer

public:
    explicit CTexture(CResourceEntry *pEntry = nullptr);
    CTexture(uint32_t Width, uint32_t Height);
    ~CTexture() override;

    bool BufferGL();
    void Bind(uint32_t GLTextureUnit);
    void Resize(uint32_t Width, uint32_t Height);
    float ReadTexelAlpha(const CVector2f& rkTexCoord);
    bool WriteDDS(IOutputStream& rOut);

    // Accessors
    ETexelFormat TexelFormat() const        { return mTexelFormat; }
    ETexelFormat SourceTexelFormat() const  { return mSourceTexelFormat; }
    uint32_t Width() const                  { return (uint32_t) mWidth; }
    uint32_t Height() const                 { return (uint32_t) mHeight; }
    uint32_t NumMipMaps() const             { return mNumMipMaps; }
    GLuint TextureID() const                { return mTextureID; }

    void SetMultisamplingEnabled(bool Enable)
    {
        if (mEnableMultisampling != Enable)
            DeleteBuffers();

        mEnableMultisampling = Enable;
    }

    // Static
    static uint32_t FormatBPP(ETexelFormat Format);

    // Private
private:
    void CalcLinearSize();
    uint32_t CalcTotalSize() const;
    void CopyGLBuffer();
    void DeleteBuffers();
};

#endif // CTEXTURE_H
