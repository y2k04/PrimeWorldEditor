#include "Editor/ResourceBrowser/CResourceTableView.h"

#include "Editor/CEditorApplication.h"
#include "Editor/ResourceBrowser/CResourceBrowser.h"
#include "Editor/ResourceBrowser/CResourceProxyModel.h"
#include "Editor/ResourceBrowser/CResourceTableModel.h"

#include <QAction>
#include <QDragEnterEvent>

CResourceTableView::CResourceTableView(QWidget *pParent)
    : QTableView(pParent)
{
    // todo: removed delete shortcut because it conflicts with the World Editor delete shortcut
#if 0
    mpDeleteAction = new QAction(this);
    mpDeleteAction->setShortcut(QKeySequence::Delete);
    connect(mpDeleteAction, &QAction::triggered, this, &CResourceTableView::DeleteSelected);
    addAction(mpDeleteAction);
#endif
}

CResourceTableView::~CResourceTableView() = default;

void CResourceTableView::setModel(QAbstractItemModel *pModel)
{
    if (qobject_cast<CResourceProxyModel*>(pModel) != nullptr)
        QTableView::setModel(pModel);
}

void CResourceTableView::dragEnterEvent(QDragEnterEvent *pEvent)
{
    // need to reimplement this to fix a bug in QAbstractItemView
    if (dragDropMode() == QAbstractItemView::InternalMove &&
        (pEvent->source() != this || ((pEvent->possibleActions() & Qt::MoveAction) == 0)))
    {
        return;
    }

    if (pEvent->possibleActions() & model()->supportedDropActions())
    {
        pEvent->accept();
        setState(QAbstractItemView::DraggingState);
    }
}

// ************ SLOTS ************
void CResourceTableView::DeleteSelected()
{
    QModelIndexList List = selectionModel()->selectedIndexes();

    // Figure out which indices can actually be deleted
    CResourceProxyModel *pProxy = static_cast<CResourceProxyModel*>(model());
    CResourceTableModel *pModel = static_cast<CResourceTableModel*>(pProxy->sourceModel());
    QList<CResourceEntry*> ResourcesToDelete;
    QList<CVirtualDirectory*> DirsToDelete;

    for (const QModelIndex& Index : List)
    {
        const QModelIndex SourceIndex = pProxy->mapToSource(Index);

        if (pModel->IsIndexDirectory(SourceIndex))
            DirsToDelete.push_back(pModel->IndexDirectory(SourceIndex));
        else
            ResourcesToDelete.push_back(pModel->IndexEntry(SourceIndex));

    }

    // Delete
    gpEdApp->ResourceBrowser()->Delete(std::move(ResourcesToDelete), std::move(DirsToDelete));
}
