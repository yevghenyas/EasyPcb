#include "editmultiplate.h"
#include <QLabel>
#include "roundplategraphicalitem.h"



EditMultiPlate::EditMultiPlate(MultiplateGraphicalItem *p,
                               float mX,float mY,QWidget* parent) :
    QDialog(parent),pMulPlate(p),maxX(mX),maxY(mY)
{

   resize(380, 300);
   createContent();
}

void EditMultiPlate::createContent()
{
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    buttonBox->setGeometry(QRect(20, 252, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);


    auto gridLayoutWidget = new QWidget(this);
    gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
    gridLayoutWidget->setGeometry(QRect(20, 19, 341, 223));

    auto gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);

    auto label = new QLabel(gridLayoutWidget);
    label->setObjectName(QStringLiteral("label"));
    label->setText(X_DEF);
    gridLayout->addWidget(label, 0, 0, 1, 1);

    doubleSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxX->setObjectName(QStringLiteral("doubleSpinBox"));
    doubleSpinBoxX->setMaximum(static_cast<double>(maxX));
    doubleSpinBoxX->setValue(static_cast<double>(pMulPlate->x()));
    gridLayout->addWidget(doubleSpinBoxX, 0, 1, 1, 1);


    auto label_2 = new QLabel(gridLayoutWidget);
    label_2->setObjectName(QStringLiteral("label_2"));
    label_2->setText(Y_DEF);
    gridLayout->addWidget(label_2, 1, 0, 1, 1);

    doubleSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxY->setObjectName(QStringLiteral("doubleSpinBox_2"));
    doubleSpinBoxY->setMaximum(static_cast<double>(maxY));
    doubleSpinBoxY->setValue(static_cast<double>(pMulPlate->y()));
    gridLayout->addWidget(doubleSpinBoxY, 1, 1, 1, 1);

    auto label_3 = new QLabel(gridLayoutWidget);
    label_3->setObjectName(QStringLiteral("label_3"));
    label_3->setText(D_IN_DEF);
    gridLayout->addWidget(label_3, 2, 0, 1, 1);

    auto geom = pMulPlate->getChildren()->at(0)->getGeometry();
    doubleSpinBoxDin = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxDin->setObjectName(QStringLiteral("doubleSpinBox_3"));
    doubleSpinBoxDin->setValue(static_cast<double>(geom.rpl_g.d_in));
    gridLayout->addWidget(doubleSpinBoxDin, 2, 1, 1, 1);

    auto label_4 = new QLabel(gridLayoutWidget);
    label_4->setObjectName(QStringLiteral("label_4"));
    label_4->setText(D_EX_DEF);
    gridLayout->addWidget(label_4, 3, 0, 1, 1);

    doubleSpinBoxDex = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxDex->setObjectName(QStringLiteral("doubleSpinBox_4"));
    doubleSpinBoxDex->setValue(static_cast<double>(geom.rpl_g.d_ex));
    gridLayout->addWidget(doubleSpinBoxDex, 3, 1, 1, 1);


    auto label_5 = new QLabel(gridLayoutWidget);
    label_5->setObjectName(QStringLiteral("label_5"));
    label_5->setText(tr("First layer"));
    gridLayout->addWidget(label_5, 4, 0, 1, 1);

    comboBoxL1 = new LayersCombo(LayersCombo::ComboType::Active,
                               pMulPlate->getFirstPlate()->getLevel(),
                               gridLayoutWidget);
    comboBoxL1->setObjectName(QStringLiteral("comboBox"));
    gridLayout->addWidget(comboBoxL1, 4, 1, 1, 1);


    auto label_6 = new QLabel(gridLayoutWidget);
    label_6->setObjectName(QStringLiteral("label_6"));
    label_6->setText(tr("Second layer"));
    gridLayout->addWidget(label_6, 5, 0, 1, 1);

    comboBoxL2 = new LayersCombo(LayersCombo::ComboType::Active,
                                 pMulPlate->getSecondPlate()->getLevel(),
                                 gridLayoutWidget);
    comboBoxL2->setObjectName(QStringLiteral("comboBox_2"));
    gridLayout->addWidget(comboBoxL2, 5, 1, 1, 1);


    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void EditMultiPlate::accept()
{
   RoundPlateGraphicalItem *pRp1 = static_cast<RoundPlateGraphicalItem*>(pMulPlate->getFirstPlate().get());
   RoundPlateGraphicalItem *pRp2 = static_cast<RoundPlateGraphicalItem*>(pMulPlate->getSecondPlate().get());
   pRp1->setX(static_cast<float>(doubleSpinBoxX->value()));
   pRp1->setY(static_cast<float>(doubleSpinBoxY->value()));
   pRp2->setX(static_cast<float>(doubleSpinBoxX->value()));
   pRp2->setY(static_cast<float>(doubleSpinBoxY->value()));
   pRp1->setd(static_cast<float>(doubleSpinBoxDex->value()));
   pRp1->setd1(static_cast<float>(doubleSpinBoxDin->value()));
   pRp2->setd(static_cast<float>(doubleSpinBoxDex->value()));
   pRp2->setd1(static_cast<float>(doubleSpinBoxDin->value()));
   pRp1->setLevel(comboBoxL1->getSelectedLayerId());
   pRp2->setLevel(comboBoxL2->getSelectedLayerId());
   QDialog::accept();
}

EditMultiPlate::~EditMultiPlate()
{

}
