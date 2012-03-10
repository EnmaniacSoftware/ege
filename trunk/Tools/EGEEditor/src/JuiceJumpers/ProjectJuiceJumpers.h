#ifndef JUICE_JUMPERS_PROJECT_H
#define JUICE_JUMPERS_PROJECT_H

#include "Projects/Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
/*! Juice Jumpers project. */
class ProjectJuiceJumpers : public Project
{
  public:

    ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path);
   ~ProjectJuiceJumpers();

    /* Create method. */
    static Project* Create(QObject* parent, const QString& name, const QString& path);

  private:

    /* Project override. Serializes into given stream. */
    bool serialize(QXmlStreamWriter& stream) const override;
    /* Project override. Unserializes from given data stream. */
    bool unserialize(const QXmlStreamReader& stream) override;
    /* Project override. Returns project type name. */
    QString typeName() const override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // JUICE_JUMPERS_PROJECT_H