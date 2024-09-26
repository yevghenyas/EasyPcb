#include "genpackwizstartpage.h"
#include <QVBoxLayout>
#include <QRadioButton>
#include <QFormLayout>

#include <QLabel>

GenPackWizStartPage::GenPackWizStartPage(PageData *p):pData(p)
{
    QVBoxLayout *pLayout = new QVBoxLayout;

    QFormLayout *pFrnLay = new QFormLayout;
    QLabel *selLabel = new QLabel;
    selLabel->setText("Select the type of the package:");
    pLayout->addWidget(selLabel);
    pLayout->addSpacing(15);
    pButDip = new QRadioButton;
    pButDip->setChecked(true);
    pButDip->setText("DIP packege and similar");
    QLabel *pLblDip = new QLabel;
    pLblDip->setPixmap(QPixmap(":/images/dippack.png"));
    pFrnLay->addRow(pLblDip,pButDip);

    QLabel *pLblSmd = new QLabel;
    pLblSmd->setPixmap(QPixmap(":/images/smdpack.png"));
    pButSmd = new QRadioButton;
    pButSmd->setText("SSOP,SOIC,DPAK and similar");
    pFrnLay->addRow(pLblSmd,pButSmd);
    pLayout->addLayout(pFrnLay);
    setLayout(pLayout);
    registerField("dipType",pButDip);
    connect(pButDip,SIGNAL(clicked()),this,SLOT(butClicked()));
    connect(pButSmd,SIGNAL(clicked()),this,SLOT(butClicked()));
}

void GenPackWizStartPage::butClicked()
{
   pData->bValidated = false;
}
