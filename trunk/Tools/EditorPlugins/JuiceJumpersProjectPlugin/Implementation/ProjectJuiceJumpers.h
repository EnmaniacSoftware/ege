#ifndef PLUGIN_PROJECTJUICEJUMPERS_H
#define PLUGIN_PROJECTJUICEJUMPERS_H

/*! Juice Jumpers project declaration. 
 */

#include <Projects/Project.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProjectJuiceJumpers : public Project
{
  public:

    ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path);
   ~ProjectJuiceJumpers();

    /*! Create method. */
    static Project* Create(QObject* parent, const QString& name, const QString& path);
    /*! Returns type name. */
    static QString TypeName();

  private:

    /*! @see Project::serialize. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /*! @see Project::unserialize. */
    bool unserialize(QXmlStreamReader& stream) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // PLUGIN_PROJECTJUICEJUMPERS_H
