#ifndef CRESOURCETABLEVIEW_H
#define CRESOURCETABLEVIEW_H

#include <QTableView>

class CResourceProxyModel;
class CResourceTableModel;

class CResourceTableView : public QTableView
{
    Q_OBJECT

    CResourceTableModel *mpModel = nullptr;
    CResourceProxyModel *mpProxy = nullptr;
    QAction *mpDeleteAction = nullptr;

public:
    explicit CResourceTableView(QWidget *pParent = nullptr);
    ~CResourceTableView() override;

    void setModel(QAbstractItemModel *pModel) override;
    void dragEnterEvent(QDragEnterEvent *pEvent) override;

public slots:
    void DeleteSelected();
};

#endif // CRESOURCETABLEVIEW_H
