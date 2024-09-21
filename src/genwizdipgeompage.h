#ifndef GENWIZDIPGEOMPAGE_H
#define GENWIZDIPGEOMPAGE_H

#include <QWizardPage>
#include <QPixmap>
#include "previewlabel.h"
#include "genericgraphicalitemscontainer.h"
#include <QDoubleSpinBox>
#include "pagedata.h"


class GenWizDipGeomPage : public QWizardPage
{
   Q_OBJECT
   QPixmap pix;
   PreviewLabel *pPreviewLbl{nullptr};
   SmartPtr<GraphicalItem> item;
   bool bDipTypeChip{true};
   QDoubleSpinBox *pHeightSpin{nullptr};
   QDoubleSpinBox *pWidthSpin{nullptr};
   QSpinBox *pNumSpin1{nullptr};
   QSpinBox *pNumSpin2{nullptr};
   QSpinBox *pNumSpin3{nullptr};
   QSpinBox *pNumSpin4{nullptr};
   QDoubleSpinBox *pPinPackDistSpin{nullptr};
   QDoubleSpinBox *pDistSpin{nullptr};
   void updatePinsGeometry(bool updatePinSize);
   PageData *pData;
public slots:
   void pinPackDistanseChanged(double dValue);
   void widthChanged(double dValue);
   void heightChanged(double dValue);
   void pinsNumberChanged(int value);
   void pinsDistChanged(double dValue);
public:
   GenWizDipGeomPage(PageData *p);
   void updatePackageGeometry();
   void initializePage() override;
};

#endif // GENWIZDIPGEOMPAGE_H
