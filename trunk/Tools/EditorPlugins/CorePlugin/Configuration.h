#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <QWidget>
#include <QString>
#include "coreplugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_Configuration;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class COREPLUGIN_API Configuration : public QWidget
{
  Q_OBJECT

  public:

    Configuration(QWidget* parent = NULL);

  private slots:

    /* Slot called when object has been added into pool. */
    void onObjectAdded(QObject* object);
    /* Slot called when add button has been clicked. */
    void onAddClicked();
    /* Slot called when remove button has been clicked. */
    void onRemoveClicked();
    /* Slot called when onfiguration selection changed. */
    void onSelectionChanged();
    /* Slot called when configuration with given name is to be added. */
    void onAddConfiguration(const QString& name);
    
  private:

    /*! UI. */
    Ui_Configuration* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // CONFIGURATION_H