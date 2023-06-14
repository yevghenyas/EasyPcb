#ifndef COMPONENTSTREEMODEL_H
#define COMPONENTSTREEMODEL_H

#include <QStandardItemModel>


class ComponentsTreeModel : public QStandardItemModel
{
public:
    ComponentsTreeModel();
    virtual Qt::ItemFlags flags(const QModelIndex &index) const override;
    virtual Qt::DropActions supportedDropActions() const override;
};

#endif // COMPONENTSTREEMODEL_H
