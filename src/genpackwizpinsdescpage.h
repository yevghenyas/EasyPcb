#ifndef GENPACKWIZPINSDESCPAGE_H
#define GENPACKWIZPINSDESCPAGE_H

#include <QWizardPage>
#include <QListWidget>
#include <QPushButton>
#include "common.h"
#include "genericgraphicalitemscontainer.h"
#include "pagedata.h"

class GenPackWizPinsDescPage : public QWizardPage
{
   Q_OBJECT
   QListWidget *lWidget1{nullptr};
   QListWidget *lWidget2{nullptr};
   QPushButton *but1{nullptr};
   QPushButton *but2{nullptr};
   PageData    *pData;
public:
   GenPackWizPinsDescPage(PageData *p);
   void initializePage() override;
   bool validatePage() override;
public slots:
   void but1Pushed();
   void but2Pushed();
};

#endif // GENPACKWIZPINSDESCPAGE_H
