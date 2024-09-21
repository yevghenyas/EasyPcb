#include "genpackwizstartpage.h"
#include <QVBoxLayout>
#include <QRadioButton>
#include <QFormLayout>

#include <QLabel>

GenPackWizStartPage::GenPackWizStartPage(PageData *p):pData(p)
{
    QVBoxLayout *pLayout = new QVBoxLayout(this);

    QFormLayout *pFrnLay = new QFormLayout(this);
    QLabel *selLabel = new QLabel(this);
    selLabel->setText("Select the type of the package:");
    pLayout->addWidget(selLabel);
    pLayout->addSpacing(15);
    pButDip = new QRadioButton(this);
    pButDip->setChecked(true);
    pButDip->setText("DIP packege and similar");
    QLabel *pLblDip = new QLabel(this);
    pLblDip->setPixmap(QPixmap(":/images/dippack.png"));
    pFrnLay->addRow(pLblDip,pButDip);

    QLabel *pLblSmd = new QLabel(this);
    pLblSmd->setPixmap(QPixmap(":/images/smdpack.png"));
    pButSmd = new QRadioButton(this);
    pButSmd->setText("SSOP,SOIC,DPAK and similar");
    pFrnLay->addRow(pLblSmd,pButSmd);
    pLayout->addLayout(pFrnLay);
    setLayout(pLayout);
    registerField("dipType",pButDip);
    connect(pButDip,SIGNAL(clicked()),this,SLOT(butClicked()));
    connect(pButSmd,SIGNAL(clicked()),this,SLOT(butClicked()));

    /*
//   QVBoxLayout *pLayout = new QVBoxLayout(this);

   QGridLayout *gridLayout = new QGridLayout(this);

//   QLabel *selLabel = new QLabel(this);
//   selLabel->setText("Select the type of package:");
   //pLayout->addWidget(selLabel);

   QRadioButton *pButDip = new QRadioButton(this);
   pButDip->setText("DIP packege and similar");
//   pButDip->setIcon(QIcon(QPixmap(":/images/dippack.jpeg")));
   QLabel *pLblDip = new QLabel(this);
   pLblDip->setPixmap(QPixmap(":/images/dippack.png"));
//   pLayout->addWidget(pButDip);
   gridLayout->addWidget(pLblDip,0,1);
   gridLayout->addWidget(pButDip,0,0);

   QLabel *pLblSmd = new QLabel(this);
   pLblSmd->setPixmap(QPixmap(":/images/smdpack.png"));
   QRadioButton *pButSmd = new QRadioButton(this);
   pButSmd->setText("SSOP,SOIC,DPAK and similar");
//   pButSmd->setIcon(QIcon(QPixmap(":/images/smdpack.jpeg")));
//   pLayout->addWidget(pButSmd);
   gridLayout->addWidget(pLblSmd,1,1);
   gridLayout->addWidget(pButSmd,1,0);

   setLayout(gridLayout);
   */
}

void GenPackWizStartPage::butClicked()
{
   pData->bValidated = false;
}
