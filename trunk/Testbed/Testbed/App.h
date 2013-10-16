#ifndef EGEAPP_H
#define EGEAPP_H

#include <EGEApplication.h>
#include <EGEString.h>
#include <EGEList.h>
#include <EGESignal.h>
#include "Test.h"

//--------------------------------------------------------------------------------------------------------------------------------------------------------------
class App : public EGE::Application
{
  public:

    App();
    virtual ~App();

    EGE_DECLARE_NEW_OPERATORS
    EGE_DECLARE_DELETE_OPERATORS

    /*! @see Application::construct. */
    EGE::EGEResult construct(const EGE::Dictionary& commandLineParams) override;
    /*! @see Application::version. */
    EGE::Version version() const override;

    bool start();

    /* Selects test. */
    bool selectTest();

  private:

    /* Application override. Application updater. */
    void update(const EGE::Time& time) override;

  private slots:

    /* Pointer event receiver. */
    void pointerEvent(EGE::PPointerData data);

	private:

    /*! Currently active test. NULL if none is active. */
    Test* m_activeTest;
    /*! List of all tests. */
    EGE::List<Test*> m_tests;
};
//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // EGEAPP_H