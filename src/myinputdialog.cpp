#include "myinputdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QLabel>
#include <QDialogButtonBox>

MyInputDialog::MyInputDialog()
{

}

QString MyInputDialog::getText(QWidget *parent,const QString& caption,const QString& labelText,int textLength,const QString& text)
{
    QDialog dlg(parent);
    dlg.setWindowTitle(caption);
    dlg.resize(200,100);
    auto buttonBox = new QDialogButtonBox(&dlg);
    buttonBox->setObjectName(QStringLiteral("buttonBox"));
//    buttonBox->setGeometry(20, 80, 160, 30);
    buttonBox->setOrientation(Qt::Horizontal);
    buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

    QHBoxLayout *hLayout = new QHBoxLayout();
    QLabel *label = new QLabel(labelText);
    hLayout->addWidget(label);
    QLineEdit *edit = new QLineEdit(text.left(textLength));
    edit->setMaxLength(textLength);
    hLayout->addWidget(edit);
    QVBoxLayout *vBox = new QVBoxLayout();
    vBox->addLayout(hLayout);
    vBox->addWidget(buttonBox);
    dlg.setLayout(vBox);
    dlg.connect(buttonBox,&QDialogButtonBox::accepted , &dlg, &QDialog::accept);
    dlg.connect(buttonBox, &QDialogButtonBox::rejected, &dlg, &QDialog::reject);

    auto res = dlg.exec();
    if(res == QDialog::Accepted)
       return edit->text();
    else
       return QString();
}
