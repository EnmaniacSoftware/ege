#ifndef WORKSPACE_PROPERTIESWINDOW_H
#define WORKSPACE_PROPERTIESWINDOW_H

/*! Properties window. It is used to display a set of properties for currently selected objects. */

#include <QDockWidget>
#include <QList>
#include "PropertyObject.h"
#include "PropertyDefinition.h"
#include "WorkspacePlugin_global.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class Ui_PropertiesWindow;
class QtBoolPropertyManager;
class QtIntPropertyManager;
class QtStringPropertyManager;
class QtSizePropertyManager;
class QtRectPropertyManager;
class QtEnumPropertyManager;
class QtGroupPropertyManager;
class QtProperty;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WORKSPACEPLUGIN_API PropertiesWindow : public QDockWidget
{
  Q_OBJECT

  public:

    explicit PropertiesWindow(QWidget* parent = NULL);
   ~PropertiesWindow();

    /*! Attaches given property object for display and manipulation.
     *  @param  object  Object to be attached to window.
     */
    void attach(NPropertyObject::PropertyObject* object);

    /*! Saves settings. */
    void saveSettings();
    /*! Loads settings. */
    void loadSettings();

  private:

    /*! Populates view with given list of property definitions.
     *  @param  properties  List of property definitions used with population of a view.
     */
    void populateFromDefinition(const QList<NPropertyObject::PropertyDefinition>& properties);
    /*! Creates property based on given definition including all children.
     *  @param  definition  Property definition based on which new view property is created.
     *  @return Property object created from definition. NULL if error occured.
     *  @note Created property object is hierarchical in the same manner as its definition.
     */
    QtProperty* createAndPopulatePropertyFromDefinition(const NPropertyObject::PropertyDefinition& definition) const;
    /*! Creates property from given data.
     *  @param  type  Property type to create.
     *  @param  name  Property name.
     *  @param  value Property value.
     *  @return Created property object. NULL if error occured.
     */
    QtProperty* createProperty(const NPropertyObject::PropertyType& type, const QString& name, const QString& value) const;

  private:

    /*! UI. */
    Ui_PropertiesWindow* m_ui;
    /*! Property manager for boolean types. */
    QtBoolPropertyManager* m_boolManager;
    /*! Property manager for integer types. */
    QtIntPropertyManager* m_intManager;
    /*! Property manager for string types. */
    QtStringPropertyManager* m_stringManager;
    /*! Property manager for size types. */
    QtSizePropertyManager* m_sizeManager;
    /*! Property manager for rectanulgar types. */
    QtRectPropertyManager* m_rectManager;
    /*! Property manager for enumeration types. */
    QtEnumPropertyManager* m_enumManager;
    /*! Property manager for group types. */
    QtGroupPropertyManager* m_groupManager;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // WORKSPACE_PROPERTIESWINDOW_H
