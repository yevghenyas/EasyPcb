#include "componentstreemodel.h"

ComponentsTreeModel::ComponentsTreeModel()
{

}

Qt::ItemFlags ComponentsTreeModel::flags(const QModelIndex &index) const
{
   Qt::ItemFlags defaultFlags = QStandardItemModel::flags(index);

   if (index.isValid())
      return Qt::ItemIsDragEnabled | Qt::ItemIsDropEnabled | defaultFlags;
   else
      return Qt::ItemIsDropEnabled | defaultFlags;
}
Qt::DropActions ComponentsTreeModel::supportedDropActions() const
{
   return Qt::CopyAction | Qt::MoveAction;
}
