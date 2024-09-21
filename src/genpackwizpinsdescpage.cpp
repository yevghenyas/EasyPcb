#include "genpackwizpinsdescpage.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include "genpackwizard.h"
#include "packagegraphicalitem.h"
#include "QInputDialog"
#include "textgraphicalitem.h"
#include "itemsfactory.h"
#include <QGuiApplication>
#include <QScreen>

GenPackWizPinsDescPage::GenPackWizPinsDescPage(PageData *p) : pData(p)
{
   QHBoxLayout *hBox = new QHBoxLayout(this);
   QVBoxLayout *vBox = new QVBoxLayout(this);
   lWidget1 = new QListWidget();
   hBox->addWidget(lWidget1);
   but1 = new QPushButton();
   but1->setText(">>");
   but2 = new QPushButton();
   but2->setText("<<");
   vBox->addWidget(but1);
   vBox->addWidget(but2);
   lWidget2 = new QListWidget();
   hBox->addLayout(vBox);
   hBox->addWidget(lWidget2);
   setLayout(hBox);
}

void GenPackWizPinsDescPage::initializePage()
{
   if(!pData->bValidated)
   {
      lWidget1->clear();
      lWidget2->clear();
      GenericGraphicalItemsContainer *pContaniner =
           static_cast<GenericGraphicalItemsContainer*>(pData->item.get());
      auto children = pContaniner->getChildren();
      int i = 0;
      for(auto& child:*children)
      {
         if(dynamic_cast<PackageGraphicalItem*>(child.get()) == nullptr)
         {
            QString text = QString("Pin") + QString::number(i);
            lWidget1->addItem(text);
            ++i;
         }
      }
      connect(but1,SIGNAL(clicked()),this,SLOT(but1Pushed()));
      connect(but2,SIGNAL(clicked()),this,SLOT(but2Pushed()));
      pData->bValidated = false;
   }
   cout<<"pppppp"<<endl;
}

void GenPackWizPinsDescPage::but1Pushed()
{
   auto selItems = lWidget1->selectedItems();
   for(auto& item:selItems)
   {
      auto text = QInputDialog::getText(this,tr("Pin text"),tr("Pin text"),QLineEdit::EchoMode::Normal);
      QString pinText = item->text() + ":" + text;
      lWidget2->addItem(pinText);
      lWidget1->model()->removeRow(lWidget1->row(item));
   }
}

void GenPackWizPinsDescPage::but2Pushed()
{
   auto selItems = lWidget2->selectedItems();
   for(auto& item:selItems)
   {
      auto pinText = item->text();
      lWidget1->addItem(pinText.split(":").at(0));
      lWidget2->model()->removeRow(lWidget2->row(item));
   }
}

bool GenPackWizPinsDescPage::validatePage()
{
   map<int,QString> pinToTextMap;
   for(auto row = 0; row < lWidget2->count(); ++row)
   {
      auto item = lWidget2->item(row);
      auto text = item->text();
      auto textElements = text.split(":");
      auto pinNum = textElements[0].right(textElements[0].size() - 3).toInt();
      auto pinText = textElements[1];
      pinToTextMap[pinNum] = pinText;
   }
   GenericGraphicalItemsContainer *pContaniner =
        static_cast<GenericGraphicalItemsContainer*>(pData->item.get());
   auto children = pContaniner->getChildren();
   bool textInsidePackage = pData->pinPackDist > 0;
   bool bDip = field("dipType").toBool();
   vector<SmartPtr<GraphicalItem>> textVector;
   // children[0] - PackageGraphicalItem we dont take it into account
   for(size_t i = 1 ; i < children->size(); ++i)
   {
      auto pair = pinToTextMap.find(i - 1);
      if(pair != pinToTextMap.end())
      {
         auto& child = children->at(i);
         auto orientation = child->getType();
         QScreen* pScreen = QGuiApplication::primaryScreen();
         auto ratioY = pScreen->physicalDotsPerInchY()/72;
         auto fntSize = static_cast<int>(pData->pinDist * 0.7 * PIXELS_PER_MM /
                 ratioY);
         auto dif = abs(pData->pinDist) * 0.2;
         if(fntSize < fontSizes[0])
            fntSize = fontSizes[0];
/*
         // 4 * 2 = 8 characters per 2 text descriptions
         if(fntSize * pScreen->physicalDotsPerInchX()/72 * PIXELS_PER_MM * 8 >
            field("pacWidth").toDouble())
             textInsidePackage = false;
         if(fntSize * pScreen->physicalDotsPerInchY()/72 * PIXELS_PER_MM * 8 >
              field("pacHeight").toDouble())
             textInsidePackage = false;
*/
         float x = 0;
         float y = 0;
         auto pinPackDist = abs(pData->pinPackDist);
         switch (orientation)
         {
            case O_VERTICAL_TOP:
               y = textInsidePackage ? child->y() + 2 * pinPackDist :
                                    child->y() - 2 * pinPackDist ;
               x = child->x() + dif;
               break;
            case O_VERTICAL_BOTTOM:
               y = textInsidePackage ? child->y() - 2 * pinPackDist :
                                    child->y() + 2 * pinPackDist ;
               x = child->x() - dif;
               break;
            case O_HORIZONTAL_LEFT:
               x = textInsidePackage ? child->x() + 2 * pinPackDist :
                                      child->x() - 2 * pinPackDist ;
               y = child->y() - dif;
               break;
            case O_HORIZONTAL_RIGHT:
               x = textInsidePackage ? child->x() - 2 * pinPackDist :
                                    child->x() + 2 * pinPackDist ;
               y = child->y() + dif;
               break;

            default:
               break;

         }
         auto newItem = ItemsFactory::createText(x,y,pair->second,fntSize,orientation,
                             bDip? LEVEL_F1 : LEVEL_A1,ID_NONE,1,1);
         textVector.push_back(newItem);
      }
   }
   pContaniner->unGroupItems();
   children->insert(children->end(),textVector.begin(),textVector.end());
   pContaniner->groupItems();
}
