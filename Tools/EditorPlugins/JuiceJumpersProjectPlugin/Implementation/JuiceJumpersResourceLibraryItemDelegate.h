#ifndef PLUGIN_JUICEJUMPERSRESOURCELIBRARYITEMDELEGATE_H
#define PLUGIN_JUICEJUMPERSRESOURCELIBRARYITEMDELEGATE_H

/*! Specialization of Resource item render delegate.
 */

#include <ResourceLibraryItemDelegate.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class JuiceJumpersResourceLibraryItemDelegate : public ResourceLibraryItemDelegate
{
  Q_OBJECT

  public:

    explicit JuiceJumpersResourceLibraryItemDelegate(QObject* parent = NULL);
   ~JuiceJumpersResourceLibraryItemDelegate();
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGIN_JUICEJUMPERSRESOURCELIBRARYITEMDELEGATE_H
