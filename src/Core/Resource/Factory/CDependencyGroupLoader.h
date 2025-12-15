#ifndef CDEPENDENCYGROUPLOADER_H
#define CDEPENDENCYGROUPLOADER_H

#include <Common/EGame.h>
#include <memory>

class CDependencyGroup;
class CResourceEntry;
class IInputStream;

class CDependencyGroupLoader
{
    CDependencyGroupLoader() = delete;

public:
    static std::unique_ptr<CDependencyGroup> LoadDGRP(IInputStream& rDGRP, CResourceEntry *pEntry);
};

#endif // CDEPENDENCYGROUPLOADER_H
