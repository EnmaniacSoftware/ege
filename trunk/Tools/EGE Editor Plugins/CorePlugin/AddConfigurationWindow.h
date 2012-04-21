#ifndef ADD_CONFIGURATION_WINDOW_H
#define ADD_CONFIGURATION_WINDOW_H

#include <QDialog>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_AddConfiguration;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API AddConfigurationWindow : public QDialog
{
  Q_OBJECT

  public:

    AddConfigurationWindow(QWidget* parent = NULL);

  signals:

    /*! Signal emitted when configuration with given name was requested. */
    void accepted(const QString& name);

  private slots:

    /* Slot called when dialog was accepted. */
    void onAccepted();
    /* Slot called when text in edit changes. */
    void onTextChanged(const QString& text);

  private:

    /*! UI. */
    Ui_AddConfiguration* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // ADD_CONFIGURATION_WINDOW_H