#ifndef CSCANCOOKER_H
#define CSCANCOOKER_H

class CScan;
class IOutputStream;

/** Cooker class for writing game-compatible SCAN assets */
class CScanCooker
{
public:
    CScanCooker() = delete;
    static bool CookSCAN(const CScan* pScan, IOutputStream& SCAN);
};

#endif // CSCANCOOKER_H
