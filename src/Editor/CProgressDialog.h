#ifndef CPROGRESSDIALOG_H
#define CPROGRESSDIALOG_H

#include "IProgressNotifierUI.h"
#include "UICommon.h"
#include <Core/GameProject/CGameProject.h>
#include <Core/IProgressNotifier.h>

#include <QDialog>
#include <QFuture>
#include <QFutureWatcher>

#include <memory>

namespace Ui {
class CProgressDialog;
}

class CProgressDialog : public IProgressNotifierUI
{
    Q_OBJECT
    std::unique_ptr<Ui::CProgressDialog> mpUI;
    bool mUseBusyIndicator;
    bool mAlertOnFinish;
    bool mFinished = false;
    bool mCanceled = false;

public:
    explicit CProgressDialog(const QString& OperationName, bool UseBusyIndicator, bool AlertOnFinish, QWidget *pParent = nullptr);
    ~CProgressDialog() override;

    void DisallowCanceling();

    // IProgressNotifier interface
    bool ShouldCancel() const override;

    // Slots
public slots:
    void closeEvent(QCloseEvent *pEvent) override;
    void FinishAndClose();
    void CancelButtonClicked();
    void UpdateUI(const QString& rkTaskDesc, const QString& rkStepDesc, float ProgressPercent) override;

    // Results
protected:
    template<typename RetType>
    void InternalWaitForResults(const QFuture<RetType>& Future)
    {
        gpEdApp->SetEditorTicksEnabled(false);

        QFutureWatcher<RetType> Watcher;
        connect(&Watcher, &QFutureWatcher<RetType>::finished, this, &CProgressDialog::FinishAndClose);
        Watcher.setFuture(Future);
        exec();

        gpEdApp->SetEditorTicksEnabled(true);

        if (mAlertOnFinish)
            gpEdApp->alert(parentWidget());
    }

public:
    template<typename RetType>
    RetType WaitForResults(const QFuture<RetType>& Future)
    {
        InternalWaitForResults(Future);
        return Future.result();
    }
};

template<>
inline void CProgressDialog::WaitForResults(const QFuture<void>& Future)
{
    InternalWaitForResults(Future);
}

#endif // CPROGRESSDIALOG_H
