#ifndef EXCELLONGENERATOR_H
#define EXCELLONGENERATOR_H

#include "boardlayerswrapper.h"
#include <iostream>

class ExcellonGenerator
{
   BoardLayersWrapper *pBoardLayers{nullptr};
public:
   ExcellonGenerator(BoardLayersWrapper *p);
   void formatFloat(float val,char *buf ,
                                     int intLen,int fracLen);
   void generateFile(fstream& out);
};

#endif // EXCELLONGENERATOR_H
