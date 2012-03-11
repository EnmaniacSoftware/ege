#ifndef JUICE_JUMPERS_PROJECT_H
#define JUICE_JUMPERS_PROJECT_H

/*! Juice Jumpers project declaration. 
 */

#include "Projects/Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProjectJuiceJumpers : public Project
{
  public:

    ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path);
   ~ProjectJuiceJumpers();

    /* Create method. */
    static Project* Create(QObject* parent, const QString& name, const QString& path);
    /* Returns type name. */
    static QString TypeName();

  private:

    /* Project override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* Project override. Unserializes from given data stream. */
    bool unserialize(const QXmlStreamReader& stream) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // JUICE_JUMPERS_PROJECT_H