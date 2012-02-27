#ifndef JUICE_JUMPERS_PROJECT_H
#define JUICE_JUMPERS_PROJECT_H

#include "Project.h"

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

    /* Project override. Serializes into given buffer. */
    QString serialize() const override;
    /* Project override. Unserializes from given data buffer. */
    bool unserialize(const QString& data) override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // JUICE_JUMPERS_PROJECT_H