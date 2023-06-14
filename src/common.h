#ifndef COMMON_H
#define COMMON_H

#include <QColor>
#include <vector>
#include <QPointF>
#include <mutex>
#include <stdio.h>
#include <memory>
#include <iostream>
#include <atomic>
#include <unordered_set>
#include <set>
using namespace std;

#define _SCROLL

constexpr float PI = 3.14159265358979323846f;

enum MODE_DRAW
{
    MODE_CURSOR,
    MODE_PLATE,
    MODE_LINE,
    MODE_MOVE,
    MODE_NODE_PRE_MOVE,
    MODE_NODE_MOVE,
    MODE_CHIP,
    MODE_PRINT,
    MODE_PLATE_RECT,
    MODE_PACK_RECT,
    MODE_PACK_ROUND,
    MODE_SIMPLE_RULER,
    MODE_MULTIPLATE,
    MODE_VC_CON,
    MODE_TEXT,
    MODE_CONNECTED_RULER
};
//Level A Connectors ,A1 - packages of elements
enum BOARD_LEVEL_ID {LEVEL_A1 = 0x0001,LEVEL_A = 0x0002,LEVEL_B = 0x0004,LEVEL_C = 0x0008,LEVEL_D = 0x0010,LEVEL_E = 0x0020,LEVEL_F = 0x0040,LEVEL_F1 = 0x0080,LEVEL_VC = 0x0100,LEVEL_NONE = 0x0000,LEVEL_ALL = 0x1000};

//enum ITEMS_ORIENTATION {O_VERTICAL_TOP,O_VERTICAL_BOTTOM,O_HORIZONTAL_LEFT,O_HORIZONTAL_RIGHT};

enum ITEMS_ORIENTATION {O_HORIZONTAL_LEFT,O_VERTICAL_TOP,O_HORIZONTAL_RIGHT,O_VERTICAL_BOTTOM};

enum SMD_PACKAGE_TYPE {TYPE_0805,TYPE_1206,TYPE_1210,TYPE_2510,TYPE_2512};

enum PRINT_TRANSFORMATIONS {TR_ROTATE = 0,TR_MIRROW = 1};

enum class LINE_STYLE {LINE_ROUNDED,LINE_SQUARED};

enum class CONNECTOR_TYPE {BOARD,SCHEMATIC,CONNECTED_RULER,SIMPLE_RULER,NONE};

enum AUTO_SORT {TOP_LEFT = 1,BOTTOM_RIGHT,TOP_RIGHT,BOTTOM_LEFT,SORT_NONE};

static AUTO_SORT getNextAutoSort(AUTO_SORT i) noexcept
{
   switch(i)
   {
      case TOP_LEFT:return BOTTOM_RIGHT;
      case BOTTOM_RIGHT: return TOP_RIGHT;
      case TOP_RIGHT:return BOTTOM_LEFT;
      default: return SORT_NONE;
   }
}


using ITEM_ID = int;

static constexpr ITEM_ID ID_NONE=-1;

struct SquareGeom
{
   float width;
   float height;
};

struct RoundPlateGeom
{
   float d_in;
   float d_ex;
};

struct RoundShapeGeom
{
   float r_x;
   float r_y;
   int a_st;
   int a_sp;
   int closed;
};

struct LineGeom
{
   float width;
   LINE_STYLE style;
};

struct CapGeom
{
   float width;
};

struct TextGeom
{
   int fntSize;
};


union GeomCommonProps
{
   SquareGeom sqw_g;
   RoundPlateGeom rpl_g;
   RoundShapeGeom rsh_g;
   LineGeom line_g;
   CapGeom cap_g;
   TextGeom txt_g;

   GeomCommonProps(const GeomCommonProps& props)
   {
      memcpy(this,&props,sizeof(GeomCommonProps));
   }
   GeomCommonProps(float h,float w)
   {
      sqw_g.width = w;
      sqw_g.height= h;
   }
   GeomCommonProps(float d_ex,float d_in,bool)
   {
      rpl_g.d_ex = d_ex;
      rpl_g.d_in = d_in;
   }
   GeomCommonProps(float d_ex,int ast,int asp,bool c)
   {
      rsh_g.r_x = d_ex;
      rsh_g.r_y = d_ex;
      rsh_g.a_st = ast;
      rsh_g.a_sp = asp;
      rsh_g.closed = c;
   }
   GeomCommonProps(float w,bool bLine)
   {
       if(bLine)
          line_g.width = w;
       else
           cap_g.width = w;
   }
   GeomCommonProps(float w,LINE_STYLE style)
   {
       line_g.width = w;
       line_g.style = style;
   }
   GeomCommonProps(int fntSize)
   {
      txt_g.fntSize = fntSize;
   }

};




static GeomCommonProps* makeRectGeom(float width,float height)
{
   return new GeomCommonProps(height,width);
}

static GeomCommonProps* makeRoundPlateGeom(float d_ex,float d_in)
{
   return new GeomCommonProps(d_ex,d_in,true);
}

static GeomCommonProps* makeRoundShapeGeom(float d_ex,int ast,int asp,bool c)
{
   return new GeomCommonProps(d_ex,ast,asp,c);
}

static GeomCommonProps* makeLineGeom(float d_ex,LINE_STYLE s)
{
   return new GeomCommonProps(d_ex,s);
}

static GeomCommonProps* makeCapGeom(float d_ex)
{
   return new GeomCommonProps(d_ex,false);
}

static GeomCommonProps* makeTextGeom(int fntSize)
{
   return new GeomCommonProps(fntSize);
}


#define CURSOR_TEXT "Select"
#define CURSOR_TEXT_TIP "Select individual objects"
#define CONNECTOR_TEXT "Connector"
#define CONNECTOR_TEXT_TIP "PCB connector"
#define EL_CONNECTOR_TEXT "Schematic connection"
#define EL_CONNECTOR_TEXT_TIP "Connect schematic objects"
#define ZOOM_IN_TEXT "Zoom in"
#define ZOOM_IN_TEXT_TIP "Zoom in"

#define ZOOM_OUT_TEXT "Zoom out"
#define ZOOM_OUT_TEXT_TIP "Zoom out"

#define PLATE_TEXT "Round plate"
#define PLATE_RECT_TEXT "Rect plate"
#define ACTIVE_LEVELS_TEXT  "Active  levels :"
#define VISIBLE_LEVELS_TEXT "Visible levels :"
#define LINE_WIDTH 3
#define PIXELS_PER_MM 10
//#define MIN_GRAN 1/PIXELS_PER_MM
#define CHIP_PLATE_D_EX 2.0f
#define CHIP_PLATE_D_IN 0.7f
#define SO_PLATE_HEIGHT 0.7f
#define TSSOP_PLATE_HEIGHT 0.35f
#define CONNECTOR_DEF_WIDTH 0.7
#define DEFAULT_RECT_HEIGHT 2.2
#define DEFAULT_RECT_WIDTH 2.2
#define PACK_LINE_DEF_W 2
#define PACKAGE_DEF "PACKAGE"
#define CAP_PLATE_D_EX 1.8f
#define CAP_PLATE_D_IN 0.8f
#define LABEL_ADDTOLIB_DLG "Provide the path where the component should be added in the form PARENT\\CHILD\\CHILD_OF_CHILD"
#define CHKBX_ADDTOLIB_DLG "Create the path when it does not exist"
#define LABEL_ADDTOLIB_DLG_NAME "Name of the new component"
#define NAME_IS_ALREADY_EXISTS_MSG "The item with the name selected is already present! Update this item?"
#define ADDTOLIB_DLG_FIELDS_REQUIRED_EMPTY_MSG "The name and path fields cannot be empty"
#define FAILED_HIERARCHICAL_GROUPING "Selection contains already grouped items.Ungroup\n all grouped items first and try grouping again"
#define SAVE_SCHEMA "The current schema is not saved.Save it?"
#define SAVE_SCHEMA_TITLE "Saving schema"
#define NEW_SCHEM_DEF_WIDTH  80
#define NEW_SCHEM_DEF_HEIGHT 60
#define MULPLATE_D_EX 1.0f
#define MULPLATE_D_IN 0.5f







//definitions for parsing files
#define X_DEF "x"
#define Y_DEF "y"
#define D_EX_DEF "d_ex"
#define D_IN_DEF "d_in"
#define ANGLE_ST_DEF "angle_start"
#define ANGLE_SP_DEF "angle_span"
#define CHORD "chord"
#define LEVEL_DEF "level"
#define NAME_DEF "name"
#define ID_DEF "id"
#define WIDTH_DEF "width"
#define HEIGHT_DEF "height"
#define STYLE_DEF "style"
#define POINT_DEF "POINT"
#define PLATE_DEF "PLATE"
#define PLATE_RND_DEF "PLATE_RND"
#define PLATE_RECT_DEF "PLATE_RECT"
//electric connector plate
#define PLATE_CNT "PLATE_CNT"
#define CONNECTOR_DEF "CONNECTOR"
#define CONTAINER_DEF "CONTAINER"
#define PARENT_DEF "parent"
#define CONNECTOR_TYPE_DEF "cnt"
#define EL_OR_BOARD_DEF "cnt_type"
#define TYPE_DEF "type"
#define PACKAGE_DEF "PACKAGE"
#define TEXT_DEF "TEXT"
#define PACKAGE_RECT_DEF "PACKAGE_RECT"
#define PACKAGE_RND_DEF "PACKAGE_RND"
#define RESISTOR_DEF "RESISTOR"
#define EL_CAPACITOR_DEF "EL_CAPACITOR"
#define SMD_PASSIVE_DEF "SMD_PASSIVE"
#define CAPACITOR_DEF "CAPACITOR"
#define CAP_SCEMATIC_DEF "CAP_SCHEMA"
#define DIP_CHIP_DEF "DIP_CHIP"
#define SO_CHIP_DEF "SO_CHIP"
#define MSSOP_CHIP_DEF "MSSOP_CHIP"
#define TSSOP_CHIP_DEF "TSSOP_CHIP"
#define N_DEF       "n"
#define ORIENTATION_DEF "orientation"
#define ORIENTATION_SHORT_DEF "o"
#define FILLED_SHORT_DEF "f"
#define ZOOM_DEF "zoom"
#define ZOOM_D_DEF "zoom_d"
#define ITEMS_DEF "ITEMS"
#define BD_STRT_EL "BD_SCHEMATIC"
#define DIST_DEF "dist"
#define SEL_LAYERS "Select layers"
#define SEL_TR "Transformations"
#define MIRROW_HOR_STRING "Mirrow vertically"
#define ROTATE_STRING "Rotate clockwise 90 degrees"
#define NUM_OF_COPIES "Number of copies"
#define PRINT_DEF "Print"
#define PRINTER_PROPS_DEF "Printer properties"
#define LOAD_TEMPLATE "Load image"
#define LOAD_TEMPLATE_TIP "Load image as a template in background"
#define UNLOAD_TEMPLATE "Unload image"
#define UNLOAD_TEMPLATE_TIP "Unload image as a template in background"


//type defs
#define TYPE_S_R_DIP    "r_dip"
#define TYPE_S_C        "c"
#define TYPE_S_EL_C     "c_el"
#define TYPE_S_0805     "0805"
#define TYPE_S_1206     "1206"
#define TYPE_S_1210     "1210"
#define TYPE_S_2510     "2510"
#define TYPE_S_2512     "2512"
#define TYPE_S_CHIP_DIP "DIP"
#define TYPE_S_CHIP_SO  "SO"
#define TYPE_S_CHIP_MSOP  "MSSOP"
#define TYPE_S_CHIP_TSOP  "TSSOP"
#define PLATE_ROUNd_TYPE_DEF "plt_rnd"
#define GENERIC_TYPE_DEF "generic"
#define RECT_PKG_TYPE_DEF "rect_pkg"
#define RND_PKG_TYPE_DEF "rnd_pkg"
#define PKG_TYPE_DEF "gen_pkg"
#define RECT_TYPE_DEF "rect"
#define CAP_TYPE_DEF "cap"
#define CAP_SCHEM_TYPE_DEF "cap_schema"
#define TEXT_TYPE_DEF "txt_fld"
#define TXT_CONT_DEF "txt_cont"
#define TXT_FNT_SZ_DEF "fnt_sz"


//MENU definitions
#define ADD_NODE_MENU_STR    "&Add node"
#define DELETE_NODE_MENU_STR "&Delete node"
#define GROUP_MENU_STR "&Group"
#define UNGROUP_MENU_STR "&Ungroup"
#define ADDTOLIB_MENU_STR "Add to &library"
#define OPEN_MENU_STR "&Open..."
#define NEW_MENU_STR "&New"
#define SAVE_MENU_STR "&Save"
#define SAVE_AS_MENU_STR "Sa&ve as"
#define PRINT_MENU_STR "&Print"
#define RULER_MV_STR "Ruler mv"
#define EXIT_MENU_STR "E&xit"
#define NEW_SCHEM_NAME_DEF "unnamed.bd_xml"
#define ADD_NODE_TIP_STR    "Create a new node"
#define DELETE_NODE_TIP_STR "Delete a node"
#define GROUP_TIP_STR "Group items together"
#define UNGROUP_TIP_STR "Ungroup items"
#define OPEN_TIP_STR "Open an existing file"
#define SAVE_TIP_STR "Save the document to disk"
#define PRINT_TIP_STR "Print the document"
#define EXIT_TIP_STR "Exit the application"
#define ADDTOLIB_TIP_STR "Adds to library selected component"
#define PROP_DEF_STR "Properties"
#define PROP_DEF_TIP_STR "Properties of item"
#define ZOOM_IN_STR "Zoom+"
#define ZOOM_OUT_STR "Zoom-"
#define RULER_STR "Ruler"
#define DISTANCE_STR "Distance"
#define COPY_DEF_TIP_STR "Copy selected items into clipboard"
#define CUT_DEF_TIP_STR "Cut selected items into clipboard"
#define PASTE_DEF_TIP_STR "Paste items from clipboard"
#define SELALL_DEF_TIP_STR "Select all items"
#define UPDATE_GEOM_PRP_STR "Update geometry and properties"

constexpr int iNoZoom = 1;
constexpr int iPreviewSizeX = 300;
constexpr int iPreviewSizeY = 264;
constexpr int lItemNameLength = 64; // the length with zero at the end
//items divide into different types: SO,DIP,PLATE_RND etc
constexpr int lItemTypeLength = 64; // the length with zero at the end
constexpr float fMaxBoardWidth = 150.00;
constexpr float fMaxBoardHeight = 150.00;
constexpr float fDefVirtConWidth = 0.1f;
constexpr float fDefVirtConPlateD = 0.99f;

constexpr int iA4WidthMM = 210;
constexpr int iA4HeightMM = 297;
constexpr int PIXELS_PER_MM_PRTPREVIW = 2;
constexpr int iPrintPreviewWidth = iA4WidthMM * PIXELS_PER_MM_PRTPREVIW;
constexpr int iPrintPreviewHeight = iA4HeightMM * PIXELS_PER_MM_PRTPREVIW;
constexpr int iRulerTextW = 200;
constexpr int iRulerTextH = 14;
constexpr char szItemTemplate[] = "item";
const QColor bckgColor(80,80,80);
auto constexpr VC_CON_COLOR = Qt::cyan;
const QColor virtConColor{VC_CON_COLOR};
const QColor rulerColor(255,155,255);
const QColor selectColor(35,127,190);
constexpr float DIP_STEP = 2.54f;
constexpr float fSizeOfMatrixCell = 0.5f;
constexpr float fWidthOfConAuto = fSizeOfMatrixCell ;
constexpr float minGraularity = 1.0f/PIXELS_PER_MM;

constexpr float pklnwf= static_cast<float>(PACK_LINE_DEF_W)/PIXELS_PER_MM;
constexpr int DEFAULT_TEXT_ITEM_FONT_SIZE = 12;
constexpr int intDefNumForGerber = 3;
constexpr int intMaxNumForGerber = 4;
constexpr int intMinNumForGerber = 2;

constexpr int decDefNumForGerber = 4;
constexpr int decMaxNumForGerber = 5;
constexpr int decMinNumForGerber = 3;


constexpr int orientation_angles[] = {0,90,180,270};
constexpr int fontSizes[] = {4,5,6,8,10,12,14,16,18,20,22,24,26};

enum class ZoomValue {NO_ZOOM = 1,ZOOM_2 = 2,ZOOM_4 = 4,ZOOM_8 = 8,ZOOM_16=16};

class GraphicalItem;


struct  SchemData
{
   SchemData():m_width(NEW_SCHEM_DEF_WIDTH * PIXELS_PER_MM),m_height(NEW_SCHEM_DEF_HEIGHT * PIXELS_PER_MM){}
   SchemData(int w,int h):m_width(w),m_height(h){}
   bool setStored(bool s){stored = s;}
   bool isStored(){return stored;}
   bool stored{false}; //is the shema associated was saved into file;
   int m_width;
   int m_height;
};



struct PointF
{
    float m_x{0.0f};
    float m_y{0.0f};
    PointF(){}
    PointF(float xx,float yy):m_x(xx),m_y(yy){}
    PointF(const PointF& pt) = default;
    PointF& operator=(const PointF& pt) = default;
    float x(){return m_x;}
    float y(){return m_y;}
    void setX(float x){m_x = x;}
    void setY(float y){m_y = y;}
    const PointF& operator/=(int n)
    {
       m_x /= n;
       m_y /= n;
       return *this;
    }
    const PointF& operator+=(PointF val)
    {
       m_x += val.x();
       m_y += val.y();
       return *this;
    }
    operator QPoint(){return QPoint(static_cast<int>(m_x),static_cast<int>(m_y));}
};


class IDsGenerator
{
    atomic_int m_lastID{0};
public:
   IDsGenerator()
   {
      m_lastID.store(0);             ;
   }
   //the buffer should be more than 64 char length
   ITEM_ID getNewID()
   {
      int n = ++m_lastID;
      return n;
   }
   void getNewID(ITEM_ID& id,char *pBuf)
   {
      if(!pBuf)
         return;
      id = ++m_lastID;
      sprintf(pBuf,"%s%d",szItemTemplate,id);
   }
   void adjustCounter(int n)
   {
      //non thread safe
      if (n > m_lastID)
         m_lastID.store(n);
   }
   static void idToString(ITEM_ID id,char *pBuf)
   {
      if(!pBuf)
         return;
      sprintf(pBuf,"%s%d",szItemTemplate,id );
   }
   void init()
   {
      m_lastID = 0;
   }
   static IDsGenerator* instance()
   {
      static shared_ptr<IDsGenerator> IDsGen_instance = make_shared<IDsGenerator>();
      return IDsGen_instance.get();
   }
};



static QPoint getCellCoordForPoint(PointF& pt,float step)
{
   QPoint r{0,0};
   int x = static_cast<int>(pt.x() * 1000);
   int y = static_cast<int>(pt.y() * 1000);
   int s = static_cast<int>(step * 1000);
   r.setX(x/s);
   r.setY(y/s);
   return r;
}



struct VcConCunstrInfo
{
   VcConCunstrInfo(ITEM_ID id,PointF& pt,bool connected):
               m_id(id),m_pt(pt),m_isConnected(connected)
   {
   }
   VcConCunstrInfo():m_id(ID_NONE),m_pt(-1,-1),m_isConnected(false)
   {
   }
   bool isConnected(){return m_id != ID_NONE;}
   void clear()
   {
      m_id = ID_NONE;
      m_pt.setX(-1),m_pt.setY(-1);
      m_isConnected = false;
   }
   ITEM_ID m_id;
   PointF m_pt;
   bool m_isConnected;
};

struct BoardLevel
{
   BOARD_LEVEL_ID idLevel;
   Qt::GlobalColor color;
   char desc[16];
   char colorDesc[24];
   QObject *pHandler;
   vector<GraphicalItem*> levelItems;
   BoardLevel():pHandler(nullptr){}
};


static constexpr float roundFloat(float val)
{
   return round(val * 100)/100;
}

static constexpr int castFloatToInt(float val)
{
   return static_cast<int>(round(val * 1000)/1000);
}

static constexpr int castFloatToIntPxls(float val)
{
   return static_cast<int>(round(val * PIXELS_PER_MM * 1000)/1000);
}

static constexpr int castFloatToIntPxls(float val,int zoom) noexcept
{
   return static_cast<int>(val * PIXELS_PER_MM * zoom);
}

static void rotateRoundShape(PointF& pt,float& x1,float& y1,int angle)
{
   if (angle == 0)
       return;
   //recalcuting coordinates taking pt as a center
   if(qFuzzyCompare(pt.x(),x1) && qFuzzyCompare(pt.y(),y1))
      return;
   //rotate for 90 degrees.In the future will be rotation for 45 degrees
   float anglef = angle > 0 ? PI/2 : -PI/2;
   auto x11 = x1 - pt.x();
   auto y11 = y1 - pt.y();
   auto x_new = x11 * cosf(anglef) - y11 * sinf(anglef);
   auto y_new = x11 * sinf(anglef) + y11 * cosf(anglef);
   x1 = pt.x() + x_new;
   y1 = pt.y() + y_new;
}


//pt - is the center around which (x,y) should be rotated
static void rotateRoundShape(QPoint& pt,int& x1,int& y1,int angle)
{
   if (angle != 90 * 16) //at the moment only 90 degrees supported
      return;
   if(pt.x() == x1 && pt.y() == y1) //round shape around its center - nothing to do
      return;
   
   int dx = x1 - pt.x();
   int dy = y1 - pt.y();
   if(dx <= 0 && dy < 0)
   {
      x1 = pt.x() + std::abs(dy);
      y1 = pt.y() - std::abs(dx);
   }
   else if(dx > 0 && dy <= 0)
   {
      x1 = pt.x() + std::abs(dy);
      y1 = pt.y() + std::abs(dx);
   }
   else if(dx >= 0 && dy > 0)
   {
      x1 = pt.x() - std::abs(dy);
      y1 = pt.y() + std::abs(dx);      
   }
   else if(dx < 0 && dy >= 0)
   {
      x1 = pt.x() - std::abs(dy);
      y1 = pt.y() - std::abs(dx);      
   }         
};

static void rotateRectShape(PointF& pt,float& x1,float& y1,int angle,ITEMS_ORIENTATION& t)
{
    rotateRoundShape(pt,x1,y1,angle);

    switch(t)
    {
       case O_VERTICAL_TOP:
          t = O_HORIZONTAL_RIGHT;
          break;
       case O_HORIZONTAL_RIGHT:
          t = O_VERTICAL_BOTTOM;
          break;
       case O_VERTICAL_BOTTOM:
          t = O_HORIZONTAL_LEFT;
          break;
       case O_HORIZONTAL_LEFT:
          t = O_VERTICAL_TOP;
          break;
       default:
          break;
    }   
}

static void rotateRectShape(QPoint& pt,int& x1,int& y1,int angle,ITEMS_ORIENTATION& t)
{
   if (angle != 90 * 16) //at the moment only 90 degrees supported
      return;

   if(pt.x() != x1 || pt.y() != y1)
      rotateRoundShape(pt,x1,y1,angle);      

   switch(t)
   {
      case O_VERTICAL_TOP:
         t = O_HORIZONTAL_RIGHT;
         break;
      case O_HORIZONTAL_RIGHT:
         t = O_VERTICAL_BOTTOM;
         break;         
      case O_VERTICAL_BOTTOM:
         t = O_HORIZONTAL_LEFT;
         break;                  
      case O_HORIZONTAL_LEFT:
         t = O_VERTICAL_TOP;
         break;                           
      default:
         break;            
   }      
};







class LevelsWrapper
{

public:
   LevelsWrapper()
   {
/*      
      levels[0].idLevel =  LEVEL_A1;
      levels[0].color = Qt::red;
      strcpy(levels[0].desc,"A1");
      strcpy(levels[0].colorDesc,"red");
      
      levels[1].idLevel =  LEVEL_A;
      levels[1].color = Qt::blue;
      strcpy(levels[1].desc,"A");
      strcpy(levels[1].colorDesc,"blue");
      
      levels[2].idLevel =  LEVEL_B;
      levels[2].color = Qt::darkRed;
      strcpy(levels[2].desc,"B");
      strcpy(levels[2].colorDesc,"darkRed");      
      
      levels[3].idLevel =  LEVEL_C;
      levels[3].color = Qt::darkCyan;   for (int i = 0; i < m_points.size(); ++i)
   {
      m_points[i]); 
   }

      strcpy(levels[3].desc,"C");
      strcpy(levels[3].colorDesc,"darkCyan");http://
      
      levels[4].idLevel =  LEVEL_D;
      levels[4].color = Qt::darkMagenta;
      strcpy(levels[4].desc,"D");
      strcpy(levels[4].colorDesc,"darkMagenta");
      
      
      levels[5].idLevel =  LEVEL_E;
      levels[5].color = Qt::darkYellow;
      strcpy(levels[5].desc,"E");
      strcpy(levels[5].colorDesc,"darkYellow");
      
      levels[6].idLevel =  LEVEL_F;
      levels[6].color = Qt::green;
      strcpy(levels[6].desc,"F");
      strcpy(levels[6].colorDesc,"green");
      
      levels[7].idLevel =  LEVEL_F1;http://
      levels[7].color = Qt::yellow;
      strcpy(levels[7].desc,"F1");
      strcpy(levels[7].colorDesc,"yellow");
*/      
   }

   void getAvalableLevels(BoardLevel **pLevels,short& s)
   {
      short size = 7;
      short i = 0;
      BoardLevel *p = new BoardLevel[size];

      p[i].idLevel =  LEVEL_VC;
      p[i].color = Qt::cyan;
      strcpy(p[i].desc,"VC_COM");
      strcpy(p[i].colorDesc,"cyan");

      ++i;

      p[i].idLevel =  LEVEL_A1;
      p[i].color = Qt::red;
      strcpy(p[i].desc,"A1");
      strcpy(p[i].colorDesc,"red");      
      
      ++i;
      
      p[i].idLevel =  LEVEL_A;
      p[i].color = Qt::blue;
      strcpy(p[i].desc,"A");
      strcpy(p[i].colorDesc,"blue");
      
      ++i;
      
      p[i].idLevel =  LEVEL_B;
      p[i].color = Qt::darkRed;
      strcpy(p[i].desc,"B");
      strcpy(p[i].colorDesc,"darkRed");      

      ++i;
      
      p[i].idLevel =  LEVEL_C;
      p[i].color = Qt::darkCyan;
      strcpy(p[i].desc,"C");
      strcpy(p[i].colorDesc,"darkCyan");      

      ++i;
            
      p[i].idLevel =  LEVEL_F;
      p[i].color = Qt::green;
      strcpy(p[i].desc,"F");
      strcpy(p[i].colorDesc,"green");
      
      ++i;
      
      p[i].idLevel =  LEVEL_F1;
      p[i].color = Qt::yellow;
      strcpy(p[i].desc,"F1");
      strcpy(p[i].colorDesc,"yellow");


      
      *pLevels = p;
      s = size;
      
   }
   static void getDrawableLevels(vector<BoardLevel>& vIn)
   {
       vIn.reserve(6);
       BoardLevel lev;
       memset(&lev,0,sizeof(BoardLevel));


       lev.idLevel =  LEVEL_A1;
       lev.color = Qt::red;
       strcpy(lev.desc,"A1");
       strcpy(lev.colorDesc,"red");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_A;
       lev.color = Qt::blue;
       strcpy(lev.desc,"A");
       strcpy(lev.colorDesc,"blue");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_B;
       lev.color = Qt::darkRed;
       strcpy(lev.desc,"B");
       strcpy(lev.colorDesc,"darkRed");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_C;
       lev.color = Qt::darkCyan;
       strcpy(lev.desc,"C");
       strcpy(lev.colorDesc,"darkCyan");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_F;
       lev.color = Qt::green;
       strcpy(lev.desc,"F");
       strcpy(lev.colorDesc,"green");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_F1;
       lev.color = Qt::yellow;
       strcpy(lev.desc,"F1");
       strcpy(lev.colorDesc,"yellow");
       vIn.push_back(lev);

   }
   static void getAvalableLevels(vector<BoardLevel>& vIn)
   {
      vIn.reserve(7);
      BoardLevel lev;
      memset(&lev,0,sizeof(BoardLevel));

      lev.idLevel =  LEVEL_VC;
      lev.color = VC_CON_COLOR;
      strcpy(lev.desc,"VC_COM");
      strcpy(lev.colorDesc,"vc color");
      vIn.push_back(lev);
      
      getDrawableLevels(vIn);

      
   }
   static void getTextLevels(vector<BoardLevel>& vIn)
   {
       BoardLevel lev;
       memset(&lev,0,sizeof(BoardLevel));

       lev.idLevel =  LEVEL_VC;
       lev.color = VC_CON_COLOR;
       strcpy(lev.desc,"VC_COM");
       strcpy(lev.colorDesc,"vc color");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_A;
       lev.color = Qt::blue;
       strcpy(lev.desc,"A");
       strcpy(lev.colorDesc,"blue");
       vIn.push_back(lev);

       lev.idLevel =  LEVEL_F;
       lev.color = Qt::green;
       strcpy(lev.desc,"F");
       strcpy(lev.colorDesc,"green");
       vIn.push_back(lev);

   }
   static void getActiveLevels(vector<BoardLevel>& vIn)
   {
      vIn.reserve(4);
      BoardLevel lev;
      memset(&lev,0,sizeof(BoardLevel));
      
      
      lev.idLevel =  LEVEL_A;
      lev.color = Qt::blue;
      strcpy(lev.desc,"A");
      strcpy(lev.colorDesc,"blue");
      vIn.push_back(lev);                  
      
      lev.idLevel =  LEVEL_B;
      lev.color = Qt::darkRed;
      strcpy(lev.desc,"B");
      strcpy(lev.colorDesc,"darkRed");
      vIn.push_back(lev);                  
      
      lev.idLevel =  LEVEL_C;
      lev.color = Qt::darkCyan;
      strcpy(lev.desc,"C");
      strcpy(lev.colorDesc,"darkCyan");
      vIn.push_back(lev);                        
           
      lev.idLevel =  LEVEL_F;
      lev.color = Qt::green;
      strcpy(lev.desc,"F");
      strcpy(lev.colorDesc,"green");
      vIn.push_back(lev);                        
   }
   static void getBaseContainerLayers(vector<BoardLevel>& vIn)
   {
      BoardLevel lev;
      lev.idLevel =  LEVEL_A;
      lev.color = Qt::blue;
      strcpy(lev.desc,"A");
      strcpy(lev.colorDesc,"blue");
      vIn.push_back(lev);


      lev.idLevel =  LEVEL_F;
      lev.color = Qt::green;
      strcpy(lev.desc,"F");
      strcpy(lev.colorDesc,"green");
      vIn.push_back(lev);

   }
   void getActiveLevels(BoardLevel **pLevels,short& s)
   {
      short size = 4;
      BoardLevel *p = new BoardLevel[size];
      
      p[0].idLevel =  LEVEL_A;
      p[0].color = Qt::blue;
      strcpy(p[0].desc,"A");
      strcpy(p[0].colorDesc,"blue");
      
      p[1].idLevel =  LEVEL_B;
      p[1].color = Qt::darkRed;
      strcpy(p[1].desc,"B");
      strcpy(p[1].colorDesc,"darkRed");            
      
      p[2].idLevel =  LEVEL_C;
      p[2].color = Qt::darkCyan;
      strcpy(p[2].desc,"C");
      strcpy(p[2].colorDesc,"darkCyan");            
      
      
      p[3].idLevel =  LEVEL_F;
      p[3].color = Qt::green;
      strcpy(p[3].desc,"F");
      strcpy(p[3].colorDesc,"green");
      
      
      *pLevels = p;
      s = size;
   }
   static constexpr int getActiveLevelsSize(){return 4;}
   static  Qt::GlobalColor getColorForLevel(BOARD_LEVEL_ID level)
   {
      switch(level)
      {
         case LEVEL_A1:return Qt::red;
         case LEVEL_A: return Qt::blue;
         case LEVEL_B: return Qt::darkRed;
         case LEVEL_C: return Qt::darkCyan;
         case LEVEL_D: return Qt::darkMagenta;
         case LEVEL_E: return Qt::darkYellow;
         case LEVEL_F: return Qt::green;           
         case LEVEL_F1:return Qt::yellow;
         case LEVEL_VC:return VC_CON_COLOR;
         default: return Qt::transparent;
      }
   }
   static constexpr Qt::GlobalColor getColorForSelection()
   {
      return Qt::magenta;
   }
   static constexpr Qt::GlobalColor getColorForMove()
   {
      return Qt::gray;
   }   
   static BOARD_LEVEL_ID geLevelForDip(BOARD_LEVEL_ID level)
   {
      if(level == LEVEL_A)
         return LEVEL_F1;
      else if(level == LEVEL_F)
         return LEVEL_A1;
      else
         return LEVEL_NONE;
   }
   static BOARD_LEVEL_ID geLevelForSO(BOARD_LEVEL_ID level)
   {
      if(level == LEVEL_A)
         return LEVEL_A1;
      else if(level == LEVEL_F)
         return LEVEL_F1;
      else
         return LEVEL_NONE;
   }
   static BOARD_LEVEL_ID geLevelForTSOP(BOARD_LEVEL_ID level)
   {
      return geLevelForSO(level);
   }
   static bool isActiveLevelExternal(BOARD_LEVEL_ID level)
   {
      return level == BOARD_LEVEL_ID::LEVEL_A || level == BOARD_LEVEL_ID::LEVEL_F;
   }
   static BoardLevel getVCLevel()
   {
      BoardLevel lev;
      memset(&lev,0,sizeof(BoardLevel));

      lev.idLevel =  LEVEL_VC;
      lev.color = VC_CON_COLOR;
      strcpy(lev.desc,"VC_COM");
      strcpy(lev.colorDesc,"vc color");
      return lev;
   }
   static bool isPackageLevel(BOARD_LEVEL_ID level)
   {
      return level == BOARD_LEVEL_ID::LEVEL_A1 ||
              level == BOARD_LEVEL_ID::LEVEL_F1;
   }
   static BOARD_LEVEL_ID getConnectorLevelForPackage(BOARD_LEVEL_ID level)
   {
      if(level == LEVEL_A1)
         return LEVEL_A;
      else if(level == LEVEL_F1)
         return LEVEL_F;
      else
         return LEVEL_NONE;
   }
   static BOARD_LEVEL_ID getOppositePackageLevel(BOARD_LEVEL_ID level)
   {
      switch(level)
      {
         case LEVEL_A1: return LEVEL_F1;
         case LEVEL_F1: return LEVEL_A1;
         default: return LEVEL_NONE;
      }
   }
   static BOARD_LEVEL_ID getOppositeLevel(BOARD_LEVEL_ID level)
   {
      switch(level)
      {
         case LEVEL_A: return LEVEL_F;
         case LEVEL_F: return LEVEL_A;
         default: return LEVEL_NONE;
      }
   }
};


static void getMaxMinCoord(const vector<PointF>& pts,float& maxX,float& maxY,
                                                     float& minX,float& minY)
{
   if(pts.empty())
      return;

   maxX = pts.at(0).m_x;
   maxY = pts.at(0).m_y;
   minX = pts.at(0).m_x;
   minY = pts.at(0).m_y;
   for(const auto& pt:pts)
   {
      if(pt.m_x > maxX)
         maxX = pt.m_x;
      if(pt.m_y > maxY)
         maxY = pt.m_y;
      if(pt.m_x < minX)
         minX = pt.m_x;
      if(pt.m_y < minY)
         minY = pt.m_y;
   }
}

struct TransInfo
{
   PRINT_TRANSFORMATIONS idTransform;
   char desc[64];
};

static void getAvailableTransformations(vector<TransInfo>& v)
{
   TransInfo info;
   info.idTransform = TR_ROTATE;
   strncpy(info.desc,ROTATE_STRING,sizeof(info.desc));
   v.push_back(info);
   info.idTransform = TR_MIRROW;
   strncpy(info.desc,MIRROW_HOR_STRING,sizeof(info.desc));
   v.push_back(info);
}



static float getWidthForDipPack(int n,int k = 1)
{
   switch(n)
   {
      case 4:  return 3.6f * k;
      case 8:  return 7.62f * k;
      case 14: return 7.62f * k;
      case 16: return 7.62f * k;
      case 18: return 7.62f * k;
      case 20: return 7.62f * k;
      case 22: return 10.16f * k;
      case 24: return 15.24f * k;
      case 28: return 15.24f * k;
      case 32: return 15.24f * k;
      case 36: return 15.24f * k;
      case 40: return 15.24f * k;
      case 42: return 15.24f * k;
      case 48: return 15.24f * k;
      case 52: return 15.24f * k;
      case 64: return 19.05f * k;
      default: return 7.62f * k;
   }
}

static float getFullWidthForDipPack(int n,int k = 1)
{
   return getWidthForDipPack(n,k) - CHIP_PLATE_D_EX * k - pklnwf * 2 * k ;
}

static constexpr float getInternalDForDipPlate()
{
  return CHIP_PLATE_D_IN;
}

static constexpr float getExternalDForDipPlate()
{
  return CHIP_PLATE_D_EX;
}

static constexpr float getHeightForDipPack(int n,int k = 1)
{
   return n/2 * 2.54f * k + PACK_LINE_DEF_W * 2 * k;
}



static float getFforElCapacitor(int d)
{
   if (d == 5)
      return 2;
   else if (d == 6)
      return 2.5;
   else if (d == 8)
      return 3.5;
   else if (d == 10 || d == 12)
      return 5.0;
   else 
      return 7.5;
      
}



static int getLForELCapFromStrType(const QString& str)
{
   QString d_str = str.mid(4);
   int d = 16; // large
   if(d_str[0].isDigit())
      d = str.mid(4).toInt();
  
   return d;
}


static int getLForCapFromStrType(const QString& str)
{
   QString d_str = str.mid(1);
   int d = 16; // large
   if(d_str[0].isDigit())
      d = str.mid(1).toInt();
  
   return d;
}


static float getDforResistor(int l)
{
   if (l == 3)
      return 1.5f;
   else if (l >= 6 && l < 9)
      return 2.3f;
   else if (l == 9)
      return 3.2f;
   else if (l == 11)
      return 4.5f;
   else if (l == 15)
      return 5.0f;
   else 
      return 5.5f;
      
}



static float getWidthForSOPack(int n)
{
   if(n <= 16)
      return 3.81f;
   else
      return 7.62f;
}

static float getHeightForSOPack(int n)
{
   return DIP_STEP/2 * n/2;
}

static float getWidthForTSOPPack(int n)
{
   if(n <= 10)
      return 3.0f;
   else
      return 4.4f;
}

static float getHeightForTSOPPack(int n)
{
   switch(n)
   {
      case 8: return 3.0f;
      case 10:return 3.5f;
      case 16:return 5.0f;
      case 20:return 6.5f;
      case 24:return 7.8f;
   }
   return 3.0f;
}


static PointF getSizeOfSmfPack(SMD_PACKAGE_TYPE type)
{
   switch(type)
   {
      case TYPE_0805:return PointF(2.1f,1.25f);
      case TYPE_1206:return PointF(3.2f,1.6f);
      case TYPE_1210:return PointF(3.2f,2.5f);
      case TYPE_2510:return PointF(5.0f,2.5f);
      case TYPE_2512:return PointF(6.3f,3.2f);
   }
}

static SMD_PACKAGE_TYPE getSmdPackForStr(const QString& str)
{
   if(str.compare(TYPE_S_0805) == 0)
      return TYPE_0805;
   else if(str.compare(TYPE_S_1206) == 0)
      return TYPE_1206;
   else if(str.compare(TYPE_S_1210) == 0)
      return TYPE_1210;
   else if(str.compare(TYPE_S_2510) == 0)
      return TYPE_2510;
   else if(str.compare(TYPE_S_2512) == 0)
      return TYPE_2512;
   else
      return TYPE_1206;
}

static int getLResistorFromStrType(const QString& str)
{
   QString d_str = str.mid(5);
   int d = 15; // large
   if(d_str[0].isDigit())
      d = str.mid(5).toInt();
  
   return d;
}



static ITEMS_ORIENTATION getOrientationStr(QString& str)
{
   if(str.compare("H_L") == 0)
      return O_HORIZONTAL_LEFT;
   else if(str.compare("H_R") == 0)
      return O_HORIZONTAL_RIGHT;
   else if(str.compare("V_T") == 0)
      return O_VERTICAL_TOP;
   else if(str.compare("V_B") == 0)
      return O_VERTICAL_BOTTOM;
   else
      return O_VERTICAL_TOP;
}

constexpr int WALL = -2;
constexpr int EMPTY = ID_NONE;

union UniCoord
{
   UniCoord(int i):pos(i){}
   UniCoord(char16_t xx,char16_t yy)
   {
       coord.x = xx;
       coord.y = yy;
   }
   struct
   {
      char16_t x;
      char16_t y;
   } coord;
   int pos;
};


using VecIndex = vector<int>::size_type;
static bool lee(vector<vector<int> >& grid,
         int W,int H, //grid dimemsions
         VecIndex ax, VecIndex ay, VecIndex bx, VecIndex by, //start and end coordinates
         int& len, //resulting length of found way
         vector<UniCoord>& v0,
         vector<UniCoord>& v1,
         set<ITEM_ID> ids //ids of connectors we need to find way from...to
                )
{
   grid[ay][ax] = 0;
   char16_t k = 1;
   v1[0]= UniCoord(static_cast<char16_t>(ax),
                   static_cast<char16_t>(ay));
   bool bFound = false;
   VecIndex j = 0;
   len = 0;
   do
   {
      auto looping = false;
      vector<UniCoord>& v_cur = k&1 ? v1:v0;
      vector<UniCoord>& v_next = k&1 ? v0:v1;
      j = 0;
      int counter = 0;
      for(VecIndex i = 0; v_cur[i].pos != -1;++i, ++counter)
      {
         char16_t xx = v_cur[i].coord.x;
         char16_t yy = v_cur[i].coord.y;
         auto l = [&grid,&j,&bFound,&ids,bx,by,k](
                   vector<UniCoord>& v,char16_t xi,char16_t yi) {
                    if(grid[yi][xi] != WALL)
                    {
                        if((grid[yi][xi] < WALL))
                           cout<<"grid="<<grid[yi][xi]<<"temp="<<WALL - grid[yi][xi]<<endl;

                       if(grid[yi][xi] == EMPTY || (grid[yi][xi] < WALL &&
                          ids.find(WALL - grid[yi][xi]) != ids.end()))
                       {
                          if(j == v.size() - 1)
                              throw logic_error("the memory for vector is exceeded");
                          grid[yi][xi] = k;
                          v[j++] = UniCoord(xi,yi);
                       }
                       if(xi == bx && yi == by)
                          bFound = true;
                    }
                 };
         if((xx - 1) >= 0)
         {
            l(k&1 ? v0:v1,xx - 1,yy);
            if(bFound)
               break;
         }
         if((xx + 1) < W)
         {
            l(k&1 ? v0:v1,xx + 1,yy);
            if(bFound)
               break;
         }
         if((yy - 1) >= 0)
         {
            l(k&1 ? v0:v1,xx,yy - 1);
            if(bFound)
               break;
         }
         if((yy + 1) < H)
         {
            l(k&1 ? v0:v1,xx,yy + 1);
            if(bFound)
               break;
         }
      }
      cout<<"counter"<<counter<<endl;
      cout<<"k="<<k<<" j="<<j<<" found="<<bFound<<" looping="<<looping<<endl;
      if(bFound)
         break;
      v_next[j] = -1;
      ++k;
   }while(j);
   if(bFound)
   {
      auto& vx = v0;
      auto& vy = v1;
      len = k;
      vx[k] = UniCoord(static_cast<int>(bx));
      vy[k] = UniCoord(static_cast<int>(by));
      --k;

      while(true)
      {
         auto xx = static_cast<VecIndex>(vx[k + 1].pos);
         auto yy = static_cast<VecIndex>(vy[k + 1].pos);
         if(vy[k + 1].pos - 1 >= 0 &&
            grid[yy - 1][xx] == k)
         {
            vx[k] = vx[k + 1].pos;
            vy[k] = vy[k + 1].pos - 1;
         }
         else if(vy[k + 1].pos + 1 < H &&
                 grid[yy + 1][xx] == k)
         {
            vx[k] = vx[k + 1].pos;
            vy[k] = vy[k + 1].pos + 1;
         }
         else if(vx[k + 1].pos - 1 >= 0 &&
                 grid[yy][xx - 1] == k)
         {
            vx[k] = vx[k + 1].pos - 1;
            vy[k] = vy[k + 1].pos;
         }
         else if(vx[k + 1].pos + 1 < W &&
                grid[yy][xx + 1] == k)
         {
            vx[k] = vx[k + 1].pos + 1;
            vy[k] = vy[k + 1].pos;
         }
         else
         {
            len = 0;
            return false;
         }
         if(k == 0)
            break;
         --k;
      }
      return true;
   }
   else
      return false;
}

/*
const int WALL = -2;

static bool lee(vector<vector<ITEM_ID> >& grid,
                int W,int H,
                int ax, int ay, int bx, int by,
                int& len,
                vector<int>& px,
                vector<int>& py,
                ITEM_ID id)   // поиск пути из ячейки (ax, ay) в ячейку (bx, by)
{
  const int BLANK = ID_NONE;
  int dx[4] = {1, 0, -1, 0};   // смещения, соответствующие соседям ячейки
  int dy[4] = {0, 1, 0, -1};   // справа, снизу, слева и сверху
  int d, x, y, k;
  bool stop;

  if (grid[ay][ax] == WALL || grid[by][bx] == WALL ) return false;  // ячейка (ax, ay) или (bx, by) - стена

  // распространение волны
  d = 0;
  grid[ay][ax] = 0;            // стартовая ячейка помечена 0
  do {
    stop = true;               // предполагаем, что все свободные клетки уже помечены
    for ( y = 0; y < H; ++y )
      for ( x = 0; x < W; ++x )
        if ( grid[y][x] == d )                         // ячейка (x, y) помечена числом d
        {
          for ( k = 0; k < 4; ++k )                    // проходим по всем непомеченным соседям
          {
             int iy=y + dy[k], ix = x + dx[k];
             if ( iy >= 0 && iy < H && ix >= 0 && ix < W &&
                  grid[iy][ix] == BLANK)
             {
                stop = false;              // найдены непомеченные клетки
                grid[iy][ix] = d + 1;      // распространяем волну
             }
          }
        }
    d++;
  } while ( !stop && (grid[by][bx] == BLANK || grid[by][bx] == id));

  if (grid[by][bx] == BLANK) return false;  // путь не найден

  // восстановление пути
  len = grid[by][bx];            // длина кратчайшего пути из (ax, ay) в (bx, by)
  x = bx;
  y = by;
  d = len;
  while ( d > 0 )
  {
    px[d] = x;
    py[d] = y;                   // записываем ячейку (x, y) в путь
    d--;
    for (k = 0; k < 4; ++k)
    {
       int iy=y + dy[k], ix = x + dx[k];
       if ( iy >= 0 && iy < H && ix >= 0 && ix < W &&
            grid[iy][ix] == d)
      {
          x = x + dx[k];
          y = y + dy[k];           // переходим в ячейку, которая на 1 ближе к старту
          break;
      }
    }
  }
  px[0] = ax;
  py[0] = ay;                    // теперь px[0..len] и py[0..len] - координаты ячеек пути
  return true;
}
*/

#endif // COMMONN_H
