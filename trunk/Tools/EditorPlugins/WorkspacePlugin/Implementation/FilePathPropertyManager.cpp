#include "FilePathPropertyManager.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathPropertyManager::FilePathPropertyManager(QObject* parent) : QtAbstractPropertyManager(parent)
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
FilePathPropertyManager::~FilePathPropertyManager()
{
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathPropertyManager::setValue(QtProperty* property, const QString& value)
{
  // check if property is part of this manager
  if (m_values.contains(property))
  {
    Data data = m_values[property];

    if (data.value != value)
    {
      data.value = value;
      m_values[property] = data;

      // notify
      emit propertyChanged(property);
      emit valueChanged(property, data.value);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathPropertyManager::setFilter(QtProperty* property, const QString& value)
{
  // check if property is part of this manager
  if (m_values.contains(property))
  {
    Data data = m_values[property];

    if (data.filter != value)
    {
      data.filter = value;
      m_values[property] = data;

      // notify
      emit propertyChanged(property);
      emit filterChanged(property, data.filter);
    }
  }
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString FilePathPropertyManager::value(const QtProperty* property) const
{
  QString val;

  if (m_values.contains(property))
  {
    val = m_values[property].value;
  }

  return val;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString FilePathPropertyManager::filter(const QtProperty* property) const
{
  QString val;

  if (m_values.contains(property))
  {
    val = m_values[property].filter;
  }

  return val;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
QString FilePathPropertyManager::valueText(const QtProperty* property) const
{
  return value(property);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathPropertyManager::initializeProperty(QtProperty* property)
{
  m_values[property] = Data();
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
void FilePathPropertyManager::uninitializeProperty(QtProperty* property)
{
  m_values.remove(property);
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
