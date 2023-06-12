#ifndef LAYERSCOMBO_H
#define LAYERSCOMBO_H

#include "common.h"
#include <QComboBox>


class LayersCombo : public QComboBox
{
   vector<BoardLevel> layers{};
public:
   enum class ComboType {Active,Available,Drawable,BaseContainerLayers};
   LayersCombo(ComboType type,BOARD_LEVEL_ID lSel,QWidget *parent = nullptr);
   BOARD_LEVEL_ID getSelectedLayerId();
   void setSelectedLayerId(BOARD_LEVEL_ID lSel);
};

#endif // LAYERSCOMBO_H
