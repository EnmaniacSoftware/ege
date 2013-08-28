#ifndef RESOURCELIBRARYINTERFACE_H
#define RESOURCELIBRARYINTERFACE_H

/*! Resource library interface. */

#include "resoucelibraryplugin_global.h"
#include "Serializer.h"

class RESOUCELIBRARYPLUGIN_API IResourceLibrary : public ISerializer
{
  public:
};

Q_DECLARE_INTERFACE(IResourceLibrary, "com.littlebeestudios.plugin.iresourcelibrary/1.0");

#endif // RESOURCELIBRARYINTERFACE_H