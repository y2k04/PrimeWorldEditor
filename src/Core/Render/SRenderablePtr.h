#ifndef SRENDERABLEPTR_H
#define SRENDERABLEPTR_H

#include "Core/Render/ERenderCommand.h"
#include "Core/Render/IRenderable.h"
#include <Common/Math/CAABox.h>
#include <cstdint>

struct SRenderablePtr
{
    IRenderable *pRenderable;
    int32_t ComponentIndex;
    CAABox AABox;
    ERenderCommand Command;
};

#endif // SRENDERABLEPTR_H
