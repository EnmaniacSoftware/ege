#include "Core/UI/WidgetFactory.h"
#include "EGEDebug.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGE_DEFINE_NEW_OPERATORS(WidgetFactory)
EGE_DEFINE_DELETE_OPERATORS(WidgetFactory)
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
struct BuiltInWidget
{
  const char* name;
  egeWidgetCreateFunc pfCreateFunc;
};

static BuiltInWidget l_widgetsToRegister[] = {  { "dialog", Dialog::Create },
                                                { "push-button", PushButton::Create },
                                                { "scrollable-view", UIScrollableView::Create },
                                                { "label", Label::Create },
                                                { "scrollable-page-view", UIScrollablePageView::Create }
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WidgetFactory::WidgetFactory(Engine& engine) 
: m_engine(engine)                              
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
WidgetFactory::~WidgetFactory()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult WidgetFactory::construct()
{
  EGEResult result;

  // register build-in widget types
  for (u32 i = 0; i < sizeof (l_widgetsToRegister) / sizeof (BuiltInWidget); ++i)
  {
    const BuiltInWidget& widget = l_widgetsToRegister[i];

    if (EGE_SUCCESS != (result = registerWidget(widget.name, widget.pfCreateFunc)))
    {
      // error!
      return result;
    }
  }

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
EGEResult WidgetFactory::registerWidget(const String& typeName, egeWidgetCreateFunc createFunc)
{
  // check if resource with such a name exists already
  if (isWidgetRegistered(typeName))
  {
    // error!
    return EGE_ERROR_ALREADY_EXISTS;
  }

  // register
  WidgetRegistryEntry entry;
  entry.createFunc  = createFunc;

  m_registeredWidgets.insert(typeName, entry);

  return EGE_SUCCESS;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PWidget WidgetFactory::createWidget(const String& typeName, const String& name) const
{
  PWidget widget;

  // find widget with given type name
  const WidgetRegistryEntry& entry = m_registeredWidgets.value(typeName, WidgetRegistryEntry());
  if (NULL != entry.createFunc)
  {
    // create resource
    widget = entry.createFunc(engine(), name);
  }

  return widget;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
bool WidgetFactory::isWidgetRegistered(const String& typeName) const
{
  return m_registeredWidgets.contains(typeName);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
Engine& WidgetFactory::engine() const
{
  return m_engine;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END