#ifndef GENPACKWIZARD_H
#define GENPACKWIZARD_H

#include <QWizard>
#include <graphicalitem.h>
#include "pagedata.h"

class GenPackWizard : public QWizard
{
   PageData data;
public:
   GenPackWizard();
   PageData* getPageData(){return &data;}
};

#endif // GENPACKWIZARD_H
