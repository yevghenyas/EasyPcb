#include "genwizdipgeompage.h"
#include <QFormLayout>
#include <QLabel>
#include <QSpinBox>
#include "common.h"
#include <QGridLayout>
#include <QPainter>
#include "itemsfactory.h"
#include "packagegraphicalitem.h"
#include "roundplategraphicalitem.h"
#include "rectgraphicalitem.h"
#include <vector>

GenWizDipGeomPage::GenWizDipGeomPage(PageData *p) : pData(p)
{
   QGridLayout *pLay = new QGridLayout(this);
   QFormLayout *pFrnLay = new QFormLayout(this);

   QLabel *pLblNum1 = new QLabel(this);
   pLblNum1->setText("The number of pins for Side 1:");
   pNumSpin1 = new QSpinBox(this);
   pNumSpin1->setRange(2,120);
   pNumSpin1->setValue(4);
   pFrnLay->addRow(pLblNum1,pNumSpin1);

   QLabel *pLblNum2 = new QLabel(this);
   pLblNum2->setText("The number of pins for Side 2:");
   pNumSpin2 = new QSpinBox(this);
   pNumSpin2->setRange(0,120);
   pNumSpin2->setValue(0);
   pFrnLay->addRow(pLblNum2,pNumSpin2);

   QLabel *pLblNum3 = new QLabel(this);
   pLblNum3->setText("The number of pins for Side 3:");
   pNumSpin3 = new QSpinBox(this);
   pNumSpin3->setRange(2,120);
   pNumSpin3->setValue(4);
   pFrnLay->addRow(pLblNum3,pNumSpin3);

   QLabel *pLblNum4 = new QLabel(this);
   pLblNum4->setText("The number of pins for Side 4:");
   pNumSpin4 = new QSpinBox(this);
   pNumSpin4->setRange(0,120);
   pNumSpin4->setValue(0);
   pFrnLay->addRow(pLblNum4,pNumSpin4);


   QLabel *pLblDist = new QLabel(this);
   pLblDist->setText("Distance between pins:");
   pDistSpin = new QDoubleSpinBox(this);
   pDistSpin->setRange(0.3,10.0);
   pFrnLay->addRow(pLblDist,pDistSpin);

   QLabel *pLblHeight = new QLabel(this);
   pLblHeight->setText(HEIGHT_DEF);
   pHeightSpin = new QDoubleSpinBox(this);
   pHeightSpin->setRange(1.0,100.0);
   pFrnLay->addRow(pLblHeight,pHeightSpin);
   registerField("pacHeight",pHeightSpin);


   QLabel *pLblWidth = new QLabel(this);
   pLblWidth->setText(WIDTH_DEF);
   pWidthSpin = new QDoubleSpinBox(this);
   pWidthSpin->setRange(1.0,100.0);
   pFrnLay->addRow(pLblWidth,pWidthSpin);

   QLabel *pLblPackPinDist = new QLabel(this);
   pLblPackPinDist->setText("Pin-package) distance");
   pPinPackDistSpin = new QDoubleSpinBox(this);
   pPinPackDistSpin->setRange(-100.0,100.0);
   pFrnLay->addRow(pLblPackPinDist,pPinPackDistSpin);


   pPreviewLbl = new PreviewLabel();
   pPreviewLbl->setSizeHintX(250);
   pPreviewLbl->setSizeHintY(250);
   //set new pixmap or clear existing
   pPreviewLbl->setDefaultPixmap();

   pLay->addLayout(pFrnLay,0,0);
   pLay->addWidget(pPreviewLbl,0,1);
   setLayout(pLay);
}

void GenWizDipGeomPage::initializePage()
{
    if(!pData->bValidated)
    {
       bDipTypeChip = field("dipType").toBool();
       array<int,4> n = {pNumSpin1->value(),pNumSpin2->value(),pNumSpin3->value(),pNumSpin4->value()};

       auto dist = bDipTypeChip ? 2.5 : 1.27;

       pDistSpin->setValue(dist);
       pData->pinDist = dist;

       if(bDipTypeChip)
          pWidthSpin->setValue(getFullWidthForDipPack(8));
       else
          pWidthSpin->setValue(getWidthForSOPack(8));

       pPinPackDistSpin->setValue(bDipTypeChip ? 1.27 : 0.9);
       pData->pinPackDist = pPinPackDistSpin->value();

       pHeightSpin->setValue(pNumSpin1->value() * dist);

       item = ItemsFactory::createGenericChip(bDipTypeChip,0,0,pDistSpin->value(),pPinPackDistSpin->value(),
                                              pWidthSpin->value(),pHeightSpin->value(),
                                              bDipTypeChip ? BOARD_LEVEL_ID::LEVEL_F : BOARD_LEVEL_ID::LEVEL_A,
                                              ID_NONE,
                                              ITEMS_ORIENTATION::O_VERTICAL_TOP,
                                              n);


       pPreviewLbl->previewItem(item,"previewItem");
       pData->item = item;

       connect(pNumSpin1,SIGNAL(valueChanged(int)),this,SLOT(pinsNumberChanged(int)));
       connect(pNumSpin2,SIGNAL(valueChanged(int)),this,SLOT(pinsNumberChanged(int)));
       connect(pNumSpin3,SIGNAL(valueChanged(int)),this,SLOT(pinsNumberChanged(int)));
       connect(pNumSpin4,SIGNAL(valueChanged(int)),this,SLOT(pinsNumberChanged(int)));
       connect(pDistSpin,SIGNAL(valueChanged(double)),this,SLOT(pinsDistChanged(double)));
       connect(pHeightSpin,SIGNAL(valueChanged(double)),this,SLOT(heightChanged(double)));
       connect(pWidthSpin,SIGNAL(valueChanged(double)),this,SLOT(widthChanged(double)));
       connect(pPinPackDistSpin,SIGNAL(valueChanged(double)),this,SLOT(pinPackDistanseChanged(double)));
    }
}

void GenWizDipGeomPage::widthChanged(double)
{
   updatePackageGeometry();
   updatePinsGeometry(false);
   pPreviewLbl->refresh();
}

void GenWizDipGeomPage::heightChanged(double)
{
   updatePackageGeometry();
   updatePinsGeometry(false);
   pPreviewLbl->refresh();
}

void GenWizDipGeomPage::updatePinsGeometry(bool updatePinSize)
{
   const int l = 4;
   vector<PointF> pts;
   ITEMS_ORIENTATION po[l] = {O_HORIZONTAL_LEFT,O_VERTICAL_BOTTOM,O_HORIZONTAL_RIGHT,O_VERTICAL_TOP};
   int pinsForSides[l] = {pNumSpin1->value(),pNumSpin2->value(),pNumSpin3->value(),pNumSpin4->value()};
   for(auto i = 0; i < l; ++i)
   {
      list<PointF> points;
      calculatePointsForPins(points,po[i],pinsForSides[i],pDistSpin->value(),pPinPackDistSpin->value(),
                             pWidthSpin->value(),pHeightSpin->value());
      for(auto& coord:points)
          pts.push_back(coord);

   }
   GenericGraphicalItemsContainer *p = static_cast<GenericGraphicalItemsContainer*>(pPreviewLbl->getPreviewItem());
   auto children = p->getChildren();
   auto i = 0;
   // item[0] PackageGraphicalItem,we do not need it. Begin with i = 1
   for(size_t i = 1; i < children->size(); ++i)
   {
      PackageGraphicalItem *p = nullptr;
      if((p = dynamic_cast<PackageGraphicalItem*>(children->at(i).get())) == nullptr)
      {
         children->at(i)->setX(pts.at(i - 1).x());
         children->at(i)->setY(pts.at(i - 1).y());
         if(updatePinSize)
         {
            auto pinHeight = abs(pPinPackDistSpin->value()) * 2;
            auto pinWidth = pDistSpin->value() * 0.7;
            RoundPlateGraphicalItem *pRp = nullptr;
            if((pRp = dynamic_cast<RoundPlateGraphicalItem*>(children->at(i).get())) != nullptr)
            {
               pRp->setd(pinWidth);
               pRp->setd1(pinWidth/3);
            }
            else
            {
               (static_cast<RectGraphicalItem*>(children->at(i).get()))->setWidth(pinWidth);
               (static_cast<RectGraphicalItem*>(children->at(i).get()))->setHeight(pinHeight);
            }
         }
      }
   }
}

void GenWizDipGeomPage::pinPackDistanseChanged(double dValue)
{
   updatePinsGeometry(true);
   pData->pinPackDist = dValue;
   pPreviewLbl->refresh();
}
void GenWizDipGeomPage::pinsNumberChanged(int )
{
   item.reset(nullptr);
   array<int,4> n = {pNumSpin1->value(),pNumSpin2->value(),pNumSpin3->value(),pNumSpin4->value()};
   item = ItemsFactory::createGenericChip(bDipTypeChip,0,0,pDistSpin->value(),pPinPackDistSpin->value(),
                                          pWidthSpin->value(),pHeightSpin->value(),
                                          BOARD_LEVEL_ID::LEVEL_A,
                                          ID_NONE,
                                          ITEMS_ORIENTATION::O_VERTICAL_TOP,
                                          n);
   pPreviewLbl->previewItem(item,"previewItem");
   pData->item = item;
   pData->bValidated = false;
}
void GenWizDipGeomPage::pinsDistChanged(double value)
{
   updatePinsGeometry(true);
   pData->pinDist = value;
   pPreviewLbl->refresh();
}


void GenWizDipGeomPage::updatePackageGeometry()
{
   GenericGraphicalItemsContainer *pContaner = static_cast<GenericGraphicalItemsContainer*>(item.get());
   auto pChildren = pContaner->getChildren();
   for(auto& contItem:*pChildren)
   {
      PackageGraphicalItem *pckg = nullptr;
      if((pckg  = dynamic_cast<PackageGraphicalItem*>(contItem.get())) != nullptr)
      {
         pckg->setWidth(pWidthSpin->value());
         pckg->setHeight(pHeightSpin->value());
      }
   }
}
