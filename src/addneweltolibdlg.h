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

#ifndef ADDNEWELTOLIBDLG_H
#define ADDNEWELTOLIBDLG_H

#include <qdialog.h>
#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QHeaderView>
#include <QPlainTextEdit>
#include <QLabel>
#include <QVBoxLayout>
#include <QWidget>
#include <QCheckBox>
#include <QString>
#include <vector>
using namespace std;

class AddNewElToLibDlg :  public QDialog
{
    QDialogButtonBox *buttonBox;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QLabel *labelName;
    QPlainTextEdit *plainTextEditName;
    QLabel *label;
    QPlainTextEdit *plainTextEdit;
    QCheckBox *checkBox;
    QString result;
public:
    AddNewElToLibDlg(QWidget *parent);
    ~AddNewElToLibDlg();
    virtual void accept();
    void getResult(vector<QString>& paths,QString &name);
};

#endif // ADDNEWELTOLIBDLG_H
