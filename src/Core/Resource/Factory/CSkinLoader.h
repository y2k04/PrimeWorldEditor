#ifndef CSKINLOADER_H
#define CSKINLOADER_H

#include "Core/Resource/TResPtr.h"
#include <memory>

class CSkin;

class CSkinLoader
{
    CSkinLoader() = delete;

public:
    static std::unique_ptr<CSkin> LoadCSKR(IInputStream& rCSKR, CResourceEntry *pEntry);
};

#endif // CSKINLOADER_H
