#ifndef GENPACKWIZSTARTPAGE_H
#define GENPACKWIZSTARTPAGE_H

#include <QWizardPage>
#include "pagedata.h"
#include <QRadioButton>

class GenPackWizStartPage : public QWizardPage
{
    QRadioButton *pButDip{nullptr};
    QRadioButton *pButSmd{nullptr};
    PageData *pData{nullptr};
public:
    GenPackWizStartPage(PageData *p);
public slots:
    void butClicked();
};

#endif // GENPACKWIZSTARTPAGE_H
