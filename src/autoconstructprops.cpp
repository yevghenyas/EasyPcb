#include "autoconstructprops.h"
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QCheckBox>
#include <QLabel>


AutoConstructProps::AutoConstructProps(set<BOARD_LEVEL_ID>&& l):layersSet(l)
{
    if (objectName().isEmpty())
       setObjectName(QString::fromUtf8("Dialog"));
    resize(400, 340);
    auto buttonBox = new QDialogButtonBox(this);
    buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
    buttonBox->setGeometry(QRect(30, 280, 341, 32));
    buttonBox->setOrientation(Qt::Horizontal);

    auto formLayoutWidget = new QWidget(this);
    formLayoutWidget->setObjectName(QString::fromUtf8("formLayoutWidget"));


    auto formLayout = new QFormLayout(formLayoutWidget);
    formLayout->setObjectName(QStringLiteral("formLayout"));
    formLayout->setContentsMargins(20, 20, 20, 20);

    auto label = new QLabel(formLayoutWidget);
    label->setObjectName(QString::fromUtf8("label"));
    label->setText(QString::fromUtf8("Min connector width"));
    formLayout->setWidget(0, QFormLayout::LabelRole, label);


    doubleSpinBox = new QDoubleSpinBox(formLayoutWidget);
    doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
    doubleSpinBox->setMaximum(static_cast<double>(20.0f));
    doubleSpinBox->setValue(static_cast<double>(fSizeOfMatrixCell));
    formLayout->setWidget(0, QFormLayout::FieldRole, doubleSpinBox);


    auto label1 = new QLabel(formLayoutWidget);
    label1->setObjectName(QString::fromUtf8("label1"));
    label1->setText(QString::fromUtf8("Select layers you \nwant to use \nduring processing"));

    formLayout->setWidget(1, QFormLayout::LabelRole, label1);

    auto checkLayout = new QVBoxLayout();
    checkLayout->setObjectName(QStringLiteral("checkLayout"));


    LevelsWrapper::getActiveLevels(m_activeLayers);


    for(size_t index = 0; index < m_activeLayers.size(); ++index)
    {
       auto checkBox = new QCheckBox;
       checkBox->setText(m_activeLayers[index].desc);
       QPixmap buffer(32,32);
       buffer.fill(m_activeLayers[index].color);
       checkBox->setIcon(buffer);
       //check if the layer is used by one of the schematic connectors
       //if yes set this checkbox checked and disabled
       if(layersSet.find(m_activeLayers[index].idLevel) != layersSet.end())
       {
          checkBox->setCheckState(Qt::CheckState::Checked);
          checkBox->setEnabled(false);
       }
       checkLayout->addWidget(checkBox);
       m_checkedLayers.push_back(checkBox);
    }

    formLayout->setLayout(1, QFormLayout::FieldRole, checkLayout);

    QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
    QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));


}

bool AutoConstructProps::getResult(float& fConWidth,vector<BOARD_LEVEL_ID>& layers)
{
   fConWidth = static_cast<float>(doubleSpinBox->value());
   for(size_t index = 0; index < m_checkedLayers.size() ; ++index)
      if(m_checkedLayers[index]->checkState() == Qt::CheckState::Checked)
         layers.push_back(m_activeLayers[index].idLevel);
}
