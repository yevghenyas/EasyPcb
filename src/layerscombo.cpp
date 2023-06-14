#include "layerscombo.h"

LayersCombo::LayersCombo(ComboType type,BOARD_LEVEL_ID lSel,
                         QWidget *parent) : QComboBox(parent)
{
   if(type == ComboType::Active)
      LevelsWrapper::getActiveLevels(layers);
   else if(type == ComboType::Available)
      LevelsWrapper::getAvalableLevels(layers);
   else if(type == ComboType::Drawable)
      LevelsWrapper::getDrawableLevels(layers);
   else
      LevelsWrapper::getBaseContainerLayers(layers);

   size_t selInd = 0;
   for(size_t i = 0; i < layers.size();++i)
   {
      addItem(layers[i].desc);
      if(layers[i].idLevel == lSel)
         selInd = i;
   }
   setCurrentIndex(static_cast<int>(selInd));
}

BOARD_LEVEL_ID LayersCombo::getSelectedLayerId()
{
   return layers[currentIndex()].idLevel;
}

void LayersCombo::setSelectedLayerId(BOARD_LEVEL_ID lSel)
{
   for(size_t i = 0; i < layers.size() ; ++i)
      if(layers[i].idLevel == lSel)
      {
         setCurrentIndex(i);
      }
}
