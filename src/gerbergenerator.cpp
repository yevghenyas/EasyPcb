 #include "gerbergenerator.h"

GerberGenerator::GerberGenerator(int mIntX,int mPointX,
                                 int mIntY,int mPointY):
                                 maxIntX(mIntX),maxIntY(mIntY),
                                 maxPointX(mPointX),maxPointY(mPointY)
{

}


void GerberGenerator::formatFloat(float val,char *buf ,
                                  int intLen,int fracLen)
{
   auto fractpart = 0.0f;
   auto intpart = 0.0f;
   fractpart  = std::modf(val,&intpart);
   for(int i = 0; i < fracLen; ++i)
      fractpart *= 10;

   char format[64];
   sprintf(format,"%%0%dd%%0%dd",intLen,fracLen);
   sprintf(buf,format,static_cast<int>(intpart),
           static_cast<int>(fractpart));

}

void GerberGenerator::formatFloatX(float val,char *buf)
{
   formatFloat(val,buf,maxIntY,maxPointY);
}

void GerberGenerator::formatFloatY(float val,char *buf)
{
   formatFloat(!isMirrowed ? boardMaxY - val : val,
               buf,maxIntY,maxPointY);
}

string GerberGenerator::getInitGerberString()
{
   string str = "%FSLAX"+std::to_string(maxIntX) +
           std::to_string(maxPointX) + "Y" + std::to_string(maxIntY) +
           std::to_string(maxPointY) + "*%\n%MOMM*%\n";
   return str;
}

string GerberGenerator::getStringForConnector(ITEM_ID id,float width,
                             vector<PointF>& pts)
{
   string text;
   text.reserve(512);
   //define apperture
   text = getAppertureForConnector(id,width);

   //set current apperture
   text += getAppertureSetString(id);

   //set current point
   text += getCurrentPointSetString(pts[0].x(),pts[0].y());

   //set interpolation
   text += getInterpolationString(1);

   for(std::size_t i = 1; i < pts.size() ; ++i)
      text += getCreateDrawString(pts[i].x(),pts[i].y());

   return text;
}

string GerberGenerator::getStringForRoundPlate(ITEM_ID id,float x,float y,
                              float dEx,float dIn)
{

   string text = getAppertureForRoundPlate(id,dEx,dIn);

   //set current apperture
   text += getAppertureSetString(id);
   //flash
   text += getFlashString(x,y);
   return text;
}

string GerberGenerator::getStringForRect(ITEM_ID id,float x,float y,
                              float width,float height)
{

   string text = getAppertureForRect(id,width,height);
   //set current apperture
   text += getAppertureSetString(id);
   //flash
   text += getFlashString(x,y);

   return text;
}

string GerberGenerator::getStringForRoundPckg(ITEM_ID id,float x,float y,
                              float d)
{

   string text = getAppertureForRoundPckg(id,d);
   //set current apperture
   text += getAppertureSetString(id);
   //flash
   text += getFlashString(x,y);

   return text;
}




string GerberGenerator::getStringForRectPckg(ITEM_ID id,float x,float y,
                            float width,float height,bool filled)
{
   string text;
   if(!filled)
   {
      //define apperture
      text = getAppertureForConnector(id,static_cast<float>(PACK_LINE_DEF_W) * 2 / 10);

      //set current apperture
      text += getAppertureSetString(id);

      float xx = x + width/2;
      float yy = y - height/2;

      //set current point
      text += getCurrentPointSetString(xx,yy);

      //set interpolation
      text += getInterpolationString(1);

      // draw package
      yy = y + height/2;
      text += getCreateDrawString(xx,yy);

      xx = x - width/2;
      text += getCreateDrawString(xx,yy);

      yy = y - height/2;
      text += getCreateDrawString(xx,yy);

      xx = x + width/2;
      text += getCreateDrawString(xx,yy);
   }
   else
   {
      text = getStringForRect(id,x,y,width,height);
   }
   return text;
}


string GerberGenerator::getAppertureForConnector(ITEM_ID id,float width)
{
   string text = "%ADD" + std::to_string(id) + "C," +
           std::to_string(width)+"*%\n";

   return text;
}
string GerberGenerator::getAppertureForRoundPlate(ITEM_ID id,float dEx,float dIn)
{
    string text = "%ADD" + std::to_string(id) + "C," +
            std::to_string(dEx)+"*%\n";

   return text;
}
string GerberGenerator::getAppertureForRect(ITEM_ID id,float width,float height)
{
    string text = "%ADD" + std::to_string(id) + "R," +
            std::to_string(width)+"X"+
            std::to_string(height) + "*%\n";

   return text;
}

string GerberGenerator::getAppertureForRectPckg(ITEM_ID id,float width,float height)
{
    string text = "%ADD" + std::to_string(id) + "R," +
            std::to_string(width)+"X"+
            std::to_string(height) + "*%\n";

   return text;
}

string GerberGenerator::getAppertureForRoundPckg(ITEM_ID id,float d)
{
    string text = "%ADD" + std::to_string(id) + "C," +
            std::to_string(d) + "X" +
            std::to_string(d -
              static_cast<float>(PACK_LINE_DEF_W)/PIXELS_PER_MM * 4) + "*%\n";

   return text;
}



string GerberGenerator::getAppertureSetString(int idApperture)
{
   string text = "D" + std::to_string(idApperture) + "*\n";
   return text;
}

string GerberGenerator::getInterpolationString(int type)
{
   string text;
   if(!isInterpolationSet)
   {
      text += "G0" + std::to_string(type) + "*\n";
      isInterpolationSet = true;
   }
   return text;
}

string GerberGenerator::getFlashString(float x,float y)
{
   string text = getCurrentPointString(x,y);
   text += "D03*\n";
   return text;
}

string GerberGenerator::getCurrentPointString(float x,float y)
{
   string text;
   char bufX[64];
   char bufY[64];
   formatFloatX(x,bufX);
   formatFloatY(y,bufY);
   text += "X";
   text += bufX;
   text += "Y";
   text += bufY;
   return text;
}


string GerberGenerator::getCurrentPointSetString(float x,float y)
{
   string text = getCurrentPointString(x,y);
   text += "D02*\n";
   return text;
}

string GerberGenerator::getCurrentPointSetStringX(float x)
{
    string text;
    char bufX[64];
    formatFloatX(x,bufX);
    text += "X";
    text += bufX;
    text += "D02*\n";
    return text;
}

string GerberGenerator::getCurrentPointSetStringY(float y)
{
    string text;
    char bufY[64];
    formatFloatX(y,bufY);
    text += "Y";
    text += bufY;
    text += "D02*\n";
    return text;
}


string GerberGenerator::getCreateDrawString(float x,float y)
{
   string text = getCurrentPointString(x,y);
   text += "D01*\n";
   return text;
}

void GerberGenerator::setboardMaxY(float y)
{
   boardMaxY = y;
}

void GerberGenerator::setboardMaxX(float x)
{
   boardMaxX = x;
}
