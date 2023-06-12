/*
 * Copyright 2018 <copyright holder> <email>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */
#include <QMessageBox>
#include "addneweltolibdlg.h"
#include "common.h"




AddNewElToLibDlg::AddNewElToLibDlg(QWidget *parent):QDialog(parent)
{
   if (this->objectName().isEmpty())
      this->setObjectName(QString::fromUtf8("Dialog"));
   this->resize(400, 300);
   buttonBox = new QDialogButtonBox(this);
   buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
   buttonBox->setGeometry(QRect(290, 20, 81, 241));
   buttonBox->setOrientation(Qt::Vertical);
   buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
   verticalLayoutWidget = new QWidget(this);
   verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
   verticalLayoutWidget->setGeometry(QRect(9, 9, 261, 281));
   verticalLayout = new QVBoxLayout(verticalLayoutWidget);
   verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
   verticalLayout->setContentsMargins(0, 0, 0, 0);
   
   labelName = new QLabel(verticalLayoutWidget);
   labelName->setObjectName(QString::fromUtf8("label_2"));
   labelName->setText(QString::fromUtf8(LABEL_ADDTOLIB_DLG_NAME));

   verticalLayout->addWidget(labelName);

   plainTextEditName = new QPlainTextEdit(verticalLayoutWidget);
   plainTextEditName->setObjectName(QString::fromUtf8("plainTextEdit_2"));

   verticalLayout->addWidget(plainTextEditName);   
   
   label = new QLabel(verticalLayoutWidget);
   label->setObjectName(QString::fromUtf8("label"));
   label->setWordWrap(true);
   label->setText(QString::fromUtf8(LABEL_ADDTOLIB_DLG));

   verticalLayout->addWidget(label);

   plainTextEdit = new QPlainTextEdit(verticalLayoutWidget);
   plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

   verticalLayout->addWidget(plainTextEdit);
   
   checkBox = new QCheckBox(verticalLayoutWidget);
   checkBox->setObjectName(QString::fromUtf8("checkBox"));
   checkBox->setText(QString::fromUtf8(CHKBX_ADDTOLIB_DLG));

   verticalLayout->addWidget(checkBox);


//   this->setWindowTitle(QApplication::translate("Dialog", "Dialog", 0, QApplication::UUnicodeUTF8));
   QObject::connect(buttonBox, SIGNAL(accepted()), this, SLOT(accept()));
   QObject::connect(buttonBox, SIGNAL(rejected()), this, SLOT(reject()));

}

void AddNewElToLibDlg::getResult(vector< QString >& paths,QString &name)
{
   paths = plainTextEdit->toPlainText().split('\\').toVector().toStdVector();
   name = plainTextEditName->toPlainText();
}

void AddNewElToLibDlg::accept()
{
   if(plainTextEdit->toPlainText().size() == 0 || plainTextEditName->toPlainText().size() == 0)
   {
      QMessageBox msgBox;
      msgBox.setIcon(QMessageBox::QMessageBox::Critical); 
      msgBox.setInformativeText(QString::fromUtf8(ADDTOLIB_DLG_FIELDS_REQUIRED_EMPTY_MSG)); 
      msgBox.setStandardButtons(QMessageBox::Ok); 
      msgBox.exec();
      return;
   }
   QDialog::accept();
}


AddNewElToLibDlg::~AddNewElToLibDlg()
{

}
