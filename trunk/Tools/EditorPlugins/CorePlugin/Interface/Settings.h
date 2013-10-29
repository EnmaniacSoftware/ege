#ifndef CORE_CONFIG_H
#define CORE_CONFIG_H

#include <QSettings>
#include "CorePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API Settings : public QSettings
{
  Q_OBJECT

  public:

   explicit Settings(QObject* parent = NULL);
   ~Settings();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CORE_CONFIG_H
