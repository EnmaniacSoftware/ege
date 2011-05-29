#ifndef EGE_CORE_CONTAINER_MULTIMAP_H
#define EGE_CORE_CONTAINER_MULTIMAP_H

#include "EGE.h"

EGE_NAMESPACE_BEGIN

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

template <typename T, typename U>
class EGEMultiMap : public std::multimap<T, U>
{
  public:

};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

EGE_NAMESPACE_END

#endif // EGE_CORE_CONTAINER_MULTIMAP_H