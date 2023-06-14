#include "componentstreeview.h"
#include "common.h"

ComponentsTreeView::ComponentsTreeView(QWidget *parent) : QTreeView(parent)
{

}


void ComponentsTreeView::dragMoveEvent(QDragMoveEvent*)
{
   cout<<"ComponentsTreeView::dragMoveEvent"<<endl;
}
void ComponentsTreeView::dropEvent(QDropEvent*)
{
   cout<<"ComponentsTreeView::dropEvent"<<endl;
}
