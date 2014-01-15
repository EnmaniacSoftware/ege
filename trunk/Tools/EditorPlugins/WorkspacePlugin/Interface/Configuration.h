#ifndef WORKSPACE_CONFIGURATION_H
#define WORKSPACE_CONFIGURATION_H

#include <QWidget>
#include <QString>
#include "WorkspacePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class QXmlStreamWriter;
class QXmlStreamReader;
class Ui_Configuration;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API Configuration : public QWidget
{
  Q_OBJECT

  public:

    Configuration(QWidget* parent = NULL);

  signals:

    /*! Signal emitted when current configuration changes.
     *  @param  name  Name of current configuration.
     */
    void changed(const QString& name);
    /*! Signal emitted when current configuration name has been changed.
     *  @param  oldName Old name of current configuration.
     *  @param  newName New name of current configuration.
     */
    void nameChanged(const QString& oldName, const QString& newName);

  public:

    /*! Returns name of currently selected configuration. */
    QString current() const;

  private slots:

    /*! Slot called when add button has been clicked. */
    void onAddClicked();
    /*! Slot called when remove button has been clicked. */
    void onRemoveClicked();
    /*! Slot called when edit button has been clicked. */
    void onEditClicked();
    /*! Slot called when onfiguration selection changed. */
    void onSelectionChanged();
    /*! Slot called when configuration with given name is to be added. */
    void onAddConfiguration(const QString& name);
    /*! Slot called when new object has been added into the pool. */
    void onObjectAdded(QObject* object);
    /*! Slot called when object is about to be removed from pool. */
    void onObjectRemoved(QObject* object);
    /*! Slot called when saving is required.
     *  @param  stream  XML stream where data is to be saved.
     */
    void onSaveData(QXmlStreamWriter& stream);
    /*! Slot called when loading is required.
     *  @param  stream  XML stream where data is to be saved.
     */
    void onLoadData(QXmlStreamReader& stream);
    /*! Slot called when new name for current configuration changes.
     *  @param  name  New name.
     */
    void onConfigurationNameChanged(const QString& name);

  private:

    /*! Removes all defined configurations. */
    void removeAll();
    /*! Creates default configuration. */
    void createDefault();
    /*! Updates UI. */
    void updateUI();

  private:

    /*! UI. */
    Ui_Configuration* m_ui;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_CONFIGURATION_H
