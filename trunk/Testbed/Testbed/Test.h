#ifndef TEST_H
#define TEST_H

#include <EGEString.h>
#include <EGETime.h>
#include <EGEInput.h>
#include <EGESignal.h>

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class App;

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

class Test
{
  public:

    Test(App* app);
    virtual ~Test();

    /*! Returns test name. */
    virtual EGE::String name() const = 0;
    /*! Initializes test. */
    virtual bool initialize() = 0;
    /*! Updates test. */
    virtual void update(const EGE::Time& time) = 0;
    /* Pointer event receiver. */
    virtual void pointerEvent(EGE::PPointerData data);

  protected:

    /*! Returns point to app object. */
    inline App* app() const { return m_app; }

  protected slots:

    /* Slot called when resource group has been loaded. */
    virtual void groupLoadComplete(const EGE::String& name);
    /* Slot called when resource group could not be loaded. */
    virtual void groupLoadError(const EGE::String& name);

  private:
    
    /*! Pointer to app object. */
    App* m_app;
};

//--------------------------------------------------------------------------------------------------------------------------------------------------------------

#endif // TEST_H