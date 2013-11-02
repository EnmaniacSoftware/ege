#ifndef PLUGIN_PROJECTJUICEJUMPERS_H
#define PLUGIN_PROJECTJUICEJUMPERS_H

/*! Juice Jumpers project declaration. 
 */

#include <Projects/Project.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class JuiceJumpersResourceLibraryItemDelegate;
//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class JuiceJumpersProject : public Project
{
  public:

    JuiceJumpersProject(QObject* parent, const QString& name, const QString& path);
   ~JuiceJumpersProject();

    /*! Create method. */
    static Project* Create(QObject* parent, const QString& name, const QString& path);
    /*! Returns type name. */
    static QString TypeName();

  private:

    /*! @see Project::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see Project::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;
    /*! @see Project::resourceLibraryItemDelegate. */
    QStyledItemDelegate* resourceLibraryItemDelegate() const override;

  private:

    /*! Resource library item delegate. */
    JuiceJumpersResourceLibraryItemDelegate* m_resourceItemDelegate;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGIN_PROJECTJUICEJUMPERS_H
