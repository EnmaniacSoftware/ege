#ifndef WORKSPACE_ADDCONFIGURATIONWINDOW_H
#define WORKSPACE_ADDCONFIGURATIONWINDOW_H

#include <QDialog>
#include "ui_addconfiguration.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class AddConfigurationWindow : public QDialog, public Ui::AddConfiguration
{
  Q_OBJECT

  public:

    AddConfigurationWindow(QWidget* parent = NULL);

  signals:

    /*! Signal emitted when configuration with given name was requested. */
    void accepted(const QString& name);

  private slots:

    /*! Slot called when dialog was accepted. */
    void onAccepted();
    /*! Slot called when text in edit changes. */
    void onTextChanged(const QString& text);
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_ADDCONFIGURATIONWINDOW_H
