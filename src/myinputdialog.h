#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H
#include "common.h"
#include <QDialog>

class MyInputDialog
{
public:
    MyInputDialog();
    static QString getText(QWidget *parent,const QString& caption,
                           const QString& labelText, int textLength = itemNameSize - 1,const QString& text = QString());
};

#endif // MYINPUTDIALOG_H
