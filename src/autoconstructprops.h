#ifndef AUTOCONSTRUCTPROPS_H
#define AUTOCONSTRUCTPROPS_H


#include "common.h"
#include <QDialog>
#include <QCheckBox>
#include <QDoubleSpinBox>


class AutoConstructProps : public QDialog
{
   QDoubleSpinBox *doubleSpinBox{nullptr};
   vector<BoardLevel> m_activeLayers;
   vector<QCheckBox*> m_checkedLayers;
   set<BOARD_LEVEL_ID> layersSet;
public:
   AutoConstructProps(set<BOARD_LEVEL_ID>&& l);
   bool getResult(float& fConWidth,vector<BOARD_LEVEL_ID>& layers);
};

#endif // AUTOCONSTRUCTPROPS_H
