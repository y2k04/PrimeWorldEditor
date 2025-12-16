#ifndef SCOLLISIONINDEXDATA_H
#define SCOLLISIONINDEXDATA_H

#include "Core/Resource/Collision/CCollisionMaterial.h"
#include <Common/Math/CVector3f.h>
#include <cstdint>

/** Common index data found in all collision file formats */
struct SCollisionIndexData
{
    std::vector<CCollisionMaterial> Materials;
    std::vector<uint8_t>            VertexMaterialIndices;
    std::vector<uint8_t>            EdgeMaterialIndices;
    std::vector<uint8_t>            TriangleMaterialIndices;
    std::vector<uint16_t>           EdgeIndices;
    std::vector<uint16_t>           TriangleIndices;
    std::vector<uint16_t>           UnknownData;
    std::vector<CVector3f>          Vertices;
};

#endif // SCOLLISIONINDEXDATA_H
