#ifndef CAREAATTRIBUTES_H
#define CAREAATTRIBUTES_H

#include <Common/EGame.h>
#include "Core/Resource/Script/Property/TPropertyRef.h"

class CScriptObject;

class CAreaAttributes
{
    CScriptObject* mpObject;
    EGame mGame;

    CBoolRef mNeedSky;
    CAssetRef mOverrideSky;

public:
    explicit CAreaAttributes(CScriptObject *pObj);
    ~CAreaAttributes();

    void SetObject(CScriptObject *pObj);
    bool IsLayerEnabled() const;
    bool IsSkyEnabled() const;
    CModel* SkyModel() const;

    CScriptObject* Instance() const { return mpObject; }
};

#endif // CAREAATTRIBUTES_H
