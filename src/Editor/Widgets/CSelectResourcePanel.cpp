#include "Editor/Widgets/CSelectResourcePanel.h"
#include "ui_CSelectResourcePanel.h"

#include "Editor/CEditorApplication.h"
#include "Editor/Widgets/CResourceSelector.h"

#include <algorithm>
#include <QScreen>

CSelectResourcePanel::CSelectResourcePanel(CResourceSelector *pSelector)
    : QFrame(pSelector)
    , mpUI(std::make_unique<Ui::CSelectResourcePanel>())
    , mpSelector(pSelector)
    , mModel(pSelector)
{
    setWindowFlags(windowFlags() | Qt::FramelessWindowHint | Qt::Window);

    mpUI->setupUi(this);
    mProxyModel.setSourceModel(&mModel);
    mpUI->ResourceTableView->setModel(&mProxyModel);

    // Signals/slots
    connect(gpEdApp, &QApplication::focusChanged, this, &CSelectResourcePanel::FocusChanged);
    connect(mpUI->SearchBar, &CTimedLineEdit::StoppedTyping, this, &CSelectResourcePanel::SearchStringChanged);
    connect(mpUI->ResourceTableView, &QTableView::clicked, this, &CSelectResourcePanel::ResourceClicked);

    // Determine size
    const QPoint SelectorPos = pSelector->parentWidget()->mapToGlobal(pSelector->pos());
    const QRect ScreenRect = gpEdApp->primaryScreen()->availableGeometry();

    const int MaxWidthLeft = SelectorPos.x();
    const int MaxWidthRight = ScreenRect.width() - SelectorPos.x() - pSelector->width();
    const int MaxWidth = std::max(MaxWidthLeft, MaxWidthRight);

    const int MaxHeightTop = SelectorPos.y();
    const int MaxHeightBottom = ScreenRect.height() - SelectorPos.y() - pSelector->height();
    const int MaxHeight = std::max(MaxHeightTop, MaxHeightBottom);

    const QSize PanelSize(std::min(width(), MaxWidth), std::min(height(), MaxHeight));

    // Determine position; place wherever we have the most amount of space
    QPoint PanelPos;

    if (MaxHeightTop > MaxHeightBottom)
        PanelPos.ry() = SelectorPos.y() - PanelSize.height();
    else
        PanelPos.ry() = SelectorPos.y() + pSelector->height();

    if (MaxWidthLeft > MaxWidthRight)
        PanelPos.rx() = SelectorPos.x() + (pSelector->width() - PanelSize.width());
    else
        PanelPos.rx() = SelectorPos.x();

    // Clamp position to screen boundaries
    PanelPos.rx() = std::clamp(PanelPos.x(), 0, ScreenRect.width() - PanelSize.width());
    PanelPos.ry() = std::clamp(PanelPos.y(), 0, ScreenRect.height() - PanelSize.height());

    // Create widget geometry
    const QRect PanelRect(PanelPos, PanelSize);
    setGeometry(PanelRect);

    // Jump to the currently-selected resource
    const QModelIndex Index = mModel.InitialIndex();
    const QModelIndex ProxyIndex = mProxyModel.mapFromSource(Index);

    mpUI->ResourceTableView->scrollTo(ProxyIndex, QAbstractItemView::PositionAtCenter);
    mpUI->ResourceTableView->selectionModel()->setCurrentIndex(ProxyIndex, QItemSelectionModel::ClearAndSelect);

    // Show
    show();
    mpUI->SearchBar->setFocus();
}

CSelectResourcePanel::~CSelectResourcePanel() = default;

// Slots
void CSelectResourcePanel::FocusChanged(QWidget*, QWidget *pNew)
{
    // Destroy when the panel loses focus
    if (pNew != this && !isAncestorOf(pNew))
        deleteLater();
}

void CSelectResourcePanel::SearchStringChanged(const QString& SearchString)
{
    mProxyModel.SetSearchString(SearchString);
}

void CSelectResourcePanel::ResourceClicked(const QModelIndex& Index)
{
    const QModelIndex SourceIndex = mProxyModel.mapToSource(Index);
    CResourceEntry *pEntry = mModel.EntryForIndex(SourceIndex);
    mpSelector->SetResource(pEntry);
    close();
}
