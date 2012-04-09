#ifndef IMAGEDANIMATIONPLUGIN_H
#define IMAGEDANIMATIONPLUGIN_H

#include <Plugins/IPlugin.h>
#include "imagedanimationplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ImagedAnimationWindow;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class IMAGEDANIMATIONPLUGIN_API ImagedAnimationPlugin : public QObject, public IPlugin
{
  Q_OBJECT
  Q_INTERFACES(IPlugin)

  public:
  
    explicit ImagedAnimationPlugin(QObject* parent = NULL);
    ~ImagedAnimationPlugin();

  private:

    /* IPlugin override. Initialized plugin. */
    bool initialize() override;
    /* IPlugin override. Deinitializes plugin. */
    void deinitialize() override;

  private:

    /* Module window. */
    ImagedAnimationWindow* m_imagedAnimationWindow;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // IMAGEDANIMATIONPLUGIN_H
