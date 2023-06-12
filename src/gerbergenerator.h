#ifndef GERBERGENERATOR_H
#define GERBERGENERATOR_H

#include "common.h"

class GerberGenerator
{
   int maxIntX{2};
   int maxIntY{2};
   int maxPointX{2};
   int maxPointY{2};
   float boardMaxY{0};
   float boardMaxX{0};
   bool isInitialized{false};
   bool isInterpolationSet{false};
   bool isMirrowed{true};
public:
   GerberGenerator(int mIntX,int mIntY,
                   int mPointX,int mPointY);

   void setboardMaxY(float y);
   void setboardMaxX(float x);
   void setMirrowed(bool mirrowed){isMirrowed = mirrowed;}
   void formatFloat(float val,char *buf ,int intLen,int fracLen);
   void formatFloatX(float val,char *buf);
   void formatFloatY(float val,char *buf);
   string getInitGerberString();
   string getInterpolationString(int type);
   string getAppertureSetString(int idApperture);
   string getFlashString(float x,float y);
   string getStringForConnector(ITEM_ID id,float width,vector<PointF>& pts);
   string getStringForRoundPlate(ITEM_ID id,float x,float y,
                                 float dEx,float dIn);
   string getStringForRect(ITEM_ID id,float x,float y,
                                 float width,float heigh);

   string getStringForRectPckg(ITEM_ID id,float x,float y,
                               float width,float height,bool filled = false);

   string getStringForRoundPckg(ITEM_ID id,float x,float y,float d);

   string getAppertureForConnector(ITEM_ID id,float width);
   string getAppertureForRoundPlate(ITEM_ID id,float dEx,float dIn);
   string getAppertureForRect(ITEM_ID id,float width,float heigh);
   string getAppertureForRectPckg(ITEM_ID id,float width,float heigh);
   string getAppertureForRoundPckg(ITEM_ID id,float d);


   string getFinishGerberString(){return string("M02*");}
   string getCurrentPointString(float x,float y);
   string getCurrentPointSetStringX(float x);
   string getCurrentPointSetStringY(float y);
   string getCurrentPointSetString(float x,float y);
   string getCreateDrawString(float x,float y);

};

#endif // GERBERGENERATOR_H
