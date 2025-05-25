#ifndef AUTOCONSTRUCTPROPS_H
#define AUTOCONSTRUCTPROPS_H


#include "common.h"
#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QSpinBox>


class AutoConstructProps : public QDialog
{
   Q_OBJECT
   QDoubleSpinBox *doubleSpinBox{nullptr};
   vector<BoardLevel> m_activeLayers;
   vector<QCheckBox*> m_checkedLayers;
   set<BOARD_LEVEL_ID> layersSet;
   QSpinBox *iterNumSpinBox;
public:
   AutoConstructProps(set<BOARD_LEVEL_ID>&& l);
   bool getResult(float& fConWidth,vector<BOARD_LEVEL_ID>& layers,int& iterNum);
public slots:
   void boxChecked(int state);
};

#endif // AUTOCONSTRUCTPROPS_H
