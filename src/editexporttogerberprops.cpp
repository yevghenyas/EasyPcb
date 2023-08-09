#include "editexporttogerberprops.h"
#include <QDir>
#include <QFileDialog>
//#include <QThread>
#include <thread>
#include "gerbergenerator.h"
#include <fstream>
//#include <ctime>

EditExportToGerberProps::EditExportToGerberProps(BoardLayersWrapper *pB,
                  set<BOARD_LEVEL_ID>&& lSet,
                  float bW,float bH,
                  QString& name):
                  pBoardLayers(pB),layersSet(std::move(lSet)),
                  boardWidth(bW),boardHeight(bH),
                  schemaName(name)
{
   createComponents();
}

void EditExportToGerberProps::createComponents()
{
   resize(544, 468);
   buttonBox = new QDialogButtonBox(this);
   buttonBox->setObjectName(QStringLiteral("buttonBox"));
   buttonBox->setGeometry(QRect(170, 430, 341, 32));
   buttonBox->setOrientation(Qt::Horizontal);
   buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok |QDialogButtonBox::Apply);

   verticalLayoutWidget = new QWidget(this);
   verticalLayoutWidget->setObjectName(QStringLiteral("verticalLayoutWidget"));
   verticalLayoutWidget->setGeometry(QRect(20, 10, 501, 401));
   verticalLayout = new QVBoxLayout(verticalLayoutWidget);
   verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
   verticalLayout->setContentsMargins(0, 0, 0, 0);
   horizontalLayout = new QHBoxLayout();
   horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
   pushButton = new QPushButton(verticalLayoutWidget);
   pushButton->setObjectName(QStringLiteral("SelOutFolder"));
   pushButton->setText(tr("Output folder"));
   QSizePolicy policy(QSizePolicy::Policy::Fixed,
                      QSizePolicy::Policy::Fixed);
   pushButton->setSizePolicy(policy);

   horizontalLayout->addWidget(pushButton);

   folderLabel = new QLabel(verticalLayoutWidget);
   folderLabel->setObjectName(QStringLiteral("OutFolderLabel"));
   folderLabel->setWordWrap(true);
   folderLabel->setText(QDir::currentPath());

   horizontalLayout->addWidget(folderLabel);

   verticalLayout->addLayout(horizontalLayout);

   auto line_2 = new QFrame(verticalLayoutWidget);
   line_2->setObjectName(QStringLiteral("line_2"));
   line_2->setFrameShape(QFrame::HLine);
   line_2->setFrameShadow(QFrame::Sunken);

   verticalLayout->addWidget(line_2);


   layersGrid = new QGridLayout();
   layersGrid->setObjectName(QStringLiteral("layersGrid"));

   cout<<"l_size="<<layersSet.size()<<endl;
   LevelsWrapper::getActiveLevels(m_ActiveLevels);

   auto chkIndex = 0;
   for(std::vector<BoardLevel>::size_type i = 0; i < m_ActiveLevels.size(); ++i)
   {
      if(layersSet.find(m_ActiveLevels[i].idLevel) != layersSet.end())
      {
         cout<<"active="<<m_ActiveLevels[i].idLevel<<endl;
         QCheckBox *checkT = new QCheckBox(verticalLayoutWidget);
         checkT->setObjectName(m_ActiveLevels[i].desc);
         checkT->setText(m_ActiveLevels[i].desc);
         QPixmap buffer(32,32);
         buffer.fill(m_ActiveLevels[i].color);
         checkT->setIcon(buffer);
         checkT->setCheckState(Qt::CheckState::Checked);
         layersGrid->addWidget(checkT, i, 0, 1, 1);
         layersBoxes.push_back(checkT);
         connect(checkT,&QCheckBox::clicked,this,&EditExportToGerberProps::onCheckLayer);

         QCheckBox *checkM = new QCheckBox(verticalLayoutWidget);
         checkM->setObjectName(QString::number(i));
         checkM->setText(tr("Mirrowed"));
         layersGrid->addWidget(checkM, i, 1, 1, 1);
         mirrowedChecks.push_back(checkM);
         chkIndexToVIndex[chkIndex] = i;
         ++chkIndex;
      }
   }



   verticalLayout->addLayout(layersGrid);

   auto line_3 = new QFrame(verticalLayoutWidget);
   line_3->setObjectName(QStringLiteral("line_3"));
   line_3->setFrameShape(QFrame::HLine);
   line_3->setFrameShadow(QFrame::Sunken);

   verticalLayout->addWidget(line_3);


   label_6 = new QLabel(verticalLayoutWidget);
   label_6->setObjectName(QStringLiteral("label_6"));
   label_6->setText(tr("Number of integer digits"));

   verticalLayout->addWidget(label_6);

   intNumSpinBox = new QSpinBox(verticalLayoutWidget);
   intNumSpinBox->setObjectName(QStringLiteral("spinBox"));
   intNumSpinBox->setMaximum(intMaxNumForGerber);
   intNumSpinBox->setMinimum(intMinNumForGerber);
   intNumSpinBox->setValue(intDefNumForGerber);

   verticalLayout->addWidget(intNumSpinBox);

   label_7 = new QLabel(verticalLayoutWidget);
   label_7->setObjectName(QStringLiteral("label_7"));
   label_7->setText(tr("Number of decimal digits"));

   verticalLayout->addWidget(label_7);

   decNumSpinBox = new QSpinBox(verticalLayoutWidget);
   decNumSpinBox->setObjectName(QStringLiteral("spinBox_2"));
   decNumSpinBox->setMaximum(decMaxNumForGerber);
   decNumSpinBox->setMinimum(decMinNumForGerber);
   decNumSpinBox->setValue(decDefNumForGerber);



   verticalLayout->addWidget(decNumSpinBox);

   auto label_8 = new QLabel(verticalLayoutWidget);
   label_8->setObjectName(QStringLiteral("label_8"));
   label_8->setText(tr("List of generated files"));
   verticalLayout->addWidget(label_8);

   listWidget = new QListWidget(verticalLayoutWidget);
   listWidget->setObjectName(QStringLiteral("listWidget"));

   verticalLayout->addWidget(listWidget);

   connect(pushButton,&QPushButton::clicked,this,&EditExportToGerberProps::selectOutputFolder);
   QObject::connect(buttonBox,&QDialogButtonBox::accepted,this,&EditExportToGerberProps::onOk);
   QObject::connect(buttonBox,&QDialogButtonBox::rejected,this,&EditExportToGerberProps::reject);
   QObject::connect(buttonBox,&QDialogButtonBox::clicked,this,&EditExportToGerberProps::onApply);
}

void EditExportToGerberProps::selectOutputFolder()
{
   QString dirName =
           QFileDialog::getExistingDirectory(this,
                                             tr("Select folder"),
                                             QDir::currentPath());
   if(!dirName.isEmpty())
       folderLabel->setText(dirName);
}

void EditExportToGerberProps::onApply(QAbstractButton *b)
{
   if(buttonBox->buttonRole(b) == QDialogButtonBox::ApplyRole)
      exportToGerber();
}


void EditExportToGerberProps::onOk()
{
   exportToGerber();
   //let user see the results in box
   std::this_thread::sleep_for(std::chrono::milliseconds(500));
   //close dialog
   EditExportToGerberProps::accept();
}

void EditExportToGerberProps::exportToGerber()
{
   auto maxIntValue = intNumSpinBox->value();
   auto maxDecValue = decNumSpinBox->value();
   for(std::vector<QCheckBox*>::size_type i = 0; i < layersBoxes.size() ; ++i)
   {
      if(layersBoxes[i]->checkState() == Qt::CheckState::Checked)
      {
         //find layer that corresponds to checkbox
         auto iter = chkIndexToVIndex.find(i);
         if(iter == chkIndexToVIndex.end())
            continue;
         //construct name of output file
         QString fileName = QFileInfo(schemaName).baseName() +
                 "_" + m_ActiveLevels[iter->second].desc;
         fileName += "_";
         auto tt = std::chrono::system_clock::to_time_t(
                     std::chrono::system_clock::now());
         fileName += QString::number(tt);
         fileName += ".gbr";
         //construct full path
         auto dirStr = folderLabel->text() + "/" + fileName ;
         //create output file
         fstream out_gerber(dirStr.toStdString(),std::fstream::in | std::fstream::out | std::fstream::app);
         //create instance of GerberGenerator and set its properties
         GerberGenerator gerberGen(maxIntValue,maxDecValue,maxIntValue,maxDecValue);
         gerberGen.setboardMaxX(boardWidth);
         gerberGen.setboardMaxY(boardHeight);
         gerberGen.setMirrowed(
                     mirrowedChecks[i]->checkState() == Qt::CheckState::Checked);
         //fill file with data
         out_gerber<<gerberGen.getInitGerberString();
         for (auto item:*pBoardLayers->getLayer(
               m_ActiveLevels[iter->second].idLevel)->getConnectItemsInLevel())
            out_gerber<<item.second->getGerberString(gerberGen,
                                   m_ActiveLevels[iter->second].idLevel)<<endl;
         for (auto item:*pBoardLayers->getLayer(m_ActiveLevels[iter->second].idLevel)->getItemsInLevel())
            out_gerber<<item.second->getGerberString(gerberGen,
                                                     m_ActiveLevels[iter->second].idLevel)<<endl;

         out_gerber<<gerberGen.getFinishGerberString()<<endl;
         out_gerber.close();
         //log the path to file for user
         listWidget->addItem(dirStr);
      }
   }
}
void EditExportToGerberProps::onCheckLayer(bool )
{
   auto numOfChecked = 0;
   for(vector<QCheckBox*>::size_type i = 0;
       i < layersBoxes.size(); ++i)
   {
      if(layersBoxes[i]->checkState() == Qt::CheckState::Checked)
      {
         ++numOfChecked;
         mirrowedChecks[i]->setEnabled(true);
      }
      else
      {
         mirrowedChecks[i]->setEnabled(false);
      }
   }
   if(numOfChecked == 0)
   {
      buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(false);
      buttonBox->button(QDialogButtonBox::StandardButton::Apply)->setEnabled(false);
   }
   else
   {
      buttonBox->button(QDialogButtonBox::StandardButton::Ok)->setEnabled(true);
      buttonBox->button(QDialogButtonBox::StandardButton::Apply)->setEnabled(true);
   }
}
