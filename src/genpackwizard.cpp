#include "genpackwizard.h"
#include "genpackwizstartpage.h"
#include "genwizdipgeompage.h"
#include "genpackwizpinsdescpage.h"

GenPackWizard::GenPackWizard()
{
  addPage(new GenPackWizStartPage(&data));
  addPage(new GenWizDipGeomPage(&data));
  addPage(new GenPackWizPinsDescPage(&data));
}
