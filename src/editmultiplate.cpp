#include "editmultiplate.h"
#include <QLabel>
#include <QLineEdit>
#include "roundplategraphicalitem.h"



EditMultiPlate::EditMultiPlate(MultiplateGraphicalItem *p,
                               float mX,float mY,char *pN,
                               QWidget* parent) :
    QDialog(parent),pMulPlate(p),maxX(mX),maxY(mY),pName(pN)
{

   resize(340, 340);
   createContent();
}

EditMultiPlate::EditMultiPlate(MultiplateGraphicalItem *p,
                        shared_ptr<PointF>& pos,
                        shared_ptr<GeomCommonProps>& props,char *pN,
                        QWidget *parent):
                              QDialog(parent),pMulPlate(p),maxX(fMaxBoardWidth),
                              maxY(fMaxBoardHeight),
                              bLayersEditable(false),
                              pPos(pos),pProps(props),pName(pN)
{
   resize(340, 340);
   createContent();
}


void EditMultiPlate::createContent()
{
    buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
    buttonBox->setGeometry(20, 300, 260, 32);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);


    auto gridLayoutWidget = new QWidget(this);
    gridLayoutWidget->setObjectName(QStringLiteral("gridLayoutWidget"));
    gridLayoutWidget->setGeometry(QRect(20, 19, 260, 270));

    auto gridLayout = new QGridLayout(gridLayoutWidget);
    gridLayout->setObjectName(QStringLiteral("gridLayout"));
    gridLayout->setContentsMargins(0, 0, 0, 0);

    auto rowNum = 0;
    auto labelName = new QLabel(gridLayoutWidget);
    labelName->setObjectName(QStringLiteral("label1_0"));
    labelName->setText(NAME_DEF);
    gridLayout->addWidget(labelName, rowNum, 0, 1, 1);

    m_pNameEdit = new QLineEdit(gridLayoutWidget);
    m_pNameEdit->setObjectName(QStringLiteral("name"));
    m_pNameEdit->setMaxLength(itemNameSize - 1);
    m_pNameEdit->setMaxLength(sizeof(pMulPlate->getName()));
    m_pNameEdit->setText(pMulPlate->getName());
    gridLayout->addWidget(m_pNameEdit, rowNum, 1, 1, 1);

    ++rowNum;

    auto label = new QLabel(gridLayoutWidget);
    label->setObjectName(QStringLiteral("label1_1"));
    label->setText(X_DEF);
    gridLayout->addWidget(label, rowNum, 0, 1, 1);

    doubleSpinBoxX = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxX->setObjectName(QStringLiteral("doubleSpinBox"));
    doubleSpinBoxX->setMaximum(static_cast<double>(maxX));
    doubleSpinBoxX->setValue(bLayersEditable ? static_cast<double>(pMulPlate->x()):
                                               static_cast<double>(pPos->x()));
    gridLayout->addWidget(doubleSpinBoxX, rowNum, 1, 1, 1);
    doubleSpinBoxX->setEnabled(bLayersEditable);

    ++rowNum;

    auto label_2 = new QLabel(gridLayoutWidget);
    label_2->setObjectName(QStringLiteral("label_2"));
    label_2->setText(Y_DEF);
    gridLayout->addWidget(label_2, rowNum, 0, 1, 1);

    doubleSpinBoxY = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxY->setObjectName(QStringLiteral("doubleSpinBox_2"));
    doubleSpinBoxY->setMaximum(static_cast<double>(maxY));
    doubleSpinBoxY->setValue(bLayersEditable ? static_cast<double>(pMulPlate->y()) :
                                                 pPos->y());
    gridLayout->addWidget(doubleSpinBoxY, rowNum, 1, 1, 1);
    doubleSpinBoxY->setEnabled(bLayersEditable);

    ++rowNum;

    auto label_3 = new QLabel(gridLayoutWidget);
    label_3->setObjectName(QStringLiteral("label_3"));
    label_3->setText(D_IN_DEF);
    gridLayout->addWidget(label_3, rowNum, 0, 1, 1);

    auto geom = pMulPlate->getChildren()->at(0)->getGeometry();
    doubleSpinBoxDin = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxDin->setObjectName(QStringLiteral("doubleSpinBox_3"));
    doubleSpinBoxDin->setValue(static_cast<double>(geom.rpl_g.d_in));
    gridLayout->addWidget(doubleSpinBoxDin, rowNum, 1, 1, 1);

    ++rowNum;

    auto label_4 = new QLabel(gridLayoutWidget);
    label_4->setObjectName(QStringLiteral("label_4"));
    label_4->setText(D_EX_DEF);
    gridLayout->addWidget(label_4, rowNum, 0, 1, 1);

    doubleSpinBoxDex = new QDoubleSpinBox(gridLayoutWidget);
    doubleSpinBoxDex->setObjectName(QStringLiteral("doubleSpinBox_4"));
    doubleSpinBoxDex->setValue(static_cast<double>(geom.rpl_g.d_ex));
    gridLayout->addWidget(doubleSpinBoxDex, rowNum, 1, 1, 1);

    ++rowNum;

    auto label_5 = new QLabel(gridLayoutWidget);
    label_5->setObjectName(QStringLiteral("label_5"));
    label_5->setText(tr("First layer"));
    gridLayout->addWidget(label_5, rowNum, 0, 1, 1);

    comboBoxL1 = new LayersCombo(LayersCombo::ComboType::Active,
                               pMulPlate->getFirstPlate()->getLevel(),
                               gridLayoutWidget);
    comboBoxL1->setObjectName(QStringLiteral("comboBox"));
    gridLayout->addWidget(comboBoxL1, rowNum, 1, 1, 1);
    if(!bLayersEditable)
       comboBoxL1->setEnabled(false);

    ++rowNum;

    auto label_6 = new QLabel(gridLayoutWidget);
    label_6->setObjectName(QStringLiteral("label_6"));
    label_6->setText(tr("Second layer"));
    gridLayout->addWidget(label_6, rowNum, 0, 1, 1);

    comboBoxL2 = new LayersCombo(LayersCombo::ComboType::Active,
                                 pMulPlate->getSecondPlate()->getLevel(),
                                 gridLayoutWidget);
    comboBoxL2->setObjectName(QStringLiteral("comboBox_2"));
    gridLayout->addWidget(comboBoxL2, rowNum, 1, 1, 1);
    if(!bLayersEditable)
       comboBoxL2->setEnabled(false);


    connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));
}

void EditMultiPlate::accept()
{
   if(bLayersEditable)
   {
      //it means that we are editing new item
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
   }
   else
   {
      //we are editing exising item
      pProps->rpl_g.d_ex = doubleSpinBoxDex->value();
      pProps->rpl_g.d_in = doubleSpinBoxDin->value();
      //set position

   }
   strcpy(pName,m_pNameEdit->text().toStdString().c_str());
   QDialog::accept();
}

EditMultiPlate::~EditMultiPlate()
{

}
