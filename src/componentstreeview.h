#ifndef COMPONENTSTREEVIEW_H
#define COMPONENTSTREEVIEW_H

#include <QTreeView>


class ComponentsTreeView : public QTreeView
{
public:
   ComponentsTreeView(QWidget *parent);
   virtual void dragMoveEvent(QDragMoveEvent*) override;
   virtual void dropEvent(QDropEvent*) override;
};

#endif // COMPONENTSTREEVIEW_H
