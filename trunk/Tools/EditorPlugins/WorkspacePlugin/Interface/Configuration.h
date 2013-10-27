#ifndef WORKSPACE_CONFIGURATION_H
#define WORKSPACE_CONFIGURATION_H

#include <QWidget>
#include <QString>
#include "WorkspacePlugin_global.h"
#include "ui_configuration.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API Configuration : public QWidget, public Ui::Configuration
{
  Q_OBJECT

  public:

    Configuration(QWidget* parent = NULL);

  private slots:

    /*! Slot called when add button has been clicked. */
    void onAddClicked();
    /*! Slot called when remove button has been clicked. */
    void onRemoveClicked();
    /*! Slot called when onfiguration selection changed. */
    void onSelectionChanged();
    /*! Slot called when configuration with given name is to be added. */
    void onAddConfiguration(const QString& name);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_CONFIGURATION_H
