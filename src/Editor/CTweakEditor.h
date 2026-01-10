#ifndef CTWEAKEDITOR_H
#define CTWEAKEDITOR_H

#include "Editor/IEditor.h"

#include <memory>
#include <vector>

namespace Ui {
class CTweakEditor;
}

class CGameProject;
class CTweakData;

class CTweakEditor : public IEditor
{
    Q_OBJECT

    /** Qt UI */
    std::unique_ptr<Ui::CTweakEditor> mpUI;

    /** Editable tweak assets */
    std::vector<CTweakData*> mTweakAssets;

    /** Whether the editor window has been shown before */
    bool mHasBeenShown = false;

    /** Index of tweak data currently being edited */
    int mCurrentTweakIndex = -1;

public:
    explicit CTweakEditor(QWidget* pParent = nullptr);
    ~CTweakEditor() override;

    bool HasTweaks() const;
    bool Save() override;

public slots:
    void SetActiveTweakData(const CTweakData* pTweakData);
    void SetActiveTweakIndex(int Index);
    void OnTweakTabClicked(int Index);
    void OnProjectChanged(CGameProject* pNewProject);
};

#endif // CTWEAKEDITOR_H
