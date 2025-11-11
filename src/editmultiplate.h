#ifndef EDITMULTIPLATE_H
#define EDITMULTIPLATE_H

#include <QDialog>
#include "common.h"
#include "multiplategraphicalitem.h"
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QLabel>
#include "layerscombo.h"



class EditMultiPlate : public QDialog
{
    Q_OBJECT

    MultiplateGraphicalItem *pMulPlate{nullptr};
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QDoubleSpinBox *doubleSpinBoxX;
    LayersCombo *comboBoxL1;
    QDoubleSpinBox *doubleSpinBoxDex;
    QDoubleSpinBox *doubleSpinBoxDin;
    QDoubleSpinBox *doubleSpinBoxY;
    LayersCombo *comboBoxL2;
    float maxX,maxY;
    bool bLayersEditable{true};
    shared_ptr<PointF> pPos;
    shared_ptr<GeomCommonProps> pProps;

public:
    explicit EditMultiPlate(MultiplateGraphicalItem *p,
                            float mX,float my,
                            QWidget *parent = nullptr);
    explicit EditMultiPlate(MultiplateGraphicalItem *p,
                            shared_ptr<PointF>& pos,
                            shared_ptr<GeomCommonProps>& props,
                            QWidget *parent = nullptr);
    void createContent();
    virtual void accept();

    ~EditMultiPlate();

};

#endif // EDITMULTIPLATE_H
