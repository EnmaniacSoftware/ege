#ifndef WORKSPACE_CHANGECONFIGURATIONNAMEWINDOW_H
#define WORKSPACE_CHANGECONFIGURATIONNAMEWINDOW_H

#include <QDialog>
#include <QStringList>
#include "ui_ChangeNameDialog.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ChangeConfigurationNameWindow : public QDialog, private Ui::Dialog
{
  Q_OBJECT

  public:

    ChangeConfigurationNameWindow(const QString& initialName, const QStringList& restrictedNames, QWidget* parent = NULL);

  signals:

    /*! Signal emitted when configuration with given name was requested. */
    void accepted(const QString& name);

  private slots:

    /*! Slot called when dialog was accepted. */
    void onAccepted();
    /*! Slot called when text in edit changes. */
    void onTextChanged(const QString& text);

  private:

    /*! List of names of not allowed configuration names. */
    const QStringList& m_restrictedNames;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_CHANGECONFIGURATIONNAMEWINDOW_H
