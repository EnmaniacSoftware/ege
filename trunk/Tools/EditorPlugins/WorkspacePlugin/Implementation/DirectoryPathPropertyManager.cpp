#include "DirectoryPathPropertyManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DirectoryPathPropertyManager::DirectoryPathPropertyManager(QObject* parent) : QtAbstractPropertyManager(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
DirectoryPathPropertyManager::~DirectoryPathPropertyManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathPropertyManager::setValue(QtProperty* property, const QString& value)
{
  // check if property is part of this manager
  if (m_values.contains(property))
  {
    Data data = m_values[property];

    if (data.value != value)
    {
      data.value = value;
      m_values[property] = data;

      // set tool tip
      property->setToolTip(value);

      // notify
      emit propertyChanged(property);
      emit valueChanged(property, data.value);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString DirectoryPathPropertyManager::value(const QtProperty* property) const
{
  QString val;

  if (m_values.contains(property))
  {
    val = m_values[property].value;
  }

  return val;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString DirectoryPathPropertyManager::valueText(const QtProperty* property) const
{
  return value(property);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathPropertyManager::initializeProperty(QtProperty* property)
{
  m_values[property] = Data();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void DirectoryPathPropertyManager::uninitializeProperty(QtProperty* property)
{
  m_values.remove(property);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
