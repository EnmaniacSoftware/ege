#ifndef EGE_CORE_WIDGETFACTORY_H
#define EGE_CORE_WIDGETFACTORY_H

/** 
 *   Factory (utility) class creating widgets. It is possible to register custom widgets so they can be created through factory.
 */

#include <EGE.h>
#include <EGEUI.h>

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
typedef PWidget (*egeWidgetCreateFunc)(Application* app, const String& name);
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class WidgetFactory : public Object
{
  public:

    WidgetFactory(Application* app);
   ~WidgetFactory();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! Creates object. */
    EGEResult construct();
    /*! Registeres custom particle emitter type. */
    EGEResult registerWidget(const String& typeName, egeWidgetCreateFunc createFunc);
    /*! Creates instance of particle emitter of the type given by name. */
    PWidget createWidget(const String& typeName, const String& name) const;
    /*! Returns TRUE if given emitter type is registered. */
    bool isWidgetRegistered(const String& typeName) const;

  private:

    /*! Class containing registration information for widget. */
    class WidgetRegistryEntry
    {
      public:

        egeWidgetCreateFunc createFunc;
    };

  private:

    typedef Map<String, WidgetRegistryEntry> WidgetRegisterMap;

  private:

    /*! Registered widgets sorted by type name. */
    WidgetRegisterMap m_registeredWidgets;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_WIDGETFACTORY_H
