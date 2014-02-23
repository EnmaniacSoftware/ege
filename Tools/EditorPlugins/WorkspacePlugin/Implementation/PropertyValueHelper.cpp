#include "PropertyValueHelper.h"

using namespace NPropertyObject;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyValueContainer PropertyValueHelper::CreateIntegerValue(int value, int min, int max)
{
  PropertyValueContainer values;

  values << value << min << max;

  return values;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
PropertyValueContainer PropertyValueHelper::CreateFilePathValue(const QString& name, const QString& filter, bool fileMustExist)
{
  PropertyValueContainer values;

  values << name << filter << fileMustExist;

  return values;
}
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
