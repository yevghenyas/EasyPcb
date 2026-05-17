#include "resultsautodlg.h"
#include <QPushButton>
#include <QMessageBox>
#include <connectorgraphicalitem.h>

ResultsAutoDlg::ResultsAutoDlg(QWidget *parent,const multimap<QString,SmartPtr<GraphicalItem>>& bestProc,
                               map<QString,SmartPtr<GraphicalItem>> *vcCons):QDialog(parent)
  ,m_BestProc(bestProc)
{
   if (objectName().isEmpty())
       setObjectName(QString::fromUtf8("Dialog"));
   resize(620, 294);
   buttonBox = new QDialogButtonBox(this);
   buttonBox->setObjectName(QString::fromUtf8("buttonBox"));
   buttonBox->setGeometry(QRect(520, 20, 81, 241));
   buttonBox->setOrientation(Qt::Vertical);
   QPushButton *pOkBut = new QPushButton("Ok",this);
   buttonBox->addButton(pOkBut,QDialogButtonBox::AcceptRole);
   QPushButton *pShowBut = new QPushButton("Show",this);
   buttonBox->addButton(pShowBut,QDialogButtonBox::ActionRole);

   m_pListWidget = new QListWidget(this);
   m_pListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
   m_pListWidget->setObjectName(QString::fromUtf8("ListWidget"));
   m_pListWidget->setGeometry(QRect(1, 0, 500, 270));
   set<QString> ids;
   for(auto& item:bestProc)
   {
      //add here code to look for a name of connector in case it has one
      if(ids.find(item.first) != ids.end())
          continue;
      ids.insert(item.first);
      auto con = vcCons->find(item.first);
      if(con != vcCons->end() && strlen(con->second->getName()) > 0)
      {
         m_pListWidget->addItem(con->second->getName());
         m_NameToId[con->second->getName()] = item.first;
      }
      else
      {
         m_pListWidget->addItem(item.first);
      }
   }
   connect(pShowBut,&QPushButton::clicked,this,&ResultsAutoDlg::showConstructedConnectors);
   connect(pOkBut,&QPushButton::clicked,this,&ResultsAutoDlg::accept);
}

void ResultsAutoDlg::showConstructedConnectors()
{
   const auto& selItems = m_pListWidget->selectedItems();
   if(selItems.size() > 0)
   {
      selectConstrCons(selConnectorName,false);
      auto id_iter = m_NameToId.find(selItems.first()->text());
      if(id_iter != m_NameToId.end())
         selConnectorName = id_iter->second;
      else
         selConnectorName = selItems.first()->text();
      selectConstrCons(selConnectorName,true);
   }
   else
   {
      QMessageBox box;
      box.setText("Some text shown here");
      box.exec();
   }
}

void ResultsAutoDlg::selectConstrCons(const QString& key, bool bSelect)
{
   if(!key.isEmpty())
   {
      auto range = m_BestProc.equal_range(key);
      for(auto& it = range.first; it != range.second ; ++it)
      {
         auto item = it->second;
         auto p = static_cast<ConnectorGraphicalItem*>(item.get());
//         cout<<"address="<<item.get()<<endl;
//         cout<<"x1,y1="<<p->getFirstPoint().x()<<","<<p->getFirstPoint().y()<<"x2,y2="<<
//               p->getLastPoint().x()<<","<<p->getLastPoint().y()<<endl;
         item->setSelected(bSelect);
      }
      static_cast<QWidget*>(parent())->repaint();
   }
}

void ResultsAutoDlg::accept()
{
   selectConstrCons(selConnectorName,false);
   static_cast<QWidget*>(parent())->repaint();
   QDialog::accept();
}
