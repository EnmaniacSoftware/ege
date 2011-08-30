#ifndef EGEAPP_H
#define EGEAPP_H

#include <EGEApplication.h>
#include <EGEString.h>
#include <EGEList.h>
#include "Test.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class App : public EGE::Application
{
  public:

    App();
    virtual ~App();

    bool start();

    /* Selects test. */
    bool selectTest();

  private:

    /* Application override. Application updater. */
    void update(const EGE::Time& time) override;

	private:

    /*! Currently active test. NULL if none is active. */
    Test* m_activeTest;
    /*! List of all tests. */
    EGE::List<Test*> m_tests;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGEAPP_H