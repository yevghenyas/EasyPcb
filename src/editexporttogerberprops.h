#ifndef EDITEXPORTTOGERBERPROPS_H
#define EDITEXPORTTOGERBERPROPS_H

#include <QDialog>
#include <QtWidgets/QAction>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QListWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QVBoxLayout>

#include "boardlayerswrapper.h"

class EditExportToGerberProps : public QDialog
{
   BoardLayersWrapper *pBoardLayers{nullptr};
   set<BOARD_LEVEL_ID> layersSet;
   map<int,int> chkIndexToVIndex;
   float boardWidth;
   float boardHeight;

   QDialogButtonBox *buttonBox;
   QWidget *verticalLayoutWidget;
   QVBoxLayout *verticalLayout;
   QHBoxLayout *horizontalLayout;
   QPushButton *pushButton;
   QLabel *folderLabel;
   QGridLayout *layersGrid;
   vector<QCheckBox*> mirrowedChecks;
   vector<QCheckBox*> layersBoxes;
   vector<BoardLevel> m_ActiveLevels;
   QCheckBox *checkBox;
   QCheckBox *checkBox_3;
   QLabel *label_4;
   QLabel *label_3;
   QLabel *label_2;
   QCheckBox *checkBox_2;
   QCheckBox *checkBox_4;
   QLabel *label_5;
   QLabel *label_6;
   QSpinBox *intNumSpinBox;
   QLabel *label_7;
   QSpinBox *decNumSpinBox;
   QListWidget *listWidget;

public:
   EditExportToGerberProps(BoardLayersWrapper *layers,
                           set<BOARD_LEVEL_ID>&& lSet,
                           float bW,float bH);
   void createComponents();
   void selectOutputFolder();
   void onApply(QAbstractButton *b);
   void onOk();
   void exportToGerber();
   void onCheckLayer(bool checked);
};

#endif // EDITEXPORTTOGERBERPROPS_H
