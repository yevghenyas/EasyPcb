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

#ifndef GRAPHICALITEMPROPSDLG_H
#define GRAPHICALITEMPROPSDLG_H

#include <QVariant>
#include <QAction>
#include <QApplication>
#include <QButtonGroup>
#include <QDialog>
#include <QDialogButtonBox>
#include <QSpinBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QHeaderView>
#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QRadioButton>
#include "graphicalitem.h"
#include "layerscombo.h"
#include <QPlainTextEdit>

enum class ContainerType {NonContainer,NonRelocatableCont,Multiplate,RelocatableDipType,RelocatableSoType};

class GraphicalItemPropsDlg : public QDialog
{
    Q_OBJECT
    QDialogButtonBox *buttonBox;
    QWidget *gridLayoutWidget;
    QGridLayout *gridLayout;
    // item position
    QDoubleSpinBox *posXSpinBox{nullptr};
    QDoubleSpinBox *posYSpinBox{nullptr};
    // item geometry. For different types of items may
    // have different meanings
    QDoubleSpinBox *geomASpinBox{nullptr};
    QDoubleSpinBox *geomBSpinBox{nullptr};
    // when checked means that item position and geomentry will
    // be updated
    QCheckBox *updPropsChk{nullptr};
    // displays current and available layers for the
    // item to move into
    LayersCombo *layersCombo{nullptr};
    //this set of checkboxes each one for layer
    //displayed for multiplate instead of combo
    vector<QCheckBox*> multiplateChks;
    // level is the same as layer
    // map combo_index -> levelId
    map <int ,BOARD_LEVEL_ID> m_levels;
    SmartPtr<GraphicalItem> m_pItem;
    //contains possible orientation angles in degrees i.e. if the item rotated
    QComboBox *orientationCombo{nullptr};
    //contains font sizes
    QComboBox *fntCombo{nullptr};
    //Contains text for text field
    QPlainTextEdit *pTextEdit;
    //Squared line
    QRadioButton *radioRound;
    QRadioButton *radioSquared;
    QCheckBox *checkSqStyle;
    //round shape may be an arc or chord.
    QDoubleSpinBox *geomStAngSpin;
    QDoubleSpinBox *geomSpAngSpin;
    //Chord or arc
    QCheckBox *chkChord;
    //returns one of the ContainerType values
    ContainerType checkContainerType(GraphicalItem *p,set<BOARD_LEVEL_ID>& levelIds);
    void setFieldsForRects(float width,float height,int& rowCounter);
    //d_ex and d_ex1 - external diameters(when d_ex == d_ex1 the ellipse becomes a circle)
    //d_in - internal diameter
    void setFieldsForRounds(float d_ex,float d_ex1,float d_in,int& rowCounter);
    void setFieldsForRoundShape(float d_ex,float ,
                                                   float d_in,
                                                   int a_start,
                                                   int a_span,
                                                   bool closed,
                                                   int& rowCounter);

    void setFieldsForLine(float w,int& rowCounter);
    void setFieldsForText(int fntSize,ITEMS_ORIENTATION o,const QString& text,int& rowCounter);
    //creates combo for layers
    void createLayersCombo(ContainerType type,set<BOARD_LEVEL_ID>& ids,
                           vector<BoardLevel>& v,int rowCounter);
    void createFntCombo(int rowCounter,int initFnt );

    //creates combo for selecting orientation(rotation) angles
    void createOrientationCombo(int rowCounter,ITEMS_ORIENTATION initOrient);

    //creates label -> spinBox
    void createOneRow(const char *szLabelName,
                                 const char *szSpinName,
                                 QDoubleSpinBox **spinBox,
                                 double value,
                                 int row,
                                 int minValue = 0,
                                 int maxValue = 500);

    void drawLineForRadio(QPixmap *buf,LINE_STYLE st);

public:   
    GraphicalItemPropsDlg(QWidget *parent,SmartPtr<GraphicalItem>& item);
    virtual ~GraphicalItemPropsDlg() override;
    //returns result of dialog
    //true - item geometry was updated
    //false - item geometry was not updated
    //all parameters are [in]
    bool getResult(BOARD_LEVEL_ID& level,shared_ptr<PointF>& pos,
                   shared_ptr<GeomCommonProps>& props,
                   ContainerType& containerType,
                   ITEMS_ORIENTATION& o);
public slots:
    void processLevel(int index);
    void accept() override;
    void changedProperties(double value);
};

#endif // GRAPHICALITEMPROPSDLG_H
