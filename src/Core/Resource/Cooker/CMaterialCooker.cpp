#include "Core/Resource/Cooker/CMaterialCooker.h"

#include "Core/Resource/CMaterial.h"
#include "Core/Resource/CMaterialSet.h"
#include <Core/Resource/Model/EVertexAttribute.h>

#include <algorithm>

static uint32_t ConvertFromVertexDescription(FVertexDescription VtxDesc)
{
    uint32_t Flags = 0;
    if (VtxDesc & EVertexAttribute::Position)    Flags |= 0x00000003;
    if (VtxDesc & EVertexAttribute::Normal)      Flags |= 0x0000000C;
    if (VtxDesc & EVertexAttribute::Color0)      Flags |= 0x00000030;
    if (VtxDesc & EVertexAttribute::Color1)      Flags |= 0x000000C0;
    if (VtxDesc & EVertexAttribute::Tex0)        Flags |= 0x00000300;
    if (VtxDesc & EVertexAttribute::Tex1)        Flags |= 0x00000C00;
    if (VtxDesc & EVertexAttribute::Tex2)        Flags |= 0x00003000;
    if (VtxDesc & EVertexAttribute::Tex3)        Flags |= 0x0000C000;
    if (VtxDesc & EVertexAttribute::Tex4)        Flags |= 0x00030000;
    if (VtxDesc & EVertexAttribute::Tex5)        Flags |= 0x000C0000;
    if (VtxDesc & EVertexAttribute::Tex6)        Flags |= 0x00300000;
    if (VtxDesc & EVertexAttribute::Tex7)        Flags |= 0x00C00000;
    if (VtxDesc & EVertexAttribute::PosMtx)      Flags |= 0x01000000;
    if (VtxDesc & EVertexAttribute::Tex0Mtx)     Flags |= 0x02000000;
    if (VtxDesc & EVertexAttribute::Tex1Mtx)     Flags |= 0x04000000;
    if (VtxDesc & EVertexAttribute::Tex2Mtx)     Flags |= 0x08000000;
    if (VtxDesc & EVertexAttribute::Tex3Mtx)     Flags |= 0x10000000;
    if (VtxDesc & EVertexAttribute::Tex4Mtx)     Flags |= 0x20000000;
    if (VtxDesc & EVertexAttribute::Tex5Mtx)     Flags |= 0x40000000;
    if (VtxDesc & EVertexAttribute::Tex6Mtx)     Flags |= 0x80000000;
    return Flags;
}

CMaterialCooker::CMaterialCooker() = default;

void CMaterialCooker::WriteMatSetPrime(IOutputStream& rOut)
{
    // Gather texture list from the materials before starting
    mTextureIDs.clear();
    const size_t NumMats = mpSet->mMaterials.size();

    for (size_t iMat = 0; iMat < NumMats; iMat++)
    {
        const CMaterial *pMat = mpSet->mMaterials[iMat].get();

        const size_t NumPasses  = pMat->PassCount();
        for (size_t iPass = 0; iPass < NumPasses; iPass++)
        {
            if (const CTexture* pTex = pMat->Pass(iPass)->Texture())
                mTextureIDs.push_back(pTex->ID().ToLong());
        }
    }

    // Sort/remove duplicates
    std::sort(mTextureIDs.begin(), mTextureIDs.end());
    mTextureIDs.erase(std::unique(mTextureIDs.begin(), mTextureIDs.end()), mTextureIDs.end());

    // Write texture IDs
    rOut.WriteULong(static_cast<uint32_t>(mTextureIDs.size()));

    for (const auto id : mTextureIDs)
        rOut.WriteULong(id);

    // Write material offset filler
    rOut.WriteULong(static_cast<uint32_t>(NumMats));
    const uint32_t MatOffsetsStart = rOut.Tell();

    for (size_t iMat = 0; iMat < NumMats; iMat++)
        rOut.WriteULong(0);

    // Write materials
    const uint32_t MatsStart = rOut.Tell();
    std::vector<uint32_t> MatEndOffsets(NumMats);

    for (size_t iMat = 0; iMat < NumMats; iMat++)
    {
        mpMat = mpSet->mMaterials[iMat].get();
        WriteMaterialPrime(rOut);
        MatEndOffsets[iMat] = rOut.Tell() - MatsStart;
    }

    // Write material offsets
    const uint32_t MatsEnd = rOut.Tell();
    rOut.Seek(MatOffsetsStart, SEEK_SET);

    for (size_t iMat = 0; iMat < NumMats; iMat++)
        rOut.WriteULong(MatEndOffsets[iMat]);

    // Done!
    rOut.Seek(MatsEnd, SEEK_SET);
}

void CMaterialCooker::WriteMatSetCorruption(IOutputStream& /*rOut*/)
{
    // todo
}

void CMaterialCooker::WriteMaterialPrime(IOutputStream& rOut)
{
    // Gather data from the passes before we start writing
    uint32_t TexFlags = 0;
    uint32_t NumKonst = 0;
    std::vector<uint32_t> TexIndices;

    for (uint32_t iPass = 0; iPass < mpMat->mPasses.size(); iPass++)
    {
        CMaterialPass *pPass = mpMat->Pass(iPass);

        if ((pPass->KColorSel() >= kKonst0_RGB) ||
            (pPass->KAlphaSel() >= kKonst0_R))
        {
            // Determine the highest Konst index being used
            const auto KColorIndex = static_cast<uint32_t>(pPass->KColorSel()) % 4;
            const auto KAlphaIndex = static_cast<uint32_t>(pPass->KAlphaSel()) % 4;

            if (KColorIndex >= NumKonst)
                NumKonst = KColorIndex + 1;
            if (KAlphaIndex >= NumKonst)
                NumKonst = KAlphaIndex + 1;
        }

        CTexture *pPassTex = pPass->Texture();
        if (pPassTex != nullptr)
        {
            TexFlags |= (1 << iPass);
            uint32_t TexID = pPassTex->ID().ToLong();

            for (uint32_t iTex = 0; iTex < mTextureIDs.size(); iTex++)
            {
                if (mTextureIDs[iTex] == TexID)
                {
                    TexIndices.push_back(iTex);
                    break;
                }
            }
        }
    }

    // Get group index
    size_t GroupIndex = 0;
    const uint64_t MatHash = mpMat->HashParameters();
    bool NewHash = true;

    for (size_t iHash = 0; iHash < mMaterialHashes.size(); iHash++)
    {
        if (mMaterialHashes[iHash] == MatHash)
        {
            GroupIndex = iHash;
            NewHash = false;
            break;
        }
    }

    if (NewHash)
    {
        GroupIndex = mMaterialHashes.size();
        mMaterialHashes.push_back(MatHash);
    }

    // Start writing!
    // Generate flags value
    const bool HasKonst = NumKonst > 0;
    uint32_t Flags;

    if (mVersion <= EGame::Prime)
        Flags = 0x1003;
    else
        Flags = 0x4002;

    Flags |= (HasKonst ? 0x8 : 0x0) | (mpMat->Options() & ~0x8) | (TexFlags << 16);

    rOut.WriteULong(Flags);

    // Texture indices
    rOut.WriteULong(static_cast<uint32_t>(TexIndices.size()));
    for (const auto index : TexIndices)
        rOut.WriteULong(index);

    // Vertex description
    uint32_t VtxFlags = ConvertFromVertexDescription(mpMat->VtxDesc());

    if (mVersion < EGame::Echoes)
        VtxFlags &= 0x00FFFFFF;

    rOut.WriteULong(VtxFlags);

    // Echoes unknowns
    if (mVersion == EGame::Echoes)
    {
        rOut.WriteULong(mpMat->EchoesUnknownA());
        rOut.WriteULong(mpMat->EchoesUnknownB());
    }

    // Group index
    rOut.WriteULong(static_cast<uint32_t>(GroupIndex));

    // Konst
    if (HasKonst)
    {
        rOut.WriteULong(NumKonst);
        for (size_t iKonst = 0; iKonst < NumKonst; iKonst++)
            rOut.WriteLong(mpMat->Konst(iKonst).ToLongRGBA());
    }

    // Blend Mode
    // Some modifications are done to convert the GLenum to the corresponding GX enum
    auto BlendSrcFac = static_cast<uint16>(mpMat->BlendSrcFac());
    auto BlendDstFac = static_cast<uint16>(mpMat->BlendDstFac());
    if (BlendSrcFac >= 0x300) BlendSrcFac -= 0x2FE;
    if (BlendDstFac >= 0x300) BlendDstFac -= 0x2FE;
    rOut.WriteUShort(BlendDstFac);
    rOut.WriteUShort(BlendSrcFac);

    // Color Channels
    rOut.WriteLong(1);
    rOut.WriteLong(0x3000 | (mpMat->IsLightingEnabled() ? 1 : 0));

    // TEV
    const uint32_t NumPasses = mpMat->PassCount();
    rOut.WriteULong(NumPasses);

    for (uint32_t iPass = 0; iPass < NumPasses; iPass++)
    {
        CMaterialPass *pPass = mpMat->Pass(iPass);

        const uint32_t ColorInputFlags = ((pPass->ColorInput(0)) |
                                          (pPass->ColorInput(1) << 5) |
                                          (pPass->ColorInput(2) << 10) |
                                          (pPass->ColorInput(3) << 15));
        const uint32_t AlphaInputFlags = ((pPass->AlphaInput(0)) |
                                          (pPass->AlphaInput(1) << 5) |
                                          (pPass->AlphaInput(2) << 10) |
                                          (pPass->AlphaInput(3) << 15));

        const uint32_t ColorOpFlags = 0x100 | (pPass->ColorOutput() << 9);
        const uint32_t AlphaOpFlags = 0x100 | (pPass->AlphaOutput() << 9);

        rOut.WriteULong(ColorInputFlags);
        rOut.WriteULong(AlphaInputFlags);
        rOut.WriteULong(ColorOpFlags);
        rOut.WriteULong(AlphaOpFlags);
        rOut.WriteUByte(0); // Padding
        rOut.WriteUByte(static_cast<uint8_t>(pPass->KAlphaSel()));
        rOut.WriteUByte(static_cast<uint8_t>(pPass->KColorSel()));
        rOut.WriteUByte(static_cast<uint8_t>(pPass->RasSel()));
    }

    // TEV Tex/UV input selection
    uint32_t CurTexIdx = 0;

    for (size_t iPass = 0; iPass < NumPasses; iPass++)
    {
        rOut.WriteUShort(0); // Padding

        if (mpMat->Pass(iPass)->Texture() != nullptr)
        {
            rOut.WriteUByte(static_cast<uint8_t>(CurTexIdx));
            rOut.WriteUByte(static_cast<uint8_t>(CurTexIdx));
            CurTexIdx++;
        }
        else
        {
            rOut.WriteUShort(static_cast<uint16_t>(0xFFFF));
        }
    }

    // TexGen
    const uint32_t NumTexCoords = CurTexIdx; // TexIdx is currently equal to the tex coord count
    rOut.WriteULong(NumTexCoords);
    uint32_t CurTexMtx = 0;

    for (size_t iPass = 0; iPass < NumPasses; iPass++)
    {
        CMaterialPass *pPass = mpMat->Pass(iPass);
        if (pPass->Texture() == nullptr)
            continue;

        const EUVAnimMode AnimType = pPass->AnimMode();
        const uint32_t CoordSource = pPass->TexCoordSource();

        uint32_t TexMtxIdx, PostMtxIdx;
        bool Normalize;

        // No animation - set TexMtx and PostMtx to identity, disable normalization
        if (AnimType == EUVAnimMode::NoUVAnim)
        {
            TexMtxIdx = 30;
            PostMtxIdx = 61;
            Normalize = false;
        }
        else // Animation - set parameters as the animation mode needs them
        {
            TexMtxIdx = CurTexMtx;

            if (AnimType <= EUVAnimMode::InverseMVTranslated || AnimType >= EUVAnimMode::ModelMatrix)
            {
                PostMtxIdx = CurTexMtx;
                Normalize = true;
            }
            else
            {
                PostMtxIdx = 61;
                Normalize = false;
            }
            CurTexMtx += 3;
        }

        const uint32_t TexGenFlags = (CoordSource << 4) | (TexMtxIdx << 9) | (Normalize << 14) | (PostMtxIdx << 15);
        rOut.WriteULong(TexGenFlags);
    }

    // Animations
    const uint32_t AnimSizeOffset = rOut.Tell();
    const uint32_t NumAnims = CurTexMtx; // CurTexMtx is currently equal to the anim count
    rOut.WriteULong(0);         // Anim size filler
    const uint32_t AnimsStart = rOut.Tell();
    rOut.WriteULong(NumAnims);

    for (uint32_t iPass = 0; iPass < NumPasses; iPass++)
    {
        CMaterialPass *pPass = mpMat->Pass(iPass);
        EUVAnimMode AnimMode = pPass->AnimMode();
        if (AnimMode == EUVAnimMode::NoUVAnim)
            continue;

        rOut.WriteLong(static_cast<int>(AnimMode));

        if ((AnimMode >= EUVAnimMode::UVScroll) && (AnimMode != EUVAnimMode::ModelMatrix))
        {
            rOut.WriteFloat(pPass->AnimParam(0));
            rOut.WriteFloat(pPass->AnimParam(1));

            if ((AnimMode == EUVAnimMode::UVScroll) || (AnimMode == EUVAnimMode::HFilmstrip) || (AnimMode == EUVAnimMode::VFilmstrip))
            {
                rOut.WriteFloat(pPass->AnimParam(2));
                rOut.WriteFloat(pPass->AnimParam(3));
            }
        }
    }

    const uint32_t AnimsEnd = rOut.Tell();
    const uint32_t AnimsSize = AnimsEnd - AnimsStart;
    rOut.Seek(AnimSizeOffset, SEEK_SET);
    rOut.WriteULong(AnimsSize);
    rOut.Seek(AnimsEnd, SEEK_SET);

    // Done!
}

void CMaterialCooker::WriteMaterialCorruption(IOutputStream& /*rOut*/)
{
    // todo
}

// ************ STATIC ************
void CMaterialCooker::WriteCookedMatSet(CMaterialSet* pSet, EGame Version, IOutputStream& rOut)
{
    CMaterialCooker Cooker;
    Cooker.mpSet = pSet;
    Cooker.mVersion = Version;

    switch (Version)
    {
    case EGame::PrimeDemo:
    case EGame::Prime:
    case EGame::EchoesDemo:
    case EGame::Echoes:
        Cooker.WriteMatSetPrime(rOut);
        break;
    default:
        break;
    }
}

void CMaterialCooker::WriteCookedMaterial(CMaterial* pMat, EGame Version, IOutputStream& rOut)
{
    CMaterialCooker Cooker;
    Cooker.mpMat = pMat;
    Cooker.mVersion = Version;

    switch (Version)
    {
    case EGame::PrimeDemo:
    case EGame::Prime:
    case EGame::EchoesDemo:
    case EGame::Echoes:
        Cooker.WriteMaterialPrime(rOut);
        break;
    // TODO: Corruption/Uncooked
    default: break;
    }
}
