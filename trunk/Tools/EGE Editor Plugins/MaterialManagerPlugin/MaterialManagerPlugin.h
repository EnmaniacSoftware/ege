#ifndef MATERIALMANAGERPLUGIN_H
#define MATERIALMANAGERPLUGIN_H

#include <Plugins/IPlugin.h>
#include "materialmanagerplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MaterialManagerWindow;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class MATERIALMANAGERPLUGIN_API MaterialManagerPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit MaterialManagerPlugin(QObject* parent = NULL);
    ~MaterialManagerPlugin();

  private:

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;

  private:

    /*! Module window. */
    MaterialManagerWindow* m_window;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // MATERIALMANAGERPLUGIN_H
