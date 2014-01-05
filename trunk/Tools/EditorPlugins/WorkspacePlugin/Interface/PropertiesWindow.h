#ifndef WORKSPACE_PROPERTIESWINDOW_H
#define WORKSPACE_PROPERTIESWINDOW_H

/*! Properties window. It is used to display a set of properties for currently selected objects. */

#include <QDockWidget>
#include "WorkspacePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_PropertiesWindow;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertiesWindow : public QDockWidget
{
  Q_OBJECT

  public:

    explicit PropertiesWindow(QWidget* parent = NULL);
   ~PropertiesWindow();

    /*! Saves settings. */
    void saveSettings();
    /*! Loads settings. */
    void loadSettings();

  private:

    /*! UI. */
    Ui_PropertiesWindow* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_PROPERTIESWINDOW_H
