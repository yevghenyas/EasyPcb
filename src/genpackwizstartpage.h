#ifndef GENPACKWIZSTARTPAGE_H
#define GENPACKWIZSTARTPAGE_H

#include <QWizardPage>
#include <QRadioButton>
#include "pagedata.h"

class GenPackWizStartPage : public QWizardPage
{
    Q_OBJECT
    QRadioButton *pButDip{nullptr};
    QRadioButton *pButSmd{nullptr};
    PageData *pData{nullptr};
public:
    GenPackWizStartPage(PageData *p);
public slots:
    void butClicked();
};

#endif // GENPACKWIZSTARTPAGE_H
