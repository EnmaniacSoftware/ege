#ifndef JUICE_JUMPERS_PROJECT_H
#define JUICE_JUMPERS_PROJECT_H

#include "Project.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class ProjectJuiceJumpers : public Project
{
  public:

    ProjectJuiceJumpers(QObject* parent, const QString& name, const QString& path);
   ~ProjectJuiceJumpers();

    /* Create method. */
    static Project* Create(QObject* parent, const QString& name, const QString& path);

  private:

    /* Project override. Saves all data. */
    bool save() override;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // JUICE_JUMPERS_PROJECT_H