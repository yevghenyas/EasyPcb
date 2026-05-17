#ifndef RESULTSAUTODLG_H
#define RESULTSAUTODLG_H

#include <QDialog>
#include <QDialogButtonBox>
#include "graphicalitem.h"
#include "smartptr.h"
#include <map>
#include <QVBoxLayout>
#include <QListWidget>
#include "boardlayerswrapper.h"


class ResultsAutoDlg : public QDialog
{
   QDialogButtonBox *buttonBox;
   QListWidget *m_pListWidget;
   const multimap<QString,SmartPtr<GraphicalItem>> m_BestProc;
   QString selConnectorName;
   map<QString,QString> m_NameToId;
public:
   ResultsAutoDlg(QWidget *parent,const multimap<QString,SmartPtr<GraphicalItem>>& bestProc,
                  map<QString,SmartPtr<GraphicalItem>> *vcCons);
   virtual void accept();
   void selectConstrCons(const QString& key, bool bSelect);
public slots:
   void showConstructedConnectors();
};

#endif // RESULTSAUTODLG_H
